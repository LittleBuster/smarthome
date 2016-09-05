/* SmartHome: StreetLight server library
 *
 * Copyright (C) 2016 Sergey Denisov.
 * Written by Sergey Denisov aka LittleBuster (DenisovS21@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public Licence
 * as published by the Free Software Foundation; either version 3
 * of the Licence, or (at your option) any later version.
 */

#ifndef __STLIGHT_H__
#define __STLIGHT_H__

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Setting log path for street light module
 * @filename: path to log file
 *
 * returns false: if path is to long
 * returns true: if ok
 */
bool stlight_set_log(const char *filename);


enum st_cfg_err {
	ST_CFG_FILE_NOT_FOUND,
	ST_CFG_PARSE_ERR,
	ST_CFG_OK
};

/**
 * Loading configs ot street light module
 * @filename: path to configs file
 *
 * returns CFG_OK: if reading ok
 * returns cfg_err codes: if fail reading
 */
uint8_t stlight_load_configs(const char *filename);

/**
 * Switch on lamp
 * @lamp: number of lamp: 1-8
 *
 * returns false: if fail sending switch on cmd
 * returns true: if ok
 */
bool stlight_switch_on(unsigned lamp);

/**
 * Switch off lamp
 * @lamp: number of lamp: 1-8
 *
 * returns false: if fail sending switch off cmd
 * returns true: if ok
 */
bool stlight_switch_off(unsigned lamp);


struct status_data {
	unsigned lamps[8]; 
};

/**
 * Get lamps status array
 * @status: output array
 *
 * returns false: if fail receiving status from device
 * returns true: if status received
 */
bool stlight_get_status(struct status_data *restrict status);

/**
 * Set lamps status array
 * @status: input array
 *
 * returns false: if fail sending status to device
 * returns true: if status sended
 */
bool stlight_set_status(const struct status_data *restrict status);


#ifdef __cplusplus
}
#endif

#endif