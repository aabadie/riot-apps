/*
 * Copyright (C) 2016 Inria
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v3. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @{
 * @brief Uart to uart communication program
 *
 * @}
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "board.h"
#include "shell.h"
#include "msg.h"
#include "thread.h"
#include "periph/uart.h"

#define BT_UART             (1)    /* use the second UART to communicate with BT module */
#define SHELL_BUFSIZE       (128U)

#define PRINTER_PRIO        (THREAD_PRIORITY_MAIN - 1)

static kernel_pid_t printer_pid;
static char printer_stack[THREAD_STACKSIZE_MAIN];

static void rx_cb(void *uart, char c)
{
    /* A character was received on an UART interface and triggered
       this callback through an interruption, we forward it via a message
       to the idle thread. */
    msg_t msg;
    msg.type = (int)uart;
    msg.content.value = (uint32_t)c;
    msg_send(&msg, printer_pid);
}

static void *printer(void *arg)
{
    (void)arg;
    
    /* Local variable containing the message exchanged between 
       the 2 UART interfaces */
    msg_t msg;
    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);
    for (;;) {
	/* Next line blocks the loop until a message is received on the idle 
	   thread */
	msg_receive(&msg);
	printf("%c", (char)msg.content.value);
    }

    /* this should never be reached */
    return NULL;
}

static int cmd_init(int argc, char **argv)
{
    int res;
    uint32_t baud;

    if (argc < 3) {
        printf("usage: %s <baudrate> <init command>\n", argv[0]);
        return 1;
    }

    baud = (uint32_t)atoi(argv[1]);

    /* initialize UART */
    res = uart_init(BT_UART, baud, rx_cb, (void *)BT_UART);
    if (res == -1) {
        printf("Error: Given baudrate (%u) not possible\n", (unsigned int)baud);
        return 1;
    }
    else if (res < -1) {
        puts("Error: Unable to initialize bluetooth device\n");
        return 1;
    }
    printf("Successfully initialized bluetooth device\n");

    uart_write(BT_UART, (uint8_t *)argv[2], strlen(argv[2]));

    return 0;
}

static int cmd_send(int argc, char **argv)
{   
    if (argc < 2) {
        printf("Usage: %s <command>\n", argv[0]);
        return 1;
    }
    
    uart_write(BT_UART, (uint8_t *)strcat(argv[1], "\r\n"), strlen(argv[1]) + 2);
    return 0;
}

static const shell_command_t shell_commands[] = {
    { "init", "Start bluetooth module configuration mode", cmd_init },
    { "send", "Send a command to the bluetooth module", cmd_send },
    { NULL, NULL, NULL }
};

int main(void)
{
    printf("\nBluetooth module configuration tool\n");
    printf("===================================\n");

    /* start the printer thread */
    printer_pid = thread_create(printer_stack, sizeof(printer_stack),
                                PRINTER_PRIO, 0, printer, NULL, "printer");

    /* run the shell */
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}
