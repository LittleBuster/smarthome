#include "stlight.h"
#include "tcpserver.h"
#include "log.h"
#include "configs.h"
#include "pthread.h"
#include <wiringPi.h>
#include <stdio.h>
#include <string.h>


static struct {
	unsigned lamps[8];
	pthread_mutex_t mutex;
	struct tcp_server server;
} stlight;


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


bool stlight_start(void)
{
	struct server_cfg *sc = configs_get_server();
	struct lamp_cfg *lc = configs_get_lamps();

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
