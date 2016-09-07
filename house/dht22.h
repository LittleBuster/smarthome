/* Smart Home: House server
 *
 * Copyright (C) 2016 Sergey Denisov, Adafruit Industries.
 * Written by Sergey Denisov aka LittleBuster (DenisovS21@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence 3
 * as published by the Free Software Foundation; either version 3
 * of the Licence, or (at your option) any later version.
 */

#ifndef __DHT22_H__
#define __DHT22_H__

#include <stdint.h>
#include <stdbool.h>
#include "utils.h"


enum dht_err_codes {
	DHT_TIMEOUT_ERR,
	DHT_READ_ERR,
	DHT_OK
};


struct dht22 {
	uint8_t data[5];
	uint8_t pin;
};


/**
 * Initialization gpio and dht structure
 * @dht: dht structure pointer
 * @pin: reading pin
 */
void dht22_init(struct dht22 *dht, uint8_t pin);

/**
* Reading temperature and humidity
* @dht: dht structure pointer
* @temp: temperature pointer
* @hum: humidity pointer
*
* returns DHT_OK: if complete reading data
* returns dht_error_codes: if fail reading data
*/
uint8_t dht22_read_data(struct dht22 *dht, float *temp, float *hum);


#endif