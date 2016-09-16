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

		}

		struct timeval tv = {1, 0};
		if (select(0, NULL, NULL, NULL, &tv) == -1)
    		if (EINTR == errno)
    			continue;
	}
	return NULL;
}


void termo_start(void)
{
	pthread_create(&termo.term_th, NULL, &termo_thread, NULL);
	pthread_detach(termo.term_th);
	puts("Starting termo control...");
}

void termo_auto_on(void)
{
	termo.status = true;
}

void termo_auto_off(void)
{
	termo.status = false;
}