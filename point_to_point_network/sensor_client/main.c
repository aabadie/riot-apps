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
 * @brief       Sample application sending to an udp server the temperature read
 * on an I2C sensor.
 *
 * @author      Alexandre Abadie <alexandre.abadie@inria.fr>
 *
 * @}
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "xtimer.h"
#include "periph_conf.h"
#include "periph/i2c.h"
#include "net/af.h"
#include "net/gnrc/ipv6.h"
#include "net/conn/udp.h"

/* I2C interface number */
#define I2C_INTERFACE 0

/* I2C temperature address on sensor */
#define TEMP_ADDR  (0x48 | 0x07)

/* set interval to 5 seconds */
#define INTERVAL (5000000U)

#define MAX_MESSAGE_LENGTH 24

const char * SERVER_IP = "fe80::5847:3c7c:4950:129a";
const uint16_t SERVER_PORT = 8000;

static int read_temperature(void)
{
    uint16_t temperature;
    char buffer[2] = { 0 };

    /* temperature is stored on 2 bytes */
    i2c_read_bytes(I2C_INTERFACE, TEMP_ADDR, buffer, 2);

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

static int send_data_to_server(char *data)
{ 
    ipv6_addr_t src = IPV6_ADDR_UNSPECIFIED, dst;
    
    /* format destination address from string */
    ipv6_addr_from_str(&dst, SERVER_IP);
    
    /* send data to server */
    conn_udp_sendto(data, strlen(data), &src, sizeof(src), (struct sockaddr *)&dst,
		    sizeof(dst), AF_INET6, (uint16_t)0, SERVER_PORT);
    
    return 0;
}

int main(void)
{
    puts("Read temperature sensor on Atmel Samr21 Xplained extension\n");

    i2c_init_master(I2C_INTERFACE, I2C_SPEED_NORMAL);
    
    uint32_t last_wakeup = xtimer_now();
    char message[MAX_MESSAGE_LENGTH];
    while(1) {
        xtimer_usleep_until(&last_wakeup, INTERVAL);
	snprintf(message, sizeof(message), "Temperature: %i°C", read_temperature());
	send_data_to_server(message);
	last_wakeup = xtimer_now();
    }

    return 0;
}
