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
#include "cstlight.h"
#include "configs.h"
#include "tcpclient.h"
#include "log.h"


enum {
	SWITCH_ON,
	SWITCH_OFF,
	GET_STATUS,
	SET_STATUS
};

struct command {
	uint8_t code;
	uint8_t lamp;
};

struct switch_answ {
	uint8_t code;
};


bool stlight_switch_on(uint8_t lamp)
{
	struct tcp_client client;
	struct command cmd;
	const struct module_cfg *sc = configs_get_stlight();

	cmd.code = SWITCH_ON;
	cmd.lamp = lamp;

	if(!tcp_client_connect(&client, sc->ip, sc->port)) {
		log_local("Fail connecting to server.", LOG_ERROR);
		return false;
	}
	if (!tcp_client_send(&client, &cmd, sizeof(struct command))) {
		log_local("Fail sending cmd to server.", LOG_ERROR);
		tcp_client_close(&client);
		return false;
	}
	tcp_client_close(&client);
	return true;
}

bool stlight_switch_off(uint8_t lamp)
{
	struct tcp_client client;
	struct command cmd;
	const struct module_cfg *sc = configs_get_stlight();

	cmd.code = SWITCH_OFF;
	cmd.lamp = lamp;

	if(!tcp_client_connect(&client, sc->ip, sc->port)) {
		log_local("Fail connecting to server.", LOG_ERROR);
		return false;
	}
	if (!tcp_client_send(&client, &cmd, sizeof(struct command))) {
		log_local("Fail sending cmd to server.", LOG_ERROR);
		tcp_client_close(&client);
		return false;
	}
	tcp_client_close(&client);
	return true;
}

bool stlight_get_status(struct status_data *restrict status)
{
	struct tcp_client client;
	struct command cmd;
	const struct module_cfg *sc = configs_get_stlight();

	cmd.code = GET_STATUS;

	if(!tcp_client_connect(&client, sc->ip, sc->port)) {
		log_local("Fail connecting to server.", LOG_ERROR);
		return false;
	}
	if (!tcp_client_send(&client, &cmd, sizeof(struct command))) {
		log_local("Fail sending cmd to server.", LOG_ERROR);
		tcp_client_close(&client);
		return false;
	}
	if (!tcp_client_recv(&client, status, sizeof(struct status_data))) {
		log_local("Fail receiving answ from server.", LOG_ERROR);
		tcp_client_close(&client);
		return false;
	}
	tcp_client_close(&client);
	return true;
}

bool stlight_set_status(const struct status_data *restrict status)
{
	struct tcp_client client;
	struct command cmd;
	const struct module_cfg *sc = configs_get_stlight();

	cmd.code = SET_STATUS;

	if(!tcp_client_connect(&client, sc->ip, sc->port)) {
		log_local("Fail connecting to server.", LOG_ERROR);
		return false;
	}
	if (!tcp_client_send(&client, &cmd, sizeof(struct command))) {
		log_local("Fail sending cmd to server.", LOG_ERROR);
		tcp_client_close(&client);
		return false;
	}
	if (!tcp_client_send(&client, status, sizeof(struct status_data))) {
		log_local("Fail sending status to server.", LOG_ERROR);
		tcp_client_close(&client);
		return false;
	}
	tcp_client_close(&client);
	return true;
}