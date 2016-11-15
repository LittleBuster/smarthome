/* SmartHome: house server library
 *
 * Copyright (C) 2016 Sergey Denisov.
 * Written by Sergey Denisov aka LittleBuster (DenisovS21@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public Licence
 * as published by the Free Software Foundation; either version 3
 * of the Licence, or (at your option) any later version.
 */

#ifndef __CLIENT_HOUSE_H__
#define __CLIENT_HOUSE_H__

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Getting photo from camera
 * @num: number of houseera
 *
 * returns true: if getting photo ok
 * returns false: if fail getting photo
 */
bool cam_get_photo(uint8_t num);

/**
 * Getting meteo data array
 * [0] street temp
 * [1] street hum
 * [2] room temp
 * [3] room hum
 * [4] 2nd floor temp
 * [5] 2nd floor hum
 * [6] veranda temp
 * [7] veranda hum
 *
 * returns true: if getting photo ok
 * returns false: if fail getting photo
 */
bool meteo_get_data(float *out_meteo);

/*
 * Start termo control timer
 */
bool termo_control_on(void);

/*
 * Stop termo control timer
 */
bool termo_control_off(void);

/**
 * Get termo max temperature
 * @temp: out max temp
 *
 * returns true: if ok
 * returns false: if fail getting
 */
bool termo_get_temp(float *temp);

/**
 * Seting termo max temperature
 * @temp: max temp
 *
 * returns true: if ok
 * returns false: if fail getting
 */
bool termo_set_temp(float temp);

/*
 * Get termo status
 *
 * returns true: if ok
 * returns false: if fail getting
 */
bool termo_get_status(uint8_t *status, uint8_t *heater_status);

/*
 * Security module ON
 *
 * returns true: if ok
 * returns false: if fail
 */
bool security_set_on(void);

/*
 * Security module OFF
 *
 * returns true: if ok
 * returns false: if fail
 */
bool security_set_off(void);

/**
 * Getting status of security
 * @status: status out pointer
 *
 * returns true: if ok
 * returns false: if fail
 */
bool security_get_status(uint8_t *status);


#ifdef __cplusplus
}
#endif

#endif
