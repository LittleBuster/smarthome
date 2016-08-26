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
#include "pthread.h"
#include <smarthome/stlight.h>
#include <stdio.h>
#include <string.h>


static struct {
	pthread_mutex_t mutex;
	struct tcp_server server;
} web;


void get(const char *page, char *out)
{
	char req[255];

	char *p_req = req;
	while (*page) {
		*p_req = *page;
		p_req++;
		page++;
	}
	*p_req = '\0';
	
	size_t i = 0;
	char *str = strtok(req, " ");
	while (str) {
		if (i == 1) {
			strcpy(out, str);
			break;
		}
		str = strtok(NULL, " ");
		i++;
	}
}

static void call_stlight_module(const char *command)
{
}

static void new_session(struct tcp_client *client, void *data)
{
	char page[1024];
	char get_req[255];
	char module[255];
	char command[255];
	size_t i = 0;

	if (!tcp_client_recv(client, page, 1024)) {
		log_local("Fail receiving GET request.", LOG_ERROR);
		return;
	}
	get(page, get_req);
	char *get_str = strtok(get_req, "?");
	
	while (get_str) {
		if (i == 0)
			strcpy(module, get_str);
		if (i == 1)
			strcpy(command, get_str);
		get_str = strtok(NULL, "?");
		i++;
	}
	if (!strcmp(module, "/stlight"))
		call_stlight_module(command);
}

static void accept_error(void *data)
{
	pthread_mutex_lock(&web.mutex);
	log_local("Fail accepting new client", LOG_ERROR);
	pthread_mutex_lock(&web.mutex);
}


bool web_server_start(void)
{
	struct server_cfg *wsc = wconfigs_get_server();

	pthread_mutex_init(&web.mutex, NULL);

	tcp_server_set_newsession_cb(&web.server, new_session, NULL);
	tcp_server_set_accepterr_cb(&web.server, accept_error, NULL);
	if (!tcp_server_bind(&web.server, wsc->port, wsc->max_users)) {
		log_local("Fail starting StreetLight server", LOG_ERROR);
		return false;
	}
	pthread_mutex_destroy(&web.mutex);
	return true;
}
