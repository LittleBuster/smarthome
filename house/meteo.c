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
#include "configs.h"
#include "log.h"
#include "path.h"
#include "database.h"
#include <stdio.h>
#include <stdbool.h>
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
	pthread_mutex_t *mutex;

	uint8_t tm;
} meteo = {
	.tm = 0,
	.street_temp = -2000.0f,
	.street_hum = -2000.0f,
	.room_temp = -2000.0f,
	.room_hum = -2000.0f,
	.second_temp = -2000.0f,
	.second_hum = -2000.0f,
	.veranda_temp = -2000.0f,
	.veranda_hum = -2000.0f
};


static bool sensor_read_data(struct dht22 *dht, float *temp, float *hum)
{
	uint8_t ret = 0;

	for (uint8_t i = 0; i < 20; i++) {
    		ret = dht22_read_data(dht, temp, hum);

    		if (ret == DHT_OK)
    			return true;

		struct timeval tv = {1, 0};
		if (select(0, NULL, NULL, NULL, &tv) == -1)
    			if (EINTR == errno)
    				continue;
	}
	*temp = -2000.0f;
	*hum = -2000.0f;
	return false;
}

static void* timer_thread(void *data)
{
	for (;;) {
		if (!sensor_read_data(&meteo.dht_street, &meteo.street_temp, &meteo.street_hum)) {
			pthread_mutex_lock(meteo.mutex);
			log_local("Fail reading street sensor", LOG_WARNING);
			pthread_mutex_unlock(meteo.mutex);
		}
    		if (!sensor_read_data(&meteo.dht_room, &meteo.room_temp, &meteo.room_hum)) {
			pthread_mutex_lock(meteo.mutex);
			log_local("Fail reading room sensor", LOG_WARNING);
			pthread_mutex_unlock(meteo.mutex);
		}
	    	if (!sensor_read_data(&meteo.dht_veranda, &meteo.veranda_temp, &meteo.veranda_hum)) {
			pthread_mutex_lock(meteo.mutex);
			log_local("Fail reading veranda sensor", LOG_WARNING);
			pthread_mutex_unlock(meteo.mutex);
		}
    		if (!sensor_read_data(&meteo.dht_2nd, &meteo.second_temp, &meteo.second_hum)) {
			pthread_mutex_lock(meteo.mutex);
			log_local("Fail reading 2nd sensor", LOG_WARNING);
			pthread_mutex_unlock(meteo.mutex);
		}

    	struct timeval tv = {10, 0};
	if (select(0, NULL, NULL, NULL, &tv) == -1)
	   	if (EINTR == errno)
    			continue;
    	
    	meteo.tm++;
    	if (meteo.tm == 60) {
		struct database meteo_db;
    		meteo.tm = 0;

		if (!database_load(&meteo_db, PATH_METEO)) {
			log_local("Fail loading Meteo database.", LOG_WARNING);
			continue;
		}
    		if (!database_add_meteo(&meteo_db, meteo.street_temp, meteo.street_hum, DB_STREET))
    			log_local("Fail adding street data to DB.", LOG_WARNING);
    		if (!database_add_meteo(&meteo_db, meteo.room_temp, meteo.room_hum, DB_MAIN_ROOM))
    			log_local("Fail adding room data to DB.", LOG_WARNING);
		database_close(&meteo_db);
	    }
 	}
	return NULL;
}


bool meteo_sensors_init(void)
{
	const struct meteo_cfg *mc = configs_get_meteo();

	if (!gpio_init())
		return false;
	dht22_init(&meteo.dht_street, mc->street);
	dht22_init(&meteo.dht_room, mc->room);
   	dht22_init(&meteo.dht_veranda, mc->veranda);
    	dht22_init(&meteo.dht_2nd, mc->second);
	return true;
}

void meteo_sensors_start_timer(pthread_mutex_t *mutex)
{
	meteo.mutex = mutex;
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
