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
 * @file
 * @brief       Serial to serial communication
 *
 * @}
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "board.h"
#include "msg.h"
#include "thread.h"
#include "periph/uart.h"

#define PC_UART 0
#define BT_UART 1
#define BAUDRATE (9600U)

static kernel_pid_t idle_thread_pid;

static void rx_cb(void *uart, char c)
{
  /* A character was received on an UART interface and triggered
  this callback via an interruption, we forward it via a message
  to the idle thread. */
  msg_t msg;
  msg.type = (int)uart;
  msg.content.value = (uint32_t)c;
  msg_send(&msg, idle_thread_pid);
}

int main(void)
{
  printf("Connection between 2 UART interfaces\n");
  printf("====================================\n");
  
  /* Initialize serial devices */
  /* UART0 : PC terminal */
  uart_init(PC_UART, BAUDRATE, rx_cb, (void *)PC_UART);
	
  /* UART2 : BT terminal */
  uart_init(BT_UART, BAUDRATE, rx_cb, (void *)BT_UART);

  /* Get Idle thread pid */
  idle_thread_pid = thread_getpid();
  
  /* Local variable containing the message exchanged between 
  the 2 UART interfaces */
  msg_t msg;
  for (;;) {
	/* Next line blocks the loop until a message is received on the idle thread */
	msg_receive(&msg);
	int uart = msg.type; /* 'type' contains the UART interface number */
	switch (uart) {
	case PC_UART:
	  /* Forward message to BT UART */
	  uart_write(BT_UART, (uint8_t*)&(msg.content.value), 1);
	  break;
	case BT_UART:
	  /* Print message on PC terminal */
	  printf("%c", (char)msg.content.value);
	  break;
	default:
	  /* Should not happen */
	  printf("Unknown UART interface %i", uart);
	  break;
	}
  }

  return 0;
}
