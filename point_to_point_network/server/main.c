/*
 * Copyright (C) 2016 Inria
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v3. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @{
 *
 * @brief       Sample application redirecting packet received on an udp server
 * to an UART interface.
 *
 * @author      Alexandre Abadie <alexandre.abadie@inria.fr>
 *
 * @}
 */

#include <stdio.h>
#include "msg.h"
#include "net/af.h"
#include "net/conn/udp.h"
#include "net/gnrc/ipv6.h"
#include "periph/uart.h"
#include "thread.h"
#include "xtimer.h"

#define OUTPUT_UART 1
#define BAUDRATE (115200U)

#define MAIN_QUEUE_SIZE     (8)
#define SERVER_MSG_QUEUE_SIZE   (32)
#define SERVER_BUFFER_SIZE      (64)

static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static uint16_t SERVER_PORT=8000;

static conn_udp_t server_conn;
static char server_buffer[SERVER_BUFFER_SIZE] = { 0 };
static char server_stack[THREAD_STACKSIZE_DEFAULT];
static msg_t server_msg_queue[SERVER_MSG_QUEUE_SIZE];
static kernel_pid_t idle_thread_pid;

static void *server_thread(void *args)
{
    ipv6_addr_t server_addr = IPV6_ADDR_UNSPECIFIED;
    msg_init_queue(server_msg_queue, SERVER_MSG_QUEUE_SIZE);

    /* Create udp incoming connection */
    conn_udp_create(&server_conn, &server_addr, sizeof(server_addr), AF_INET6, SERVER_PORT);

    /* Endless loop waiting for incoming packets */
    ipv6_addr_t src;
    char src_str[IPV6_ADDR_MAX_STR_LEN];
    char from[IPV6_ADDR_MAX_STR_LEN + 7] = { 0 };
    strcpy(from, " from: ");
    size_t src_len = sizeof(ipv6_addr_t);
    ipv6_addr_to_str(src_str, &src, IPV6_ADDR_MAX_STR_LEN);
    strcat(from, src_str);
    for(;;) {
	/* wait for incoming packets */
        conn_udp_recvfrom(&server_conn, server_buffer, sizeof(server_buffer), &src,
			  &src_len, &SERVER_PORT);
	char full_message[sizeof(from) + sizeof(server_buffer)];
	strcpy(full_message, server_buffer);
	strcat(full_message, from);
	/* Forward received message address to idle thread */
	msg_t msg;
	msg.content.value = (uint32_t)&full_message;
	msg_send(&msg, idle_thread_pid);
    }
    return NULL;
}

/* dummy uart callback function, just to avoid a crash a char is received on
   this interface */
static void rx_cb(void *uart, char c) {}

int main(void)
{
    /* we need a message queue for the idle thread in order to
     * receive potentially fast incoming networking packets */
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    puts("Temperature sensor network server");

    /* create the thread that will handle the udp server */
    thread_create(server_stack, sizeof(server_stack), THREAD_PRIORITY_MAIN - 1,
		  THREAD_CREATE_STACKTEST, server_thread, NULL, "IP server");
    
    /* Initializing output UART interface */
    uart_init(OUTPUT_UART, BAUDRATE, rx_cb, (void *)OUTPUT_UART);
    

    /* Get Idle thread pid */
    idle_thread_pid = thread_getpid();

    msg_t msg;
    char * message = NULL;
    for (;;) {
	uart_write(OUTPUT_UART, (uint8_t*)"\n", 1); /* hacky way of unlocking
						     * this loop*/
	msg_receive(&msg);
	message = (char*)msg.content.value;
	/* Forward message to OUTPUT UART, we have to know the length of the message */
	uart_write(OUTPUT_UART, (uint8_t*)message, strlen(message));
    }
    
    /* should be never reached */
    return 0;
}
