/* SmartHome: server application
 *
 * Copyright (C) 2016 Sergey Denisov.
 * Written by Sergey Denisov aka LittleBuster (DenisovS21@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version 3
 * of the Licence, or (at your option) any later version.
 */

#include "webserver.h"
#include "tcpserver.h"
#include "log.h"
#include "configs.h"
#include "handlers/hstlight.h"
#include "handlers/hhlight.h"
#include "handlers/htermo.h"
#include "handlers/hsecurity.h"
#include "handlers/hmeteo.h"
#include "handlers/hcam.h"
#include <stdio.h>
#include <string.h>
#include <pthread.h>


static struct {
	pthread_mutex_t mutex;
	struct tcp_server server;
} web;


static bool get_parse(const char *restrict page, char *out)
{
	char req[255];

	char *p_req = req;
	while (*page) {
		if (*page == '\n')
			break;
		*p_req = *page;
		p_req++;
		page++;
	}
	*p_req = '\0';
	
	char *str = strtok(req, " ");
	if (str == NULL) {
		log_local("Fail parsing GET header request.", LOG_ERROR);
		return false;
	}
	str = strtok(NULL, " ");
	if (str == NULL) {
		log_local("Fail parsing GET header request.", LOG_ERROR);
		return false;
	}
	strcpy(out, str);
	return true;
}

static void new_session(struct tcp_client *client, void *data)
{
	char page[1024];
	char get_req[255];
	char module[255];
	char command[255];

	if (!tcp_client_recv(client, (void *)page, 1024)) {
		log_local("Fail receiving GET request.", LOG_ERROR);
		return;
	}
	if (!get_parse(page, get_req))
		return;

	char *get_str = strtok(get_req, "?");
	if (get_str == NULL) {
		log_local("Invalid GET request.", LOG_ERROR);
		return;
	}
	strcpy(module, get_str);
	get_str = strtok(NULL, "?");
	if (get_str == NULL) {
		log_local("Invalid GET request.", LOG_ERROR);
		return;
	}
	strcpy(command, get_str);
	puts("=================================================");

	/*
	 * Calling modules
	 */
	if (!strcmp(module, "/stlight")) {
		stlight_handle(command, client);
		return;
	}

	if (!strcmp(module, "/hlight")) {
		hlight_handle(command, client);
		return;
	}

	if (!strcmp(module, "/cam")) {
		cam_handle(command, client);
		return;
	}

	if (!strcmp(module, "/meteo")) {
		meteo_handle(command, client);
		return;
	}

	if (!strcmp(module, "/termo")) {
		termo_handle(command, client);
		return;
	}
	if (!strcmp(module, "/security")) {
		security_handle(command, client);
		return;
	}
}

static void accept_error(void *data)
{
	pthread_mutex_lock(&web.mutex);
	log_local("Fail accepting new client", LOG_ERROR);
	pthread_mutex_lock(&web.mutex);
}


bool web_server_start(void)
{
	struct server_cfg *wsc = configs_get_server();

	pthread_mutex_init(&web.mutex, NULL);

	tcp_server_set_newsession_cb(&web.server, new_session, NULL);
	tcp_server_set_accepterr_cb(&web.server, accept_error, NULL);
	if (!tcp_server_bind(&web.server, wsc->port, wsc->max_users)) {
		log_local("Fail starting smart web server.", LOG_ERROR);
		return false;
	}
	pthread_mutex_destroy(&web.mutex);
	return true;
}