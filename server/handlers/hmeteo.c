#include "hmeteo.h"
#include "../clients/chouse.h"
#include <stdio.h>
#include <string.h>
#include "log.h"


void meteo_handle(const char *command, struct tcp_client *restrict client)
{
	char answ[255];

	if (!strcmp(command, "get_weather")) {
		float meteo[8];

		if (!meteo_get_data(meteo)) {
			log_local("Fail sending GET_METEO command", LOG_ERROR);

			strcpy(answ, "HTTP/1.1 200 OK\r\n"
						"Content-Type: application/json; charset=UTF-8\r\n\r\n"
						"{\"result\": \"fail\"}\r\n");
			if (!tcp_client_send(client, (const void *)answ, strlen(answ))) {
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
		puts(answ);
		if (!tcp_client_send(client, (const void *)answ, strlen(answ))) {
			log_local("METEO: Fail sending answ to server", LOG_ERROR);
		}
	} else {
		log_local("Fail parsing meteo request func.", LOG_ERROR);
		return;
	}
}