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
 * @brief       Sample application using the low-level I2C peripheral driver
 * with SAMR21-XPRO xplained pro extension.
 *
 * @author      Alexandre Abadie <alexandre.abadie@inria.fr>
 *
 * @}
 */

#include <stdio.h>
#include <stdlib.h>

#include "xtimer.h"
#include "periph_conf.h"
#include "periph/i2c.h"

/* I2C interface number */
#define I2C_INTERFACE 0

/* I2C temperature address on sensor */
#define TEMP_ADDR  (0x48 | 0x07)

/* set interval to 1.5 second */
#define INTERVAL (1500000U)

static int init_i2c(void)
{
    switch(i2c_init_master(I2C_INTERFACE, I2C_SPEED_NORMAL)) {
    case -1:
	puts("Error: Init: Given device not available\n");
        return 1;
    case -2:
	puts("Error: Init: Unsupported speed value\n");
	return 1;
    default:
	printf("I2C_0 successfully initialized as master!\n");

    return 0;
    }
}

static int read_temperature(void)
{
    uint16_t temperature;
    char buffer[2];
    buffer[0] = 0;
    buffer[1] = 0;
    
    if (i2c_read_bytes(I2C_INTERFACE, TEMP_ADDR, buffer, 2) < 0) {
        puts("Error: no bytes read\n");
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

	/* TODO convert to float */
        return (int)temperature;
    }
}

int main(void)
{
    puts("Read temperature sensor on Atmel Samr21 Xplained extension\n");

    if (init_i2c() > 0) {
	return 1;
    }
    
    uint32_t last_wakeup = xtimer_now();
    while(1) {
        xtimer_usleep_until(&last_wakeup, INTERVAL);
	printf("Temperature: %i°C\n", read_temperature());
	last_wakeup = xtimer_now();
    }

    return 0;
}
