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

#include "security.h"
#include "configs.h"
#include "log.h"
#include "tcpclient.h"
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
 

static struct {
	pthread_t sec_th;
	pthread_t alarm_th;

	struct tcp_client client;

	bool status;

} security = {
	.status = true,
};

static bool send_sms(const char *message)
{
	char data[1024];
	struct security_cfg *sec = configs_get_security();
	
	strcpy(data, "GET /sms/send?api_id=");
	strcat(data, sec->sms_id);
	strcat(data, "&to=");
	strcat(data, sec->sms_phone);
	strcat(data, "&text=");
	strcat(data, message);
	strcat(data, " HTTP/1.1\r\n");
	strcat(data, "Host: sms.ru\r\n"
				 "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:48.0) Gecko/20100101 Firefox/48.0\r\n"
				 "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
				 "Accept-Language: en-US,en;q=0.5\r\n"
				 "Accept-Encoding: gzip, deflate\r\n"
				 "DNT: 1\r\n"
				 "Connection: keep-alive\r\n"
				 "Upgrade-Insecure-Requests: 1\r\n");

	puts(data);
	return true; //debug;
	/*
	 * Connecting to SMS.ru
	 */
	if (!tcp_client_connect(&security.client, "89.188.102.66", 80)) {
		log_local("Fail connecting to SMS server.", LOG_ERROR);
		return false;
	}
	if (!tcp_client_send(&security.client, data, strlen(data))) {
		log_local("Fail sending SMS.", LOG_ERROR);
		tcp_client_close(&security.client);
		return false;
	}
	tcp_client_close(&security.client);
	return true;
}

static void *alarm_thread(void *data)
{
	for (;;) {
		delay(500);
		delay(1000);
	}
	return NULL;
}

static void *security_thread(void *data)
{
	struct security_cfg *sec = configs_get_security();

	for (;;) {
		uint8_t mv = digitalRead(sec->move);
		uint8_t dr = digitalRead(sec->door);

		if (dr) {
			pthread_create(&security.alarm_th, NULL, &alarm_thread, NULL);
			pthread_detach(security.alarm_th);
			send_sms("DACHA:+WARNING!!!+Vori+lezut+v+deverb!!!");
			break;
		}
		delay(1000);
	}
	return NULL;
}


void security_start(void)
{
	pthread_create(&security.sec_th, NULL, &security_thread, NULL);
	pthread_detach(security.sec_th);
	puts("Starting security module...");
}

void security_set_on(void)
{
	security.status = true;
}

void security_set_off(void)
{
	security.status = false;
}

bool security_get_status(void)
{
	return security.status;
}