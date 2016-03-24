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

#define UART_INTERFACE UART_DEV(0) /* read and print on UART 0 */
#define BAUDRATE       (115200U)

#define I2C_INTERFACE  I2C_DEV(0)  /* I2C interface number */
#define SENSOR_ADDR    (0x48|0x07) /* I2C temperature address on sensor */

static kernel_pid_t main_thread_pid;

int read_temperature(void)
{
    uint16_t temperature;
    char buffer[2] = { 0 };
    
    if (i2c_read_bytes(I2C_INTERFACE, SENSOR_ADDR, buffer, 2) < 0) {
	printf("Error: cannot read at address %i on I2C interface %i\n",
	       SENSOR_ADDR, I2C_INTERFACE);
        return -1;
    }

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

static void uart_cb(void *dev, uint8_t data)
{
    printf("UART Callback, read temperature\n");
    msg_t msg;
    msg.content.value = (uint32_t)(NULL);
    msg_send(&msg, main_thread_pid);
}

int main(void)
{
    puts("Control I2C sensor from UART interface\n");
    
    /* Initialize UART interface */
    if (uart_init(UART_INTERFACE, BAUDRATE, uart_cb, (void *)NULL) < 0) {
	puts("Error: failed to initialize UART interface\n");
	return 1;
    }
    puts("UART interface initialized successfully\n");

    if (i2c_init_master(I2C_INTERFACE, I2C_SPEED_NORMAL) < 0) {
	puts("Error: failed to initialize I2C interface\n");
	return 1;
    }
    puts("I2C interface initialized successfully\n");
    
    /* Get main thread pid */
    main_thread_pid = thread_getpid();
    msg_t msg;
    for (;;) {
	msg_receive(&msg); /* This line blocks the loop until a message is 
			      received. */
	printf("Message received, temperature is %i°C\n", read_temperature());
    }
    
    return 0;
}
