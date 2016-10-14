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

#include "configs.h"
#include "log.h"
#include "tcpclient.h"
#include "path.h"
#include "termo.h"
#include "meteo.h"
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


static struct {
	pthread_t term_th;

	bool status;
	bool warm_status;

	struct termo_ext_cfg tec;
} termo = {
	.status = false,
	.warm_status = false
};


static void *termo_thread(void *data)
{
	const struct termo_cfg *tc = configs_get_termo();

	for (;;) {
		if (termo.status) {
			float cur_temp, cur_hum;			

			meteo_get_room_data(&cur_temp, &cur_hum);

			if (cur_temp <= (termo.tec.temp - 0.5)) {
				digitalWrite(tc->tpin, LOW);
				termo.warm_status = true;
			}

			if (cur_temp >= (termo.tec.temp + 0.5)) {
				digitalWrite(tc->tpin, HIGH);
				termo.warm_status = false;
			}
		} else {
			digitalWrite(tc->tpin, HIGH);
			termo.warm_status = false;
		}

		delay(1000);
	}
	return NULL;
}


bool termo_start(void)
{
	const struct termo_cfg *tc = configs_get_termo();

	pinMode(tc->tpin, OUTPUT);

	if (configs_termo_load(&termo.tec, PATH_TERMO_EXT) != CFG_OK)
		return false;

	pthread_create(&termo.term_th, NULL, &termo_thread, NULL);
	pthread_detach(termo.term_th);
	puts("Starting termo control...");
	return true;
}

void termo_control_on(void)
{
	termo.status = true;
}

void termo_control_off(void)
{
	termo.status = false;
}

void termo_get_status(uint8_t *status)
{
	*status = termo.status;
}

void termo_get_temp(float *temp)
{
	*temp = termo.tec.temp;	
}
