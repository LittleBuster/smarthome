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
	PHOTO_OK,
	PHOTO_FAIL
};

enum cam_list {
	CAM_1 = 0,
	CAM_2 = 1
};

struct command {
	uint8_t code;
	uint8_t cam;
};


bool cam_set_log(const char *filename)
{
	return cam_log_set_path(filename);
}

uint8_t cam_load_configs(const char *filename)
{
	return cam_configs_load(filename);
}

bool cam_get_photo(uint8_t cam_num)
{
	struct command cmd;
	struct command answ;
	struct tcp_client client;
	struct server_cfg *csc = cam_configs_get_server();

	if (!tcp_client_connect(&client, csc->ip, csc->port)) {
		cam_log_local("Can not connect to CAM server.", LOG_ERROR);
		return false;
	}
	cmd.code = GET_PHOTO;
	cmd.cam = cam_num;
	if (!tcp_client_send(&client, &cmd, sizeof(struct command))) {
		tcp_client_close(&client);
		cam_log_local("Fail sending CAM command.", LOG_ERROR);
		return false;
	}
	if (!tcp_client_recv(&client, &answ, sizeof(struct command))) {
		tcp_client_close(&client);
		cam_log_local("Fail receiving CAM answ.", LOG_ERROR);
		return false;
	}
	if (answ.code != PHOTO_OK) {
		tcp_client_close(&client);
		cam_log_local("Fail getting photo.", LOG_ERROR);
		return false;
	}
	tcp_client_close(&client);
	return true;
}
