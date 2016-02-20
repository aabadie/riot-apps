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

#include "shell.h"
#include "msg.h"
#include "thread.h"
#include "periph/gpio.h"
#include "periph/uart.h"

#define GPIO_BUTTON_PORT 10
#define GPIO_BUTTON_PIN 10

#define GPIO_LED_PORT 11
#define GPIO_LED_PIN 11

#define UART_INTERFACE 0
#define BAUDRATE (9600U)

static kernel_pid_t idle_thread_pid;
static bool status = 0;

static void gpio_cb(void *pin)
{
    gpio_toggle(GPIO_PIN(GPIO_LED_PORT, GPIO_LED_PIN));
    printf("INT: external interrupt from pin %i\n", (int)pin);
    msg_t msg;
    msg.content.value = (uint32_t)(gpio_read(GPIO_PIN(GPIO_LED_PORT, GPIO_LED_PIN)));
    msg_send(&msg, idle_thread_pid);
}

int main(void)
{
    puts("GPIO to UART sample application\n");

    if (gpio_init(GPIO_PIN(GPIO_LED_PORT, GPIO_LED_PIN),
		  GPIO_DIR_OUT, GPIO_PULLDOWN) < 0) {
        printf("Error while initializing LED on PORT_%i.%i as output\n",
	       GPIO_LED_PORT, GPIO_LED_PIN);
        return 1;
    }
    printf("LED on PORT_%i.%i initialized successfully as output\n",
	   GPIO_LED_PORT, GPIO_LED_PIN);
    
    if (gpio_init_int(GPIO_PIN(GPIO_BUTTON_PORT, GPIO_BUTTON_PIN),
		      GPIO_PULLDOWN, GPIO_RISING, gpio_cb,
		      (void *)GPIO_BUTTON_PIN) < 0) {
        printf("Error while initializing  PORT_%i.%02i as external interrupt\n",
               GPIO_BUTTON_PORT, GPIO_BUTTON_PIN);
        return 1;
    }
    printf("PORT_%i.%02i initialized successful as external interrupt\n",
           GPIO_BUTTON_PORT, GPIO_BUTTON_PIN);

    /* Initialize UART interface */
    uart_init(UART_INTERFACE, BAUDRATE, NULL, (void *)NULL);
    
    /* Get Idle thread pid */
    idle_thread_pid = thread_getpid();

    msg_t msg;
    for (;;) {
	msg_receive(&msg); /* This line blocks the loop until a message is received. */
	bool status = (bool)msg.content.value;
	uart_write(UART_INTERFACE,
		   (uint8_t*)strcat("Button pressed, LED is now ",
				    status? "ON" : "OFF"), 1);
    }
    
    return 0;
}
