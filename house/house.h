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

#ifndef __HOUSE_H__
#define __HOUSE_H__

#include <stdint.h>


struct command {
	uint8_t cmd;
};

struct meteo_answ {
	float street_temp;
	float street_hum;
	float room_temp;
	float room_hum;
	float second_temp;
	float second_hum;
	float veranda_temp;
	float veranda_hum;
};


/*
 * Starting main smart home application
 */
bool house_server_start(void);


#endif
