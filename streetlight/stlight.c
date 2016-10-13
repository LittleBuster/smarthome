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

#include "stlight.h"
#include "tcpserver.h"
#include "log.h"
#include "configs.h"
#include "pthread.h"
#include "utils.h"
#include "movetime.h"
#include <wiringPi.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>


enum {
	SWITCH_ON,
	SWITCH_OFF,
	GET_STATUS,
	SET_STATUS,
	GET_MOVE_TIME,
	SET_MOVE_TIME
};

struct command {
	uint8_t code;
	unsigned lamp;
};

struct switch_answ {
	uint8_t code;
};

struct status_data {
	unsigned lamps[8]; 
};

struct time_move_data {
	uint8_t on_lh1;
	uint8_t on_lh2;
	uint8_t off_lh1;
	uint8_t off_lh2;
};


static struct {
	bool hand_lamp1;
	bool hand_lamp2;

	unsigned time_mov1;
	unsigned time_mov2;
	unsigned lamps[8];

	pthread_mutex_t mutex;

	struct tcp_server server;
	uint8_t tb_on;
} stlight = {
	.time_mov1 = 0,
	.time_mov2 = 0,
	.hand_lamp1 = false,
	.hand_lamp2 = false,
	.tb_on = false
};


static void new_session(struct tcp_client *client, void *data)
{
	struct command cmd;
	struct lamp_cfg *lc = configs_get_lamps();
	struct move_detect *md = configs_get_md();

	if(!tcp_client_recv(client, &cmd, sizeof(struct command))) {
		pthread_mutex_lock(&stlight.mutex);
		log_local("Fail reading client command", LOG_ERROR);
		pthread_mutex_unlock(&stlight.mutex);
		return;
	}

	switch (cmd.code) {
		case SWITCH_ON: {
			pthread_mutex_lock(&stlight.mutex);
			printf("Switching ON #%d\n", (int)cmd.lamp);
			digitalWrite(lc->lamps[cmd.lamp], LOW);
			stlight.lamps[cmd.lamp] = 1;

			/*
			 * If not move, hand switch on
			 */
			if (cmd.lamp == md->lamp1)
				stlight.hand_lamp1 = true;
			
			if (cmd.lamp == md->lamp2)
				stlight.hand_lamp2 = true;

			pthread_mutex_unlock(&stlight.mutex);
			break;
		}
		case SWITCH_OFF: {
			pthread_mutex_lock(&stlight.mutex);
			printf("Switching OFF #%d\n", (int)cmd.lamp);
			digitalWrite(lc->lamps[cmd.lamp], HIGH);
			stlight.lamps[cmd.lamp] = 0;

			/*
			 * If not move, hand switch off
			 */
			if (cmd.lamp == md->lamp1) {
				stlight.hand_lamp1 = false;
				stlight.time_mov1 = 0;
			}
			
			if (cmd.lamp == md->lamp2) {
				stlight.hand_lamp2 = false;
				stlight.time_mov1 = 0;
			}

			pthread_mutex_unlock(&stlight.mutex);
			break;
		}
		case SET_STATUS: {
			struct status_data sdata;

			if (!tcp_client_recv(client, &sdata, sizeof(struct status_data))) {
				pthread_mutex_lock(&stlight.mutex);
				log_local("Fail receiving status data", LOG_ERROR);
				pthread_mutex_unlock(&stlight.mutex);
				return;
			}
			pthread_mutex_lock(&stlight.mutex);
			printf("New status getting:");
			for (size_t i = 0; i < 8; i++) {
				stlight.lamps[i] = sdata.lamps[i];
				printf("L%u=%u ", i, sdata.lamps[i]);
			}
			printf("\n");
			pthread_mutex_unlock(&stlight.mutex);
			break;
		}
		case GET_STATUS: {
			struct status_data sdata;
			
			pthread_mutex_lock(&stlight.mutex);
			printf("New status getting:");
			for (size_t i = 0; i < 8; i++) {
				sdata.lamps[i] = stlight.lamps[i];
				printf("L%u=%u ", i, stlight.lamps[i]);
			}
			printf("\n");				
			pthread_mutex_unlock(&stlight.mutex);

			if (!tcp_client_send(client, &sdata, sizeof(struct status_data))) {
				pthread_mutex_lock(&stlight.mutex);
				log_local("Fail receiving status data", LOG_ERROR);
				pthread_mutex_unlock(&stlight.mutex);
				return;
			}
			break;
		}
		case GET_MOVE_TIME: {
			struct time_move_data tm_answ;

			pthread_mutex_lock(&stlight.mutex);
			struct time_on *ton = move_time_get_on();
			struct time_off *toff = move_time_get_off();
			pthread_mutex_unlock(&stlight.mutex);

			tm_answ.on_lh1 = ton->on_lh1;
			tm_answ.on_lh2 = ton->on_lh2;
			tm_answ.off_lh1 = toff->off_lh1;
			tm_answ.off_lh2 = toff->off_lh2;

			if (!tcp_client_send(client, &tm_answ, sizeof(struct time_move_data))) {
				pthread_mutex_lock(&stlight.mutex);
				log_local("Fail receiving time move data", LOG_ERROR);
				pthread_mutex_unlock(&stlight.mutex);
				return;
			}
			break;
		}
		case SET_MOVE_TIME: {
			struct time_move_data tmd;
			struct time_on ton;
			struct time_off toff;

			if (!tcp_client_recv(client, &tmd, sizeof(struct time_move_data))) {
				pthread_mutex_lock(&stlight.mutex);
				log_local("Fail sending time move data", LOG_ERROR);
				pthread_mutex_unlock(&stlight.mutex);
				return;
			}
			pthread_mutex_lock(&stlight.mutex);
			struct time_on *last_ton = move_time_get_on();
			struct time_off *last_toff = move_time_get_off();
			pthread_mutex_unlock(&stlight.mutex);

			if (last_ton->on_lh1 != tmd.on_lh1 || last_ton->on_lh2 != tmd.on_lh2) {
				pthread_mutex_lock(&stlight.mutex);
				ton.on_lh1 = tmd.on_lh1;
				ton.on_lh2 = tmd.on_lh2;
				move_time_set_on(&ton);
				pthread_mutex_unlock(&stlight.mutex);
			}

			if (last_toff->off_lh1 != tmd.off_lh1 || last_toff->off_lh2 != tmd.off_lh2) {
				pthread_mutex_lock(&stlight.mutex);
				toff.off_lh1 = tmd.off_lh1;
				toff.off_lh2 = tmd.off_lh2;
				move_time_set_off(&toff);
				pthread_mutex_unlock(&stlight.mutex);
			}
			break;
		}
	}
}

