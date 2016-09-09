/* SmartHome: CAM server library
 *
 * Copyright (C) 2016 Sergey Denisov.
 * Written by Sergey Denisov aka LittleBuster (DenisovS21@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public Licence
 * as published by the Free Software Foundation; either version 3
 * of the Licence, or (at your option) any later version.
 */

#include <stdio.h>
#include <string.h>
#include "log.h"
#include "configs.h"
#include "tcpclient.h"

enum recv_cmd {
	GET_PHOTO,
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


bool house_set_log(const char *filename)
{
	return house_log_set_path(filename);
}

uint8_t house_load_configs(const char *filename)
{
	return house_configs_load(filename);
}

bool house_cam_get_photo(uint8_t cam_num)
{
	struct command cmd;
	struct command answ;
	struct tcp_client client;
	struct server_cfg *hsc = house_configs_get_server();

	if (!tcp_client_connect(&client, hsc->ip, hsc->port)) {
		house_log_local("Can not connect to house server.", HOUSE_LOG_ERROR);
		return false;
	}
	cmd.code = GET_PHOTO;
	if (!tcp_client_send(&client, &cmd, sizeof(struct command))) {
		tcp_client_close(&client);
		house_log_local("Fail sending house command.", HOUSE_LOG_ERROR);
		return false;
	}
	cmd.code = cam_num;
	if (!tcp_client_send(&client, &cmd, sizeof(struct command))) {
		tcp_client_close(&client);
		house_log_local("Fail sending cam number.", HOUSE_LOG_ERROR);
		return false;
	}
	if (!tcp_client_recv(&client, &answ, sizeof(struct command))) {
		tcp_client_close(&client);
		house_log_local("Fail receiving house answ.", HOUSE_LOG_ERROR);
		return false;
	}
	if (answ.code != PHOTO_OK) {
		tcp_client_close(&client);
		house_log_local("Fail getting photo.", HOUSE_LOG_ERROR);
		return false;
	}
	tcp_client_close(&client);
	return true;
}

bool house_meteo_get_data(float *out_meteo)
{
	struct command cmd;
	struct meteo_answ answ;
	struct tcp_client client;
	struct server_cfg *hsc = house_configs_get_server();

	if (!tcp_client_connect(&client, hsc->ip, hsc->port)) {
		house_log_local("Can not connect to house server.", HOUSE_LOG_ERROR);
		return false;
	}
	cmd.code = GET_METEO;
	if (!tcp_client_send(&client, &cmd, sizeof(struct command))) {
		tcp_client_close(&client);
		house_log_local("Fail sending house command.", HOUSE_LOG_ERROR);
		return false;
	}
	if (!tcp_client_recv(&client, &answ, sizeof(struct meteo_answ))) {
		tcp_client_close(&client);
		house_log_local("Fail receiving house answ.", HOUSE_LOG_ERROR);
		return false;
	}
	out_meteo[0] = answ.street_temp;
	out_meteo[1] = answ.street_hum;
	out_meteo[2] = answ.room_temp;
	out_meteo[3] = answ.room_hum;
	out_meteo[4] = answ.second_temp;
	out_meteo[5] = answ.second_hum;
	out_meteo[6] = answ.veranda_temp;
	out_meteo[7] = answ.veranda_hum;
	tcp_client_close(&client);
	return true;
}