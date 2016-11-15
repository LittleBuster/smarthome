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

#ifdef __cplusplus
extern "C" {
#endif

#define HL_LAMPS 2

/**
 * Switch on lamp
 * @lamp: number of lamps
 *
 * returns false: if fail sending switch on cmd
 * returns true: if ok
 */
bool hlight_switch_on(uint8_t lamp);

/**
 * Switch off lamp
 * @lamp: number of lamp: 1-8
 *
 * returns false: if fail sending switch off cmd
 * returns true: if ok
 */
bool hlight_switch_off(uint8_t lamp);


struct hl_status_data {
	uint8_t lamps[HL_LAMPS]; 
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