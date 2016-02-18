/*
 * Copyright (C) 2014 Freie Universität Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @{
 *
 * @file
 * @brief       Serial to serial communication
 *
 * @}
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "board.h"
#include "ringbuffer.h"
#include "periph/uart.h"

#define UART_BUFSIZE        (128U)

typedef struct {
    char rx_mem[UART_BUFSIZE];
    ringbuffer_t rx_buf;
	} uart_ctx_t;

static uart_ctx_t ctx[UART_NUMOF];

static void rx_cb(void *arg, char data)
{
	ringbuffer_t *buf = (ringbuffer_t *)arg;
    ringbuffer_add_one(buf, data);
}

int main(void)
{
    puts("Connection between 2 UART interfaces");
    puts("=====================================");

    /* initialize ringbuffers and serial devices */
	/* UART0 : PC terminal */
    ringbuffer_init(&(ctx[0].rx_buf), ctx[0].rx_mem, UART_BUFSIZE);
	uart_init(0, (uint32_t)9600, rx_cb, (void *)&(ctx[0].rx_buf));
	
	/* UART2 : BT terminal */
	ringbuffer_init(&(ctx[2].rx_buf), ctx[2].rx_mem, UART_BUFSIZE);
	uart_init(2, (uint32_t)9600, rx_cb, (void *)&(ctx[2].rx_buf));
	
	for (;;) {
	  while (ctx[2].rx_buf.avail) {
		// We have data coming from UART2 => simply print on PC terminal
		int data = ringbuffer_get_one(&(ctx[2].rx_buf));
		printf("%c", (char)data);
	  }

	  while (ctx[0].rx_buf.avail) {
		// We have data printed PC terminal => write them on UART2
		int data = ringbuffer_get_one(&(ctx[0].rx_buf));
		uart_write(2, (uint8_t*)&data, 1);
	  }
	}
	
	return 0;
}
