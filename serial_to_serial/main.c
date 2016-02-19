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
#define PC_UART 0
#define BT_UART 1
#define BAUDRATE (9600U)

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
  ringbuffer_init(&(ctx[PC_UART].rx_buf), ctx[PC_UART].rx_mem, UART_BUFSIZE);
  uart_init(PC_UART, BAUDRATE, rx_cb, (void *)&(ctx[PC_UART].rx_buf));
	
  /* UART2 : BT terminal */
  ringbuffer_init(&(ctx[BT_UART].rx_buf), ctx[BT_UART].rx_mem, UART_BUFSIZE);
  uart_init(BT_UART, BAUDRATE, rx_cb, (void *)&(ctx[BT_UART].rx_buf));

  for (;;) {
	while (ctx[BT_UART].rx_buf.avail) {
	  // We have data coming from UART2 => simply print on PC terminal
	  int data = ringbuffer_get_one(&(ctx[BT_UART].rx_buf));
	  printf("%c", (char)data);
	}

	while (ctx[PC_UART].rx_buf.avail) {
	  // We have data printed on the PC terminal => forward on UART2
	  int data = ringbuffer_get_one(&(ctx[PC_UART].rx_buf));
	  uart_write(BT_UART, (uint8_t*)&data, 1);
	}
  }
	
  return 0;
}
