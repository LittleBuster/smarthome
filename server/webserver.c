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
#include <smarthome/cam.h>
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

static void call_stlight_module(const char *command, struct tcp_client *client)
{
	char func[50];
	char answ[255];
	unsigned lamp;
	size_t i = 0;
	char *params = strtok(command, "=");

	while (params) {
		if (i == 0) {
			strcpy(func, params);
			if (!strcmp(params, "get_status"))
				break;
		}
		if (i == 1)
			sscanf(params, "%u", &lamp);
		params = strtok(NULL, "=");
		i++;
	}
	if (!strcmp(func, "get_status")) {
		struct status_data stat;

		if (!stlight_get_status(&stat)) {
			log_local("Fail sending GET_STATUS command", LOG_ERROR);
			strcpy(answ, "HTTP/1.1 200 OK\r\n"
					"Content-Type: application/json; charset=UTF-8\r\n\r\n"
					"{\"result\": \"fail\"}\r\n");
			if (!tcp_client_send(client, answ, strlen(answ)))
				log_local("Stlight: Fail sending answ to server", LOG_ERROR);
		}
		strcpy(answ, "HTTP/1.1 200 OK\r\n"
				"Content-Type: application/json; charset=UTF-8\r\n\r\n"
				"{\"result\": \"ok\", \"lamps\":[");
		for (size_t i = 0; i < 8; i++) {
			char num[2];
			
			sprintf(num, "%u", stat.lamps[i]);
			strcat(answ, num);
			if (i != 7)
				strcat(answ, ",");
		}
		strcat(answ, "]}\r\n");
		
		if (!tcp_client_send(client, answ, strlen(answ)))
			log_local("Stlight: Fail sending answ to server", LOG_ERROR);
		return;
	}

	if (!strcmp(func, "switch_on")) {
		if (!stlight_switch_on(lamp)) {
			log_local("Fail sending SWITCH_ON command", LOG_ERROR);
			strcpy(answ, "HTTP/1.1 200 OK\r\n"
					"Content-Type: application/json; charset=UTF-8\r\n\r\n"
					"{\"result\": \"fail\"}\r\n");
			if (!tcp_client_send(client, answ, strlen(answ)))
				log_local("Stlight: Fail sending answ to server", LOG_ERROR);
		}
		printf("Switch ON #%u sended.\n", lamp);
	}

	if (!strcmp(func, "switch_off")) {
		if (!stlight_switch_off(lamp)) {
			log_local("Fail sending SWITCH_OFF command", LOG_ERROR);
			strcpy(answ, "HTTP/1.1 200 OK\r\n"
					"Content-Type: application/json; charset=UTF-8\r\n\r\n"
					"{\"result\": \"fail\"}\r\n");
			if (!tcp_client_send(client, answ, strlen(answ)))
				log_local("Stlight: Fail sending answ to server", LOG_ERROR);
		}
		printf("Switch OFF #%u sended.\n", lamp);
	}

	strcpy(answ, "HTTP/1.1 200 OK\r\n"
			"Content-Type: application/json; charset=UTF-8\r\n\r\n"
			"{\"result\": \"ok\"}\r\n");
	if (!tcp_client_send(client, answ, strlen(answ)))
		log_local("Stlight: Fail sending answ to server", LOG_ERROR);
}

static void call_cam_module(const char *command, struct tcp_client *client)
{
	char func[50];
	char answ[255];
	unsigned cam;
	size_t i = 0;
	char *params = strtok(command, "=");

	while (params) {
		if (i == 0)
			strcpy(func, params);
		if (i == 1)
			sscanf(params, "%u", &lamp);
		params = strtok(NULL, "=");
		i++;
	}
	if (!strcmp(func, "get_photo")) {
		if (!cam_get_photo("photo.jpg", cam)) {
			log_local("Fail sending GET_PHOTO command", LOG_ERROR);
			strcpy(answ, "HTTP/1.1 200 OK\r\n"
					"Content-Type: application/json; charset=UTF-8\r\n\r\n"
					"{\"result\": \"fail\"}\r\n");
			if (!tcp_client_send(client, answ, strlen(answ)))
				log_local("CAM: Fail sending answ to server", LOG_ERROR);
		}
		strcpy(answ, "HTTP/1.1 200 OK\r\n"
				"Content-Type: application/json; charset=UTF-8\r\n\r\n"
				"{\"result\": \"ok\"}\r\n");
		if (!tcp_client_send(client, answ, strlen(answ)))
			log_local("CAM: Fail sending answ to server", LOG_ERROR);
	}
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
		call_stlight_module(command, client);

	if (!strcmp(module, "/cam"))
		call_cam_module(command, client);
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
