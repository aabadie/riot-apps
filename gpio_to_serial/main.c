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
#include "xtimer.h"
#include "periph/gpio.h"
#include "periph/uart.h"

#define UART_INTERFACE 0
#define BAUDRATE (115200U)


static uint32_t last_press;
static kernel_pid_t main_thread_pid;

static void uart_cb(void *dev, char data)
{
    printf("\rUART Callback\n");
    gpio_toggle(LED_GPIO);
    msg_t msg;
    msg.content.value = (uint32_t)(NULL);
    msg_send(&msg, main_thread_pid);
}

static void gpio_cb(void *pin)
{
    /* prevent bounce effect with on board button */
    if (last_press + 100 < xtimer_now()) {
	printf("GPIO Callback\n");
	gpio_toggle(LED_GPIO);
	msg_t msg;
	msg.content.value = (uint32_t)(NULL);
	msg_send(&msg, main_thread_pid);
	last_press = xtimer_now();
    }
}

int main(void)
{
    printf("GPIO to UART sample application\r\n");

    /* Initialize UART interface */
    if (uart_init(UART_INTERFACE, BAUDRATE, uart_cb, (void *)NULL) < 0) {
	printf("Error while initializing UART interface\n");
	return 1;
    }
    printf("UART interface initialized successfuly\n");
    
    if (gpio_init(LED_GPIO, GPIO_DIR_OUT, GPIO_NOPULL) < 0) {
        puts("Error while initializing LED GPIO as output\n");
        return 1;
    }
    printf("LED GPIO initialized successfully as output\n");
    /* Shutdown on board LED */
    gpio_set(LED_GPIO);
    
    if (gpio_init_int(BUTTON_GPIO, GPIO_PULLUP, GPIO_RISING, gpio_cb,
		      (void *)BUTTON_GPIO) < 0) {
        puts("Error while initializing BUTTON GPIO as external interrupt\n");
        return 1;
    }
    printf("BUTTON GPIO initialized successfuly\n");

    last_press = xtimer_now();
    
    /* Get main thread pid */
    main_thread_pid = thread_getpid();
    msg_t msg;
    for (;;) {
	msg_receive(&msg); /* This line blocks the loop until a message is 
			      received. */
	printf("Message received, LED is %s\n", !gpio_read(LED_GPIO)? "ON" : "OFF");
    }
    
    return 0;
}
