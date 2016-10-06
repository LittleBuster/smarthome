/* SmartHome: Street Light application
 *
 * Copyright (C) 2016 Sergey Denisov.
 * Written by Sergey Denisov aka LittleBuster (DenisovS21@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version 3
 * of the Licence, or (at your option) any later version.
 */

#ifndef __MOVE_TIME_H__
#define __MOVE_TIME_H__

#include <stdbool.h>
#include <stdint.h>

struct time_on {
	uint8_t on_lh1;
	uint8_t on_lh2;
};

struct time_off {
	uint8_t off_lh1;
	uint8_t off_lh2;
};


/**
 * Loading move time configs from file
 * @filename: meteo time file
 *
 * returns true: if loading ok
 * returns false: if loading fail
 */
bool move_time_load(const char *filename);

/*
 * Moving time on
 */
struct time_on *move_time_get_on(void);

/*
 * Moving time off
 */
struct time_off *move_time_get_off(void);

/*
 * Set moving time on
 */
bool move_time_set_on(const struct time_on *ton);

/*
 * Set moving time off
 */
bool move_time_set_off(const struct time_off *toff);


#endif