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

#include "house.h"
#include "configs.h"
#include "log.h"
#include "tcpserver.h"
#include "tcpclient.h"
#include "meteo.h"
#include "cam.h"
#include "termo.h"
#include "security.h"
#include <pthread.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>


static struct {
	pthread_mutex_t mutex;
	struct tcp_server server;
} house;


static void new_session(struct tcp_client *s_client, void *data)
{
	struct command cmd;

	if (!tcp_client_recv(s_client, (void *)&cmd, sizeof(struct command))) {
		pthread_mutex_lock(&house.mutex);
		log_local("Fail receiving command.", LOG_ERROR);
		pthread_mutex_unlock(&house.mutex);
		return;
	}
	switch (cmd.code) {
		case TERMO_ON: {
			pthread_mutex_lock(&house.mutex);
			if (!termo_control_on())
				log_local("Termocontrol: fail switching on", LOG_ERROR);
			else
				puts("Termocontrol: switching on [OK]");
			pthread_mutex_unlock(&house.mutex);
			break;
		}
		case TERMO_OFF: {
			pthread_mutex_lock(&house.mutex);
			if (!termo_control_off())
				log_local("Termocontrol: fail switching off", LOG_ERROR);
			else
				puts("Termocontrol: switching off [OK]");
			pthread_mutex_unlock(&house.mutex);
			break;
		}
		case TERMO_GET_TEMP: {
			struct termo_temp_answ answ;

			termo_get_temp(&answ.temp);

			if (!tcp_client_send(s_client, (const void *)&answ, sizeof(struct termo_temp_answ))) {
				pthread_mutex_lock(&house.mutex);
				log_local("Fail sending termo temp answ.", LOG_ERROR);
				pthread_mutex_unlock(&house.mutex);
				return;
			}
			break;
		}
		case TERMO_SET_TEMP: {
			struct termo_temp_answ ttemp;
			/*
			 * Receiving new temperature
			 */
			if (!tcp_client_recv(s_client, (void *)&ttemp, sizeof(struct termo_temp_answ))) {
				pthread_mutex_lock(&house.mutex);
				log_local("Fail receiving new termo temperature.", LOG_ERROR);
				pthread_mutex_unlock(&house.mutex);
				return;
			}

			pthread_mutex_lock(&house.mutex);
			if (!termo_set_temp(ttemp.temp))
				log_local("Termocontrol: fail changing temperature.", LOG_ERROR);
			pthread_mutex_unlock(&house.mutex);
			break;
		}
		case TERMO_GET_STATUS: {
			struct termo_stat_answ answ;

			termo_get_status(&answ.status, &answ.heater_status);
			if (!tcp_client_send(s_client, (const void *)&answ, sizeof(struct termo_stat_answ))) {
				pthread_mutex_lock(&house.mutex);
				log_local("Fail sending termo status answ.", LOG_ERROR);
				pthread_mutex_unlock(&house.mutex);
				return;
			}
			break;
		}
		case SECURITY_SET_ON: {
			security_set_on();
			break;
		}
		case SECURITY_SET_OFF: {
			security_set_off();
			break;
		}
		case SECURITY_GET_STATUS: {
			struct sec_stat_answ answ;

			security_get_status(&answ.status);
			if (!tcp_client_send(s_client, (const void *)&answ, sizeof(struct sec_stat_answ))) {
				pthread_mutex_lock(&house.mutex);
				log_local("Fail sending security status answ.", LOG_ERROR);
				pthread_mutex_unlock(&house.mutex);
				return;
			}
			break;
		}
		case GET_METEO: {
			struct meteo_answ answ;

			meteo_get_street_data(&answ.street_temp, &answ.street_hum);
			meteo_get_room_data(&answ.room_temp, &answ.room_hum);
			meteo_get_veranda_data(&answ.veranda_temp, &answ.veranda_hum);
			meteo_get_2nd_data(&answ.second_temp, &answ.second_hum);

			if (!tcp_client_send(s_client, (const void *)&answ, sizeof(struct meteo_answ))) {
				pthread_mutex_lock(&house.mutex);
				log_local("Fail sending meteo answ.", LOG_ERROR);
				pthread_mutex_unlock(&house.mutex);
				return;
			}
			break;
		}
		case GET_CAM_PHOTO: {
			struct command answ;
			/*
			 * Receiving CAM number
			 */
			if (!tcp_client_recv(s_client, (void *)&cmd, sizeof(struct command))) {
				pthread_mutex_lock(&house.mutex);
				log_local("Fail receiving cam number.", LOG_ERROR);
				pthread_mutex_unlock(&house.mutex);
				return;
			}
			/*
	 		 * Getting photo fron CAM by OpenCV
	 		 */
			if (!cam_get_photo(s_client, cmd.code, &house.mutex))
				return;

			/*
	 		 * Sending answ to client
	 		 */
			answ.code = PHOTO_OK;
			if (!tcp_client_send(s_client, &answ, sizeof(struct command))) {
				pthread_mutex_lock(&house.mutex);
				log_local("Fail sending answ.", LOG_ERROR);
				pthread_mutex_unlock(&house.mutex);
				return;
			}
			break;
		}
	}
}

bool house_server_start(void)
{
	const struct server_cfg *sc = configs_get_server();

	puts("Starting House server...");
	pthread_mutex_init(&house.mutex, NULL);

	/*
	 * Meteo module init
	 */
	if (!meteo_sensors_init()) {
		log_local("Fail initialization GPIO ports.", LOG_ERROR);
		return false;
	}
	meteo_sensors_start_timer(&house.mutex);
	if (!security_start()) {
		log_local("Fail loading security extended configs.", LOG_ERROR);
		return false;
	}
	if (!termo_start()) {
		log_local("Fail loading termo extended configs.", LOG_ERROR);
		return false;
	}
	/*
	 * TCP server init
	 */
	tcp_server_set_newsession_cb(&house.server, new_session, NULL);
	if (!tcp_server_bind(&house.server, sc->port, sc->max_users)) {
		log_local("Fail binding House server.", LOG_ERROR);
		return false;
	}
	pthread_mutex_destroy(&house.mutex);
	return true;
}