static void accept_error(void *data)
{
	pthread_mutex_lock(&stlight.mutex);
	log_local("Fail accepting new client", LOG_ERROR);
	pthread_mutex_lock(&stlight.mutex);
}

static void* timer_thread(void *data)
{
	char time[TIME_SIZE];
	struct move_detect *md = (struct move_detect *)data;
	struct lamp_cfg *lc = configs_get_lamps();
	
	puts("Starting move detect timer...");

	for (;;) {
		pthread_mutex_lock(&stlight.mutex);
		struct time_on *ton = move_time_get_on();
		struct time_off *toff = move_time_get_off();
		pthread_mutex_unlock(&stlight.mutex);

		if (stlight.time_mov1 > 0) {
			unsigned h;
			time_now(time);
			char *hour = strtok(time, ":");
			sscanf(hour, "%u", &h);
			stlight.time_mov1--;

			if (h >= ton->on_lh1 || h <= toff->off_lh1) {
				pthread_mutex_lock(&stlight.mutex);
				printf("Moving! Switch on light # %d\n", lc->lamps[md->lamp1]);
				digitalWrite(lc->lamps[md->lamp1], LOW);
				stlight.lamps[md->lamp1] = 1;
				pthread_mutex_unlock(&stlight.mutex);
			}
		} else {
			if (!stlight.hand_lamp1) {
				pthread_mutex_lock(&stlight.mutex);
				digitalWrite(lc->lamps[md->lamp1], HIGH);
				stlight.lamps[md->lamp1] = 0;
				pthread_mutex_unlock(&stlight.mutex);
			}
		}

		if (stlight.time_mov2 > 0) {
			unsigned h;
			time_now(time);
			char *hour = strtok(time, ":");
			sscanf(hour, "%u", &h);
			stlight.time_mov2--;

			if (h >= ton->on_lh2 || h <= toff->off_lh2) {
				pthread_mutex_lock(&stlight.mutex);
				printf("Moving! Switch on light # %d\n", lc->lamps[md->lamp2]);
				digitalWrite(lc->lamps[md->lamp2], LOW);
				stlight.lamps[md->lamp2] = 1;
				pthread_mutex_unlock(&stlight.mutex);
			}
		} else {
			if (!stlight.hand_lamp2) {
				pthread_mutex_lock(&stlight.mutex);
				digitalWrite(lc->lamps[md->lamp2], HIGH);
				stlight.lamps[md->lamp2] = 0;
				pthread_mutex_unlock(&stlight.mutex);
			}
		}		
        	delay(500);
	}
	return NULL;
}

