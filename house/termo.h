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

#ifndef __TERMO_H__
#define __TERMO_H__

#include <pthread.h>
#include <stdbool.h>

/*
 * Starting termo control module
 */
bool termo_start(void);

/*
 * Termo control on
 */
void termo_control_on(void);

/*
 * Termo control off
 */
void termo_control_off(void);

/*
 * Getting control status (on/off)
 */
void termo_get_status(uint8_t *status);

/*
 * Getting max temp
 */
void termo_get_temp(float *temp);


#endif
