/* SmartHome: HomeLight server library
 *
 * Copyright (C) 2016 Sergey Denisov.
 * Written by Sergey Denisov aka LittleBuster (DenisovS21@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public Licence
 * as published by the Free Software Foundation; either version 3
 * of the Licence, or (at your option) any later version.
 */

#ifndef __HLIGHT_H__
#define __HLIGHT_H__

#include <stdbool.h>
#include <stdint.h>

#define HL_LAMPS 2

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
bool hlight_set_log(const char *filename);


enum hl_cfg_err {
	HL_CFG_FILE_NOT_FOUND,
	HL_CFG_PARSE_ERR,
	HL_CFG_OK
};

/**
 * Loading configs ot street light module
 * @filename: path to configs file
 *
 * returns CFG_OK: if reading ok
 * returns cfg_err codes: if fail reading
 */
uint8_t hlight_load_configs(const char *filename);

/**
 * Switch on lamp
 * @lamp: number of lamps
 *
 * returns false: if fail sending switch on cmd
 * returns true: if ok
 */
bool hlight_switch_on(unsigned lamp);

/**
 * Switch off lamp
 * @lamp: number of lamp: 1-8
 *
 * returns false: if fail sending switch off cmd
 * returns true: if ok
 */
bool hlight_switch_off(unsigned lamp);


struct hl_status_data {
	unsigned lamps[HL_LAMPS]; 
};

/**
 * Get lamps status array
 * @status: output array
 *
 * returns false: if fail receiving status from device
 * returns true: if status received
 */
bool hlight_get_status(struct hl_status_data *restrict status);


#ifdef __cplusplus
}
#endif

#endif