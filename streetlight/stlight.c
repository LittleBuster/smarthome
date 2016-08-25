#include "stlight.h"
#include "tcpserver.h"
#include "log.h"
#include "configs.h"
#include "pthread.h"
#include <wiringPi.h>
#include <string.h>


static struct {
	char lamps[8];
	pthread_mutex_t mutex;
	struct tcp_server server;
} stlight;


static void debug_msg(const char *message)
{
	pthread_mutex_lock(&stlight.mutex);
	printf(message);
	pthread_mutex_unlock(&stlight.mutex);
}

static void debug_ok(void)
{
	pthread_mutex_lock(&stlight.mutex);
	puts("OK.");
	pthread_mutex_unlock(&stlight.mutex);
}

static void debug_fail(const char *message)
{
	pthread_mutex_lock(&stlight.mutex);
	puts("FAIL.");
	log_local(message, LOG_ERROR);
	pthread_mutex_unlock(&stlight.mutex);
}

static void new_session(struct tcp_client *client, void *data)
{
	struct command cmd;
	struct lamp_cfg *lc = configs_get_lamps();

	printf("Reading client cmd...");

	if(!tcp_client_recv(client, &cmd, sizeof(struct command))) {
		pthread_mutex_lock(&stlight.mutex);
		puts("FAIL.");
		log_local("Fail reading client command");
		pthread_mutex_unlock(&stlight.mutex);
		return;
	}
	puts("OK");

	switch (cmd.code) {
		case SWITCH_ON: {
			pthread_mutex_lock(stlight.mutex);
			printf("Switching ON #%d\n", (int)cmd.lamp);
			digitalWrite(lc->lamps[cmd.lamp], HIGH);
			stlight.lamps[cmd.lamp] = 1;
			pthread_mutex_unlock(stlight.mutex);
			break;
		}
		case SWITCH_OFF: {
			pthread_mutex_lock(stlight.mutex);
			printf("Switching OFF #%d\n", (int)cmd.lamp);
			digitalWrite(lc->lamps[cmd.lamp], LOW);
			stlight.lamps[cmd.lamp] = 0;
			pthread_mutex_unlock(stlight.mutex);
			break;
		}
		case GET_STATUS: {

			break;
		}
		case SET_STATUS: {
			break;
		}
	}
}

static void accept_error(void *data)
{
}


bool stlight_start(void)
{
	struct server_cfg *sc = configs_get_server();

	pthread_mutex_init(&stlight.mutex, NULL);	

	tcp_server_set_newsession_cb(&stlight.server, new_session, NULL);
	tcp_server_set_accepterr_cb(&stlight.server, accept_error, NULL);
	if (!tcp_server_bind(&stlight.server, sc->port, sc->max_users)) {
		log_local("Fail starting StreetLight server", LOG_ERROR);
		return false;
	}
	pthread_mutex_destroy(&stlight.mutex);
	return true;
}
