/* SmartHome: Home Light application
 *
 * Copyright (C) 2016 Sergey Denisov.
 * Written by Sergey Denisov aka LittleBuster (DenisovS21@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version 3
 * of the Licence, or (at your option) any later version.
 */

#include "hlight.h"
#include "tcpserver.h"
#include "log.h"
#include "configs.h"
#include "pthread.h"
#include "utils.h"
#include <wiringPi.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>


enum {
	SWITCH_ON,
	SWITCH_OFF,
	GET_STATUS
};

struct command {
	uint8_t code;
	unsigned lamp;
};

struct switch_answ {
	uint8_t code;
};

struct status_data {
	unsigned lamps[LAMPS]; 
};

static struct {
	unsigned lamps[LAMPS];
	pthread_mutex_t mutex;
	struct tcp_server server;
} hlight;


static void new_session(struct tcp_client *client, void *data)
{
	struct command cmd;
	const struct lamp_cfg *lc = configs_get_lamps();

	if(!tcp_client_recv(client, (void *)&cmd, sizeof(struct command))) {
		pthread_mutex_lock(&hlight.mutex);
		log_local("Fail reading client command", LOG_ERROR);
		pthread_mutex_unlock(&hlight.mutex);
		return;
	}

	switch (cmd.code) {
		case SWITCH_ON: {
			pthread_mutex_lock(&hlight.mutex);
			printf("Switching ON #%u\n", cmd.lamp);
			digitalWrite(lc->lamps[cmd.lamp], LOW);
			hlight.lamps[cmd.lamp] = 1;
			pthread_mutex_unlock(&hlight.mutex);
			break;
		}
		case SWITCH_OFF: {
			pthread_mutex_lock(&hlight.mutex);
			printf("Switching OFF #%u\n", cmd.lamp);
			digitalWrite(lc->lamps[cmd.lamp], HIGH);
			hlight.lamps[cmd.lamp] = 0;
			pthread_mutex_unlock(&hlight.mutex);
			break;
		}
		case GET_STATUS: {
			struct status_data sdata;
			
			pthread_mutex_lock(&hlight.mutex);
			printf("New status getting:");
			for (uint8_t i = 0; i < LAMPS; i++) {
				sdata.lamps[i] = hlight.lamps[i];
				printf("L%hhu=%u ", i, hlight.lamps[i]);
			}
			printf("\n");				
			pthread_mutex_unlock(&hlight.mutex);

			if (!tcp_client_send(client, (const void *)&sdata, sizeof(struct status_data))) {
				pthread_mutex_lock(&hlight.mutex);
				log_local("Fail receiving status data", LOG_ERROR);
				pthread_mutex_unlock(&hlight.mutex);
				return;
			}
			break;
		}
	}
}

static void accept_error(void *data)
{
	pthread_mutex_lock(&hlight.mutex);
	log_local("Fail accepting new client", LOG_ERROR);
	pthread_mutex_lock(&hlight.mutex);
}

bool hlight_start(void)
{
	const struct server_cfg *sc = configs_get_server();
	const struct lamp_cfg *lc = configs_get_lamps();

	puts("Starting home light server...");
	pthread_mutex_init(&hlight.mutex, NULL);
	for (size_t i = 0; i < LAMPS; i++) {
		hlight.lamps[i] = 0;
		pinMode(lc->lamps[i], OUTPUT);
		digitalWrite(lc->lamps[i], HIGH);
	}

	tcp_server_set_newsession_cb(&hlight.server, new_session, NULL);
	tcp_server_set_accepterr_cb(&hlight.server, accept_error, NULL);
	if (!tcp_server_bind(&hlight.server, sc->port, sc->max_users)) {
		log_local("Fail starting HomeLight server", LOG_ERROR);
		return false;
	}
	pthread_mutex_destroy(&hlight.mutex);
	return true;
}