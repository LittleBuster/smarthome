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
}

bool house_server_start(void)
{
	struct server_cfg *sc = configs_get_server();

	puts("Starting House server...");
	pthread_mutex_init(&house.mutex, NULL);

	tcp_server_set_newsession_cb(&house.server, new_session, NULL);
	if (!tcp_server_bind(&house.server, sc->port, sc->max_users)) {
		log_local("Fail binding House server.", LOG_ERROR);
		return false;
	}
	pthread_mutex_destroy(&house.mutex);
	return true;
}
