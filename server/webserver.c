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
#include <smarthome/house.h>
#include <stdio.h>
#include <string.h>


static struct {
	pthread_mutex_t mutex;
	struct tcp_server server;
} web;


static bool get_parse(const char *page, char *out)
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

static void call_stlight_module(const char *command, struct tcp_client *restrict client)
{
	char func[50];
	char answ[255];
	unsigned lamp;
	
	char *params = strtok((const char *)command, "=");
	if (params == NULL) {
		log_local("Fail parsing STLIGHT request params.", LOG_ERROR);
		return;
	}
	/*
	 * Calling get status command
	 */
	if (!strcmp(params, "get_status")) {
		struct status_data stat;

		if (!stlight_get_status(&stat)) {
			log_local("Fail sending GET_STATUS command", LOG_ERROR);
			strcpy(answ, "HTTP/1.1 200 OK\r\n"
					"Content-Type: application/json; charset=UTF-8\r\n\r\n"
					"{\"result\": \"fail\"}\r\n");
			if (!tcp_client_send(client, answ, strlen(answ)))
				log_local("Stlight: Fail sending answ to server", LOG_ERROR);
		}
		/*
		 * Sending json answ with array
		 */
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
			log_local("Stlight: Fail sending get_status answ to server", LOG_ERROR);
		return;
	}
	/*
	 * If not get status
	 */	
	strcpy(func, params);
	params = strtok(NULL, "=");
	if (params == NULL) {
		log_local("Fail parsing STLIGHT request params.", LOG_ERROR);
		return;
	}
	sscanf(params, "%u", &lamp);

	/*
	 * Switching on lamp
	 */
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

	/*
	 * Switching off lamp
	 */
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

	/*
	 * Sending OK result
	 */
	strcpy(answ, "HTTP/1.1 200 OK\r\n"
			"Content-Type: application/json; charset=UTF-8\r\n\r\n"
			"{\"result\": \"ok\"}\r\n");
	if (!tcp_client_send(client, answ, strlen(answ)))
		log_local("Stlight: Fail sending answ to server", LOG_ERROR);
}

static void call_cam_module(const char *command, struct tcp_client *restrict client)
{
	char func[50];
	char answ[255];
	unsigned cam;

	char *params = strtok((const char *)command, "=");
	if (params == NULL) {
		log_local("Fail parsing CAM request params.", LOG_ERROR);
		return;
	}
	strcpy(func, params);
	params = strtok(NULL, "=");
	if (params == NULL) {
		log_local("Fail parsing CAM request params.", LOG_ERROR);
		return;
	}
	sscanf(params, "%u", &cam);

	if (!strcmp(func, "get_photo")) {
		if (!house_cam_get_photo(cam)) {
			log_local("Fail sending GET_PHOTO command", LOG_ERROR);

			strcpy(answ, "HTTP/1.1 200 OK\r\n"
						"Content-Type: application/json; charset=UTF-8\r\n\r\n"
						"{\"result\": \"fail\"}\r\n");
			if (!tcp_client_send(client, answ, strlen(answ))) {
				log_local("CAM: Fail sending answ to server", LOG_ERROR);
				return;
			}
			return;
		}
		strcpy(answ, "HTTP/1.1 200 OK\r\n"
					"Content-Type: application/json; charset=UTF-8\r\n\r\n"
					"{\"result\": \"ok\"}\r\n");
		if (!tcp_client_send(client, answ, strlen(answ))) {
			log_local("CAM: Fail sending answ to server", LOG_ERROR);
			return;
		}
	} else {
		log_local("Fail parsing CAM request func.", LOG_ERROR);
		return;
	}
}

