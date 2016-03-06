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
 * to the RIOT debug UART Interface.
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
#include "thread.h"
#include "xtimer.h"

#define IDLE_MSG_QUEUE_SIZE   (8)       /* message queue size of the idle thread */
#define SERVER_MSG_QUEUE_SIZE (32)      /* message queue size of the server thread */
#define SERVER_BUFFER_SIZE    (32)      /* max size of the buffer where incoming data is stored  */
#define MAX_MESSAGE_SIZE      (64)      /* max size of the forwarded message */

static msg_t _main_msg_queue[IDLE_MSG_QUEUE_SIZE];
static uint16_t SERVER_PORT=8000;
static conn_udp_t server_conn;
static char server_buffer[SERVER_BUFFER_SIZE];
static char server_stack[THREAD_STACKSIZE_DEFAULT];
static msg_t server_msg_queue[SERVER_MSG_QUEUE_SIZE];
static kernel_pid_t idle_thread_pid;

static void *server_thread(void *args)
{
    ipv6_addr_t server_addr = IPV6_ADDR_UNSPECIFIED;
    msg_init_queue(server_msg_queue, SERVER_MSG_QUEUE_SIZE);

    /* Create udp incoming connection */
    conn_udp_create(&server_conn, &server_addr, sizeof(server_addr), AF_INET6, SERVER_PORT);

    ipv6_addr_t addr_src;
    char addr_src_str[IPV6_ADDR_MAX_STR_LEN] = { 0 };
    size_t addr_len = sizeof(ipv6_addr_t);
    char message[MAX_MESSAGE_SIZE] = { 0 };
    
    /* Endless loop waiting for incoming packets */
    for(;;) {
	/* clear incoming data server buffer */
	memset(server_buffer, 0, sizeof(server_buffer));
	
	/* wait for incoming packets */
        conn_udp_recvfrom(&server_conn, server_buffer, sizeof(server_buffer), &addr_src,
			  &addr_len, &SERVER_PORT);
	
	/* build a nice message string */
	ipv6_addr_to_str(addr_src_str, &addr_src, IPV6_ADDR_MAX_STR_LEN);
	snprintf(message, sizeof(message), "%s from: %s\n", server_buffer, addr_src_str);

	/* forward the message to the idle thread */
	/* msg_send_receive blocks until it receives the idle thread
	 * aknowledgment */
	msg_t msg;
	msg.content.value = (uint32_t)&message;
	msg_send_receive(&msg, NULL, idle_thread_pid);
    }
    
    return NULL;
}

int main(void)
{
    /* write a welcome message on the terminal */
    puts("Temperature sensor network server started\n");

    /* we need a message queue for the idle thread in order to
     * receive potentially fast incoming networking packets */
    msg_init_queue(_main_msg_queue, IDLE_MSG_QUEUE_SIZE);

    /* create the thread that will handle the udp server */
    thread_create(server_stack, sizeof(server_stack), THREAD_PRIORITY_MAIN - 1,
		  THREAD_CREATE_STACKTEST, server_thread, NULL, "IP server");
    
    /* get idle thread pid */
    idle_thread_pid = thread_getpid();

    msg_t msg;
    char * message = NULL;
    for (;;) {
	/* block the execution until a message is received */
	msg_receive(&msg);
	message = (char*)msg.content.value;
	
	/* write the message on terminal */
	printf("%s", message);
	
	/* acknowledge the server thread */
	msg_reply(&msg, NULL);
    }
    
    /* should be never reached */
    return 0;
}
