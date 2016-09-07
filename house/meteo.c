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


static void* timer_thread(void *data)
{
	struct timeval tv = {2, 0};
	for (;;) {
		if (select(0, NULL, NULL, NULL, &tv) == -1)
    		if (EINTR == errno)
    			continue;
    }
	return NULL;
}


bool meteo_sensors_init(void)
{
	gpio_init();
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
}

void meteo_get_street_data(float *temp, float *hum)
{

}

void meteo_get_room_data(float *temp, float *hum)
{

}

void meteo_get_2nd_data(float *temp, float *hum)
{

}

void meteo_get_veranda_data(float *temp, float *hum)
{

}
