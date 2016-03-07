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

#define I2C_INTERFACE (0)           /* I2C interface number */
#define SENSOR_ADDR   (0x48 | 0x07) /* I2C temperature address on sensor */
#define INTERVAL      (5000000U)    /* set interval to 5 seconds */

static int read_temperature(void)
{
    uint16_t temperature;
    char buffer[2] = { 0 };
    /* read temperature register on I2C bus */
    if (i2c_read_bytes(I2C_INTERFACE, SENSOR_ADDR, buffer, 2) < 0) {
	printf("Cannont read at address %i on I2C interface %i",
	       SENSOR_ADDR, I2C_INTERFACE);
	return 0;
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

int main(void)
{
    puts("Read Atmel SAMR21 Io-Xplained-Pro temperature sensor on I2C bus\n");

    /* Initialise the I2C serial interface as master */
    int init = i2c_init_master(I2C_INTERFACE, I2C_SPEED_NORMAL);
    if (init == -1) {
        puts("Error: Init: Given device not available");
        return 1;
    }
    else if (init == -2) {
        puts("Error: Init: Unsupported speed value");
        return 1;
    }
    else {
        printf("I2C interface %i successfully initialized as master!\n", I2C_INTERFACE);
    }
    
    uint32_t last_wakeup = xtimer_now();
    for (;;) {
	printf("Temperature: %i°C\n", read_temperature());
	xtimer_usleep_until(&last_wakeup, INTERVAL);
    }

    return 0;
}
