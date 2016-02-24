/*
 * Copyright (C) 2016 Inria
 *
 * This file is subject to the terms and conditions of the GNU Lesser General
 * Public License v3. See the file LICENSE in the top level directory for more
 * details.
 */

/**
 * @{
 *
 * @file
 * @brief       Sample application using GPIO and UART interface
 *
 * @author      Alexandre Abadie <alexandre.abadie@inria.fr>
 *
 * @}
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "board.h"
#include "msg.h"
#include "thread.h"
#include "periph_conf.h"
#include "periph/uart.h"
#include "periph/i2c.h"

#define UART_INTERFACE 0
#define BAUDRATE (115200U)

#define I2C_INTERFACE 0
#define T_ADDR (0x48|0x07)

static kernel_pid_t idle_thread_pid;

int read_temperature(void)
{
    uint16_t temperature;
    char buffer[2];
    buffer[0] = 0;
    buffer[1] = 0;
    
    if (i2c_read_bytes(I2C_0, T_ADDR, buffer, 2) < 0) {
        puts("Error: no bytes were read\r\n");
        return -1;
    } else {
	uint16_t data = (buffer[0] << 8) | buffer[1];
	int8_t sign = 1;
	/* Check if negative and clear sign bit. */
	if (data & (1 << 15)) {
	    sign *= -1;
	    data &= ~(1 << 15);
	}
	/* Convert to temperature */
	data = (data >> 5);
	temperature = data * sign * 0.125;

        return (int)temperature;
    }
}

static void uart_cb(void *dev, char data)
{
    printf("UART Callback, read temperature\r\n");
    msg_t msg;
    msg.content.value = (uint32_t)(NULL);
    msg_send(&msg, idle_thread_pid);
}

int main(void)
{
    puts("Control I2C sensor from UART interface\r\n");
    
    /* Initialize UART interface */
    if (uart_init(UART_INTERFACE, BAUDRATE, uart_cb, (void *)NULL) < 0) {
	puts("Error while initializing UART interface\r\n");
	return 1;
    }
    puts("UART interface initialized successfully\r\n");

    if (i2c_init_master(I2C_0, I2C_SPEED_NORMAL) < 0) {
	puts("Error while initializing I2C interface\r\n");
	return 1;
    }
    puts("I2C interface initialized successfully\r\n");
    
    /* Get Idle thread pid */
    idle_thread_pid = thread_getpid();
    msg_t msg;
    for (;;) {
	msg_receive(&msg); /* This line blocks the loop until a message is 
			      received. */
	printf("Message received, temperature is %i°C\r\n", read_temperature());
    }
    
    return 0;
}
