#include "stlight.h"
#include "tcpserver.h"
#include "log.h"
#include "configs.h"
#include "pthread.h"
#include "utils.h"
#include <wiringPi.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>


static struct {
	unsigned time_mov1;
	unsigned time_mov2;
	unsigned lamps[8];

	pthread_mutex_t mutex;

	struct tcp_server server;
} stlight = {
	.time_mov1 = 0,
	.time_mov2 = 0,
};


static void new_session(struct tcp_client *client, void *data)
{
	struct command cmd;
	struct lamp_cfg *lc = configs_get_lamps();

	if(!tcp_client_recv(client, &cmd, sizeof(struct command))) {
		pthread_mutex_lock(&stlight.mutex);
		log_local("Fail reading client command", LOG_ERROR);
		pthread_mutex_unlock(&stlight.mutex);
		return;
	}

	switch (cmd.code) {
		case SWITCH_ON: {
			pthread_mutex_lock(&stlight.mutex);
			printf("Switching ON #%d\n", (int)cmd.lamp);
			digitalWrite(lc->lamps[cmd.lamp], LOW);
			stlight.lamps[cmd.lamp] = 1;
			pthread_mutex_unlock(&stlight.mutex);
			break;
		}
		case SWITCH_OFF: {
			pthread_mutex_lock(&stlight.mutex);
			printf("Switching OFF #%d\n", (int)cmd.lamp);
			digitalWrite(lc->lamps[cmd.lamp], HIGH);
			stlight.lamps[cmd.lamp] = 0;
			pthread_mutex_unlock(&stlight.mutex);
			break;
		}
		case SET_STATUS: {
			struct status_data sdata;

			if (!tcp_client_recv(client, &sdata, sizeof(struct status_data))) {
				pthread_mutex_lock(&stlight.mutex);
				log_local("Fail receiving status data", LOG_ERROR);
				pthread_mutex_unlock(&stlight.mutex);
				return;
			}
			pthread_mutex_lock(&stlight.mutex);
			printf("New status getting:");
			for (size_t i = 0; i < 8; i++) {
				stlight.lamps[i] = sdata.lamps[i];
				printf("L%u=%u ", i, sdata.lamps[i]);
			}
			printf("\n");
			pthread_mutex_unlock(&stlight.mutex);
			break;
		}
		case GET_STATUS: {
			struct status_data sdata;
			
			pthread_mutex_lock(&stlight.mutex);
			printf("New status getting:");
			for (size_t i = 0; i < 8; i++) {
				sdata.lamps[i] = stlight.lamps[i];
				printf("L%u=%u ", i, stlight.lamps[i]);
			}
			printf("\n");				
			pthread_mutex_unlock(&stlight.mutex);

			if (!tcp_client_send(client, &sdata, sizeof(struct status_data))) {
				pthread_mutex_lock(&stlight.mutex);
				log_local("Fail receiving status data", LOG_ERROR);
				pthread_mutex_unlock(&stlight.mutex);
				return;
			}
			break;
		}
	}
}

static void accept_error(void *data)
{
	pthread_mutex_lock(&stlight.mutex);
	log_local("Fail accepting new client", LOG_ERROR);
	pthread_mutex_lock(&stlight.mutex);
}

static void* timer_thread(void *data)
{
	char time[TIME_SIZE];
	struct move_detect *md = (struct move_detect *)data;
	struct lamp_cfg *lc = configs_get_lamps();
	
	puts("Starting move detect timer...");

	for (;;) {
        	struct timeval tv = {1, 0};
	        if (select(0, NULL, NULL, NULL, &tv) == -1)
         		if (EINTR == errno)
                		continue;

		if (stlight.time_mov1 > 0) {
			unsigned h;
			time_now(time);
			char *hour = strtok(time, ":");
			sscanf(hour, "%u", &h);
			stlight.time_mov1--;

			if (h >= 21 || h <= 7) {
				pthread_mutex_lock(&stlight.mutex);
				printf("Moving! Switch on light # %d\n", lc->lamps[md->lamp1]);
				digitalWrite(lc->lamps[md->lamp1], LOW);
				stlight.lamps[md->lamp1] = 1;
				pthread_mutex_unlock(&stlight.mutex);
			}
		} else {
			pthread_mutex_lock(&stlight.mutex);
			digitalWrite(lc->lamps[md->lamp1], HIGH);
			stlight.lamps[md->lamp1] = 0;
			pthread_mutex_unlock(&stlight.mutex);
		}

		if (stlight.time_mov2 > 0) {
			unsigned h;
			time_now(time);
			char *hour = strtok(time, ":");
			sscanf(hour, "%u", &h);
			stlight.time_mov2--;

			if (h >= 21 || h <= 7) {
				pthread_mutex_lock(&stlight.mutex);
				printf("Moving! Switch on light # %d\n", lc->lamps[md->lamp2]);
				digitalWrite(lc->lamps[md->lamp2], LOW);
				stlight.lamps[md->lamp2] = 1;
				pthread_mutex_unlock(&stlight.mutex);
			}
		} else {
			pthread_mutex_lock(&stlight.mutex);
			digitalWrite(lc->lamps[md->lamp2], HIGH);
			stlight.lamps[md->lamp2] = 0;
			pthread_mutex_unlock(&stlight.mutex);
		}
	}
	return NULL;
}

static void* move_detect_thread(void *data)
{
	struct move_detect *md = (struct move_detect *)data;

	puts("Starting move detect server...");	
	pthread_mutex_lock(&stlight.mutex);
	pinMode(md->mov1, INPUT);
	pinMode(md->mov2, INPUT);
	pthread_mutex_unlock(&stlight.mutex);
	for (;;) {
		pthread_mutex_lock(&stlight.mutex);
		int mov1 = digitalRead(md->mov1);
		int mov2 = digitalRead(md->mov2);
		pthread_mutex_unlock(&stlight.mutex);

		if (mov1 == 1) {
			pthread_mutex_lock(&stlight.mutex);
			stlight.time_mov1 = MOVE_TIMEOUT;
			pthread_mutex_unlock(&stlight.mutex);
		}
		if (mov2 == 1) {
			pthread_mutex_lock(&stlight.mutex);
			stlight.time_mov2 = MOVE_TIMEOUT;
			pthread_mutex_unlock(&stlight.mutex);
		}
		delay(500);
	}
	return NULL;
}

void move_detect_start(void)
{
	pthread_t md_th;
	pthread_t tmr_th;
	struct move_detect *md = configs_get_md();

	pthread_create(&md_th, NULL, &move_detect_thread, (void *)md);
	pthread_create(&tmr_th, NULL, &timer_thread, (void *)md);

	pthread_detach(md_th);
	pthread_detach(tmr_th);
}

bool stlight_start(void)
{
	struct server_cfg *sc = configs_get_server();
	struct lamp_cfg *lc = configs_get_lamps();

	puts("Starting street light server...");
	pthread_mutex_init(&stlight.mutex, NULL);
	for (size_t i = 0; i < 8; i++) {
		stlight.lamps[i] = 0;
		pinMode(lc->lamps[i], OUTPUT);
		digitalWrite(lc->lamps[i], HIGH);
	}

	tcp_server_set_newsession_cb(&stlight.server, new_session, NULL);
	tcp_server_set_accepterr_cb(&stlight.server, accept_error, NULL);
	if (!tcp_server_bind(&stlight.server, sc->port, sc->max_users)) {
		log_local("Fail starting StreetLight server", LOG_ERROR);
		return false;
	}
	pthread_mutex_destroy(&stlight.mutex);
	return true;
}
