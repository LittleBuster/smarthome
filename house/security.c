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
#include <stdbool.h>
#include <curl/curl.h>
 

static struct {
	pthread_t sec_th;
	pthread_t alarm_th;

	struct tcp_client client;

	bool status;

	uint8_t move_cnt;
	unsigned time;
	char msg[255];
} security = {
	.status = false,
	.time = 0,
	.move_cnt = 0
};


static bool send_sms(const char *message)
{
	char data[1024];
	CURL *curl_handle;
	struct security_cfg *sec = configs_get_security();
	
	strcpy(data, "http://sms.ru/sms/send?api_id=");
	strcat(data, sec->sms_id);
	strcat(data, "&to=");
	strcat(data, sec->sms_phone);
	strcat(data, "&text=");
	strcat(data, message);

    curl_handle = curl_easy_init();
    if (curl_handle) {
        curl_easy_setopt(curl_handle, CURLOPT_URL, data);
        curl_easy_perform(curl_handle);
        curl_easy_cleanup(curl_handle);
        return true;
	}
	return false;
}

static void *alarm_thread(void *data)
{
	struct security_cfg *sec = (struct security_cfg *)data;

	for (uint8_t i = 0; i < 20; i++) {
		if (send_sms(security.msg))
			break;
		else
			log_local("Fail sending security SMS.", LOG_ERROR);
	}

	for (;;) {
		digitalWrite(sec->alarm_room, LOW);
		digitalWrite(sec->alarm_street, HIGH);
		delay(500);
		digitalWrite(sec->alarm_room, HIGH);
		digitalWrite(sec->alarm_street, LOW);
		delay(500);
	}
	return NULL;
}

static void *security_thread(void *data)
{
	struct security_cfg *sec = (struct security_cfg *)data;

	for (;;) {
		if (security.status) {
			uint8_t mv = digitalRead(sec->move);
			uint8_t dr = digitalRead(sec->door);

			if (mv == 1) {
				security.move_cnt++;
				if (security.move_cnt == 3) {
					puts("Security: Moving detected!");
					strcpy(security.msg, "DACHA:+WARNING!!!+Motion+detected!");
					pthread_create(&security.alarm_th, NULL, &alarm_thread, (void *)sec);
					pthread_detach(security.alarm_th);
					break;
				}
				delay(4000);
			}

			if (dr == 0) {
				puts("Security: Door detected!");
				strcpy(security.msg, "DACHA:+WARNING!!!+Door+is+opened!");
				pthread_create(&security.alarm_th, NULL, &alarm_thread, (void *)sec);
				pthread_detach(security.alarm_th);
				break;
			}
		}
		security.time++;
		if (security.time == 300) {
			security.time = 0;
			security.move_cnt = 0;
		}
		delay(1000);
	}
	return NULL;
}


void security_start(void)
{
	struct security_cfg *sec = configs_get_security();

	pinMode(sec->move, INPUT);
	pinMode(sec->door, INPUT);
	pinMode(sec->alarm_room, OUTPUT);
	pinMode(sec->alarm_street, OUTPUT);

	digitalWrite(sec->alarm_room, LOW);
	digitalWrite(sec->alarm_street, LOW);

	pthread_create(&security.sec_th, NULL, &security_thread, (void *)sec);
	pthread_detach(security.sec_th);
	puts("Starting security module...");
}

void security_set_on(void)
{
	security.status = true;
	puts("Security is on.");
}

void security_set_off(void)
{
	security.status = false;
	puts("Security is off.");
}

void security_get_status(uint8_t *status)
{
	if (security.status)
		*status = 1;
	else
		*status = 0;
}