static void* move_detect_thread(void *data)
{
	struct move_detect *md = (struct move_detect *)data;
	struct toilet *tlt = configs_get_toilet();
	struct lamp_cfg *lc = configs_get_lamps();

	puts("Starting move detect server...");	
	pinMode(md->mov1, INPUT);
	pinMode(md->mov2, INPUT);
	pinMode(tlt->btn, INPUT);
	for (;;) {
		int mov1 = digitalRead(md->mov1);
		int mov2 = digitalRead(md->mov2);
		int tb = digitalRead(tlt->btn);

		if (tb == 1) {
			pthread_mutex_lock(&stlight.mutex);
			if (!stlight.tb_on) {
				stlight.lamps[2] = 1;
				digitalWrite(lc->lamps[2], LOW);
				stlight.lamps[3] = 1;
				digitalWrite(lc->lamps[3], LOW);
				stlight.lamps[7] = 1;
				digitalWrite(lc->lamps[7], LOW);
				stlight.tb_on = true;
				stlight.hand_lamp1 = true;
				puts("Turn ON toilet projectors");
			} else {
				stlight.lamps[2] = 0;
				digitalWrite(lc->lamps[2], HIGH);
				stlight.lamps[3] = 0;
				digitalWrite(lc->lamps[3], HIGH);
				stlight.lamps[7] = 0;
				digitalWrite(lc->lamps[7], HIGH);
				stlight.tb_on = false;
				stlight.hand_lamp1 = false;
				puts("Turn OFF toilet projectors");
			}
			pthread_mutex_unlock(&stlight.mutex);
			delay(500);
		}
		if (mov1 == 1) {
			pthread_mutex_lock(&stlight.mutex);
			stlight.time_mov1 = MOVE_TIMEOUT;
			pthread_mutex_unlock(&stlight.mutex);
		}
		if (mov2 == 1) {
			pthread_mutex_lock(&stlight.mutex);
			stlight.time_mov2 = MOVE_TIMEOUT;
			pthread_mutex_unlock(&stlight.mutex);
		}
		delay(100);
	}
	return NULL;
}

void move_detect_start(void)
{
	pthread_t md_th;
	pthread_t tmr_th;
	struct move_detect *md = configs_get_md();

	pthread_create(&md_th, NULL, &move_detect_thread, (void *)md);
	pthread_create(&tmr_th, NULL, &timer_thread, (void *)md);

	pthread_detach(md_th);
	pthread_detach(tmr_th);
}

bool stlight_start(void)
{
	struct server_cfg *sc = configs_get_server();
	struct lamp_cfg *lc = configs_get_lamps();

	puts("Starting street light server...");
	pthread_mutex_init(&stlight.mutex, NULL);
	for (size_t i = 0; i < 8; i++) {
		stlight.lamps[i] = 0;
		pinMode(lc->lamps[i], OUTPUT);
		digitalWrite(lc->lamps[i], HIGH);
	}

	tcp_server_set_newsession_cb(&stlight.server, new_session, NULL);
	tcp_server_set_accepterr_cb(&stlight.server, accept_error, NULL);
	if (!tcp_server_bind(&stlight.server, sc->port, sc->max_users)) {
		log_local("Fail starting StreetLight server", LOG_ERROR);
		return false;
	}
	pthread_mutex_destroy(&stlight.mutex);
	return true;
}