static void call_termo_module(const char *command, struct tcp_client *restrict client)
{
	char func[50];
	char answ[255];
	char mode[10];

	if (!strcmp(command, "get_status")) {
		uint8_t status = 0;

		if (!house_termo_get_status(&status)) {
			log_local("Fail sending TERMO_GET_STATUS command", LOG_ERROR);
			strcpy(answ, "HTTP/1.1 200 OK\r\n"
					"Content-Type: application/json; charset=UTF-8\r\n\r\n"
					"{\"result\": \"fail\"}\r\n");
			if (!tcp_client_send(client, answ, strlen(answ))) {
				log_local("TERMO: Fail sending answ to server", LOG_ERROR);
				return;
			}
			return;
		}
		strcpy(answ, "HTTP/1.1 200 OK\r\n"
				"Content-Type: application/json; charset=UTF-8\r\n\r\n"
				"{\"result\": \"ok\", \"status\": ");
		if (status == 1)
			strcat(answ, "1");
		else
			strcat(answ, "0");
		strcat(answ, "}\r\n");
		if (!tcp_client_send(client, answ, strlen(answ))) {
			log_local("TERMO: Fail sending answ to server", LOG_ERROR);
			return;
		}
		return;
	}
	
	if (!strcmp(command, "get_temp")) {
		char ntemp[20];
		float temp;

		if (!house_termo_get_temp(&temp)) {
			log_local("Fail sending TERMO_GET_TEMP command", LOG_ERROR);
			strcpy(answ, "HTTP/1.1 200 OK\r\n"
					"Content-Type: application/json; charset=UTF-8\r\n\r\n"
					"{\"result\": \"fail\"}\r\n");
			if (!tcp_client_send(client, answ, strlen(answ))) {
				log_local("TERMO: Fail sending answ to server", LOG_ERROR);
				return;
			}
			return;
		}
		strcpy(answ, "HTTP/1.1 200 OK\r\n"
				"Content-Type: application/json; charset=UTF-8\r\n\r\n"
				"{\"result\": \"ok\", \"temp\": ");

		sprintf(ntemp, "%.2f", temp);
		strcat(answ, ntemp);

		strcat(answ, "}\r\n");
		if (!tcp_client_send(client, answ, strlen(answ))) {
			log_local("TERMO: Fail sending answ to server", LOG_ERROR);
			return;
		}
		return;
	}	

	char *params = strtok((const char *)command, "=");
	if (params == NULL) {
		log_local("Fail parsing CAM request params.", LOG_ERROR);
		return;
	}
	strcpy(func, params);
	params = strtok(NULL, "=");
	if (params == NULL) {
		log_local("Fail parsing CAM request params.", LOG_ERROR);
		return;
	}
	strcpy(mode, params);

	if (!strcmp(func, "termo_control")) {
		if (!strcmp(mode, "on")) {
			if (!house_termo_control_on()) {
				log_local("Fail sending TERMO_ON command", LOG_ERROR);
				strcpy(answ, "HTTP/1.1 200 OK\r\n"
						"Content-Type: application/json; charset=UTF-8\r\n\r\n"
						"{\"result\": \"fail\"}\r\n");
				if (!tcp_client_send(client, answ, strlen(answ))) {
					log_local("TERMO: Fail sending answ to server", LOG_ERROR);
					return;
				}
				return;
			}
		}
		if (!strcmp(mode, "off")) {
			if (!house_termo_control_off()) {
				log_local("Fail sending TERMO_OFF command", LOG_ERROR);
				strcpy(answ, "HTTP/1.1 200 OK\r\n"
						"Content-Type: application/json; charset=UTF-8\r\n\r\n"
						"{\"result\": \"fail\"}\r\n");
				if (!tcp_client_send(client, answ, strlen(answ))) {
					log_local("TERMO: Fail sending answ to server", LOG_ERROR);
					return;
				}
				return;
			}
		}
		strcpy(answ, "HTTP/1.1 200 OK\r\n"
					"Content-Type: application/json; charset=UTF-8\r\n\r\n"
					"{\"result\": \"ok\"}\r\n");
		if (!tcp_client_send(client, answ, strlen(answ))) {
			log_local("TERMO: Fail sending answ to server", LOG_ERROR);
			return;
		}
	} else {
		log_local("Fail parsing TERMO request func.", LOG_ERROR);
		return;
	}
}

static void call_meteo_module(const char *command, struct tcp_client *restrict client)
{
	char answ[255];

	if (!strcmp(command, "get_weather")) {
		float meteo[8];

		if (!house_meteo_get_data(meteo)) {
			log_local("Fail sending GET_METEO command", LOG_ERROR);

			strcpy(answ, "HTTP/1.1 200 OK\r\n"
						"Content-Type: application/json; charset=UTF-8\r\n\r\n"
						"{\"result\": \"fail\"}\r\n");
			if (!tcp_client_send(client, answ, strlen(answ))) {
				log_local("METEO: Fail sending answ to server", LOG_ERROR);
			}
			return;
		}
		strcpy(answ, "HTTP/1.1 200 OK\r\n"
					"Content-Type: application/json; charset=UTF-8\r\n\r\n"
					"{\"result\": \"ok\",\"meteo\": [");

		for (uint8_t i = 0; i < 8 ; i++) {
			char meteo_num[10];

			sprintf(meteo_num, "%0.2f", meteo[i]);
			strcat(answ, meteo_num);
			if (i != 7)
				strcat(answ, ",");
		}
		strcat(answ, "]}\r\n");
		if (!tcp_client_send(client, answ, strlen(answ))) {
			log_local("METEO: Fail sending answ to server", LOG_ERROR);
		}
	} else {
		log_local("Fail parsing meteo request func.", LOG_ERROR);
		return;
	}
}

static void new_session(struct tcp_client *client, void *data)
{
	char page[1024];
	char get_req[255];
	char module[255];
	char command[255];

	if (!tcp_client_recv(client, page, 1024)) {
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

	/*
	 * Calling modules
	 */
	if (!strcmp(module, "/stlight")) {
		call_stlight_module(command, client);
		return;
	}

	if (!strcmp(module, "/cam")) {
		call_cam_module(command, client);
		return;
	}

	if (!strcmp(module, "/meteo")) {
		call_meteo_module(command, client);
		return;
	}

	if (!strcmp(module, "/termo")) {
		call_termo_module(command, client);
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
