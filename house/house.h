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
#include <stdbool.h>


enum recv_cmd {
	TERMO_ON,
	TERMO_OFF,
	TERMO_GET_TEMP,
	TERMO_SET_TEMP,
	TERMO_GET_STATUS,
	GET_CAM_PHOTO,
	GET_METEO,
	PHOTO_OK,
	PHOTO_FAIL
};

struct command {
	uint8_t code;
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

struct termo_stat_answ {
	uint8_t status;
};

struct termo_temp_answ {
	float temp;
};


/*
 * Starting main smart home application
 */
bool house_server_start(void);


#endif
