/* SmartHome: HomeLight server library
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
#include "hlight.h"
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
	unsigned lamp;
};

struct switch_answ {
	uint8_t code;
};


bool hlight_set_log(const char *filename)
{
	return hl_log_set_path(filename);
}

uint8_t hlight_load_configs(const char *filename)
{
	return hl_configs_load(filename);
}

bool hlight_switch_on(unsigned lamp)
{
	struct tcp_client client;
	struct command cmd;
	const struct server_cfg *sc = hl_configs_get_server();

	cmd.code = SWITCH_ON;
	cmd.lamp = lamp;

	if(!tcp_client_connect(&client, sc->ip, sc->port)) {
		hl_log_local("Fail connecting to server.", HL_LOG_ERROR);
		return false;
	}
	if (!tcp_client_send(&client, (const void *)&cmd, sizeof(struct command))) {
		hl_log_local("Fail sending cmd to server.", HL_LOG_ERROR);
		tcp_client_close(&client);
		return false;
	}
	tcp_client_close(&client);
	return true;
}

bool hlight_switch_off(unsigned lamp)
{
	struct tcp_client client;
	struct command cmd;
	const struct server_cfg *sc = hl_configs_get_server();

	cmd.code = SWITCH_OFF;
	cmd.lamp = lamp;

	if(!tcp_client_connect(&client, sc->ip, sc->port)) {
		hl_log_local("Fail connecting to server.", HL_LOG_ERROR);
		return false;
	}
	if (!tcp_client_send(&client, (const void *)&cmd, sizeof(struct command))) {
		hl_log_local("Fail sending cmd to server.", HL_LOG_ERROR);
		tcp_client_close(&client);
		return false;
	}
	tcp_client_close(&client);
	return true;
}

bool hlight_get_status(struct hl_status_data *restrict status)
{
	struct tcp_client client;
	struct command cmd;
	const struct server_cfg *sc = hl_configs_get_server();

	cmd.code = GET_STATUS;

	if(!tcp_client_connect(&client, sc->ip, sc->port)) {
		hl_log_local("Fail connecting to server.", HL_LOG_ERROR);
		return false;
	}
	if (!tcp_client_send(&client, (const void *)&cmd, sizeof(struct command))) {
		hl_log_local("Fail sending cmd to server.", HL_LOG_ERROR);
		tcp_client_close(&client);
		return false;
	}
	if (!tcp_client_recv(&client, (void *)status, sizeof(struct hl_status_data))) {
		hl_log_local("Fail receiving answ from server.", HL_LOG_ERROR);
		tcp_client_close(&client);
		return false;
	}
	tcp_client_close(&client);
	return true;
}