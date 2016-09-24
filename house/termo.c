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
} termo = {
	.status = false
};


static void *termo_thread(void *data)
{
	struct termo_cfg *tc = configs_get_termo();

	for (;;) {
		if (termo.status) {
			float max_temp;
			float cur_temp, cur_hum;
			struct termo_cfg *tc = configs_get_termo();

			meteo_get_room_data(&cur_temp, &cur_hum);
			max_temp = configs_termo_get_temp();

			if (cur_temp < max_temp)
				digitalWrite(tc->tpin, LOW);
			else
				digitalWrite(tc->tpin, HIGH);
		} else
			digitalWrite(tc->tpin, HIGH);

		delay(1000);
	}
	return NULL;
}


void termo_start(void)
{
	struct termo_cfg *tc = configs_get_termo();

	pinMode(tc->tpin, OUTPUT);
	pthread_create(&termo.term_th, NULL, &termo_thread, NULL);
	pthread_detach(termo.term_th);
	puts("Starting termo control...");
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
	*temp = configs_termo_get_temp();	
	printf("%f", configs_termo_get_temp());
}
