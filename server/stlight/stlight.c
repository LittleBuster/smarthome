/* SmartHome: StreetLight server library
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
#include "stlight.h"
#include "../log.h"
#include "configs.h"
#include "../tcpclient.h"


enum {
	SWITCH_ON,
	SWITCH_OFF,
	GET_STATUS,
	SET_STATUS
};

struct command {
	uint8_t code;
	unsigned lamp;
};

struct switch_answ {
	uint8_t code;
};


bool stlight_set_log(const char *filename)
{
	return log_set_path(filename);
}

uint8_t stlight_load_configs(const char *filename)
{
	return configs_load(filename);
}

bool stlight_switch_on(unsigned lamp)
{
	struct tcp_client client;
	struct command cmd;
	struct server_cfg *sc = configs_get_server();

	cmd.code = SWITCH_ON;
	cmd.lamp = lamp;

	if(!tcp_client_connect(&client, sc->ip, sc->port))
		return false;
	if (!tcp_client_send(&client, &cmd, sizeof(struct command))) {
		tcp_client_close(&client);
		return false;
	}
	tcp_client_close(&client);
	return true;
}

bool stlight_switch_off(unsigned lamp)
{
	struct tcp_client client;
	struct command cmd;
	struct server_cfg *sc = configs_get_server();

	cmd.code = SWITCH_OFF;
	cmd.lamp = lamp;

	if(!tcp_client_connect(&client, sc->ip, sc->port))
		return false;
	if (!tcp_client_send(&client, &cmd, sizeof(struct command))) {
		tcp_client_close(&client);
		return false;
	}
	tcp_client_close(&client);
	return true;
}

bool stlight_get_status(struct status_data *status)
{
	struct tcp_client client;
	struct command cmd;
	struct server_cfg *sc = configs_get_server();

	cmd.code = GET_STATUS;

	if(!tcp_client_connect(&client, sc->ip, sc->port))
		return false;
	if (!tcp_client_send(&client, &cmd, sizeof(struct command))) {
		tcp_client_close(&client);
		return false;
	}
	if (!tcp_client_recv(&client, status, sizeof(struct status_data))) {
		tcp_client_close(&client);
		return false;
	}
	tcp_client_close(&client);
	return true;
}

bool stlight_set_status(const struct status_data *status)
{
	struct tcp_client client;
	struct command cmd;
	struct server_cfg *sc = configs_get_server();

	cmd.code = SET_STATUS;

	if(!tcp_client_connect(&client, sc->ip, sc->port))
		return false;
	if (!tcp_client_send(&client, &cmd, sizeof(struct command))) {
		tcp_client_close(&client);
		return false;
	}
	if (!tcp_client_send(&client, status, sizeof(struct status_data))) {
		tcp_client_close(&client);
		return false;
	}
	tcp_client_close(&client);
	return true;
}