/* Smart Home: House server
 *
 * Copyright (C) 2016 Sergey Denisov.
 * Written by Sergey Denisov aka LittleBuster (DenisovS21@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence 3
 * as published by the Free Software Foundation; either version 3
 * of the Licence, or (at your option) any later version.
 */

#include "meteo.h"
#include "gpio.h"
#include "dht22.h"
#include "log.h"
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <sys/time.h>


static struct {
	struct dht22 dht_street;
	struct dht22 dht_room;
	struct dht22 dht_veranda;
	struct dht22 dht_2nd;

	float street_temp;
	float street_hum;
	float room_temp;
	float room_hum;
	float second_temp;
	float second_hum;
	float veranda_temp;
	float veranda_hum;

	pthread_t sens_th;
} meteo = {
	.street_temp = 0.0f,
	.street_temp = 0.0f,
	.room_temp = 0.0f,
	.room_temp = 0.0f,
	.second_temp = 0.0f,
	.second_temp = 0.0f,
	.veranda_temp = 0.0f,
	.veranda_temp = 0.0f
};


static void sensor_read_data(struct dht22 *restrict dht, float *temp, float *hum)
{
    uint8_t last_error = 0;

	for (uint8_t i = 0; i < 20; i++) {
    	last_error = dht22_read_data(dht, temp, hum);
    	if (last_error == DHT_OK)
    		return;
    	
    	struct timeval tv = {1, 0};
		if (select(0, NULL, NULL, NULL, &tv) == -1)
    		if (EINTR == errno)
    			continue;
    }
    if (last_error != DHT_OK)
    	log_local("Fail reading street sensor.", LOG_WARNING);
}

static void* timer_thread(void *data)
{
	for (;;) {
    	sensor_read_data(&meteo.dht_street, &meteo.street_temp, &meteo.street_hum);
    	sensor_read_data(&meteo.dht_room, &meteo.room_temp, &meteo.room_hum);
    	sensor_read_data(&meteo.dht_veranda, &meteo.veranda_temp, &meteo.veranda_hum);
    	sensor_read_data(&meteo.dht_2nd, &meteo.second_temp, &meteo.second_hum);

    	struct timeval tv = {2, 0};
		if (select(0, NULL, NULL, NULL, &tv) == -1)
    		if (EINTR == errno)
    			continue;
    }
	return NULL;
}


bool meteo_sensors_init(void)
{
	if (!gpio_init())
		return false;
	dht22_init(&meteo.dht_street, 2);
    dht22_init(&meteo.dht_room, 3);
    dht22_init(&meteo.dht_veranda, 4);
    dht22_init(&meteo.dht_2nd, 17);
	return true;
}

void meteo_sensors_start_timer(void)
{
	pthread_create(&meteo.sens_th, NULL, &timer_thread, NULL);
	pthread_detach(meteo.sens_th); 
	puts("Starting meteo server...");
}

void meteo_get_street_data(float *temp, float *hum)
{
	*temp = meteo.street_temp;
	*hum = meteo.street_hum;
}

void meteo_get_room_data(float *temp, float *hum)
{
	*temp = meteo.room_temp;
	*hum = meteo.room_hum;
}

void meteo_get_2nd_data(float *temp, float *hum)
{
	*temp = meteo.second_temp;
	*hum = meteo.second_hum;
}

void meteo_get_veranda_data(float *temp, float *hum)
{
	*temp = meteo.veranda_temp;
	*hum = meteo.veranda_hum;
}
