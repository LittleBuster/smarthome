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


static void get(const char *data, char *out)
{
	while (*data) {
		if (*data == '\n')
			break;

		*out = *data;
		data++;
		out++;
	}
	*out = '\0';
}

static void new_session(struct tcp_client *client, void *data)
{
	char page[1024];
	char get_req[255];

	if (!tcp_client_recv(client, page, 1024)) {
		log_local("Fail receiving GET request.", LOG_ERROR);
		return;
	}
	get(data, get_req);
	puts(get_req);
}

static void accept_error(void *data)
{
	pthread_mutex_lock(&web.mutex);
	log_local("Fail accepting new client", LOG_ERROR);
	pthread_mutex_lock(&web.mutex);
}


bool web_server_start(void)
{
	struct server_cfg *sc = configs_get_server();

	pthread_mutex_init(&web.mutex, NULL);

	tcp_server_set_newsession_cb(&web.server, new_session, NULL);
	tcp_server_set_accepterr_cb(&web.server, accept_error, NULL);
	if (!tcp_server_bind(&web.server, sc->port, sc->max_users)) {
		log_local("Fail starting StreetLight server", LOG_ERROR);
		return false;
	}
	pthread_mutex_destroy(&web.mutex);
	return true;
}
