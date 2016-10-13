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

#ifndef __METEO_H__
#define __METEO_H__

#include <stdbool.h>
#include <pthread.h>


/*
 * Init gpio ports for temp sensors
 */
bool meteo_sensors_init(void);

/*
 * Start meteo timer
 */
void meteo_sensors_start_timer(pthread_mutex_t *mutex);

/**
 * Get street meteo data
 * @temp: temperature out pointer
 * @hum: humidity out pointer
 */
void meteo_get_street_data(float *temp, float *hum);

/**
 * Get room meteo data
 * @temp: temperature out pointer
 * @hum: humidity out pointer
 */
void meteo_get_room_data(float *temp, float *hum);

/**
 * Get 2nd floor meteo data
 * @temp: temperature out pointer
 * @hum: humidity out pointer
 */
void meteo_get_2nd_data(float *temp, float *hum);

/**
 * Get veranda meteo data
 * @temp: temperature out pointer
 * @hum: humidity out pointer
 */
void meteo_get_veranda_data(float *temp, float *hum);


#endif
