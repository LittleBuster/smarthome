#include "htermo.h"
#include "../clients/chouse.h"
#include <stdio.h>
#include <string.h>
#include "log.h"


void termo_handle(const char *command, struct tcp_client *client)
{
	char func[50];
	char answ[255];
	char mode[10];

	if (!strcmp(command, "get_status")) {
		uint8_t status = 0;
		uint8_t heater_status = 0;

		if (!termo_get_status(&status, &heater_status)) {
			log_local("Fail sending TERMO_GET_STATUS command", LOG_ERROR);
			strcpy(answ, "HTTP/1.1 200 OK\r\n"
					"Content-Type: application/json; charset=UTF-8\r\n\r\n"
					"{\"result\": \"fail\"}\r\n");
			if (!tcp_client_send(client, (const void *)answ, strlen(answ))) {
				log_local("TERMO: Fail sending answ to server", LOG_ERROR);
				return;
			}
			return;
		}
		strcpy(answ, "HTTP/1.1 200 OK\r\n"
				"Content-Type: application/json; charset=UTF-8\r\n\r\n"
				"{\"result\": \"ok\", \"status\": ");
		if (status == 1)
			strcat(answ, "1, \"heater_status\": ");
		else
			strcat(answ, "0, \"heater_status\": ");
		if (heater_status == 1)
			strcat(answ, "1}\r\n");
		else
			strcat(answ, "0}\r\n");

		if (!tcp_client_send(client, (const void *)answ, strlen(answ))) {
			log_local("TERMO: Fail sending answ to server", LOG_ERROR);
			return;
		}
		puts(answ);
		return;
	}
	
	if (!strcmp(command, "get_temp")) {
		char ntemp[20];
		float temp;

		if (!termo_get_temp(&temp)) {
			log_local("Fail sending TERMO_GET_TEMP command", LOG_ERROR);
			strcpy(answ, "HTTP/1.1 200 OK\r\n"
					"Content-Type: application/json; charset=UTF-8\r\n\r\n"
					"{\"result\": \"fail\"}\r\n");
			if (!tcp_client_send(client, (const void *)answ, strlen(answ))) {
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
		puts(answ);
		if (!tcp_client_send(client, (const void *)answ, strlen(answ))) {
			log_local("TERMO: Fail sending answ to server", LOG_ERROR);
			return;
		}
		return;
	}	

	char *params = strtok((const char *)command, "=");
	if (params == NULL) {
		log_local("Fail parsing termo request params.", LOG_ERROR);
		return;
	}
	strcpy(func, params);
	params = strtok(NULL, "=");
	if (params == NULL) {
		log_local("Fail parsing termo request params.", LOG_ERROR);
		return;
	}
	strcpy(mode, params);

	if (!strcmp(func, "termo_set_temp")) {
		float new_temp;
		sscanf(mode, "%f", &new_temp);

		if (!termo_set_temp(new_temp)) {
			log_local("Fail sending TERMO_SET_TEMP command", LOG_ERROR);
			strcpy(answ, "HTTP/1.1 200 OK\r\n"
					"Content-Type: application/json; charset=UTF-8\r\n\r\n"
					"{\"result\": \"fail\"}\r\n");
			if (!tcp_client_send(client, (const void *)answ, strlen(answ))) {
				log_local("TERMO: Fail sending answ to server", LOG_ERROR);
				return;
			}
			return;
		}
		strcpy(answ, "HTTP/1.1 200 OK\r\n"
				"Content-Type: application/json; charset=UTF-8\r\n\r\n"
				"{\"result\": \"ok\"}\r\n");
		puts(answ);
		if (!tcp_client_send(client, (const void *)answ, strlen(answ))) {
			log_local("TERMO: Fail sending answ to server", LOG_ERROR);
			return;
		}
		return;
	}

	if (!strcmp(func, "termo_control")) {
		if (!strcmp(mode, "on")) {
			if (!termo_control_on()) {
				log_local("Fail sending TERMO_ON command", LOG_ERROR);
				strcpy(answ, "HTTP/1.1 200 OK\r\n"
						"Content-Type: application/json; charset=UTF-8\r\n\r\n"
						"{\"result\": \"fail\"}\r\n");
				if (!tcp_client_send(client, (const void *)answ, strlen(answ))) {
					log_local("TERMO: Fail sending answ to server", LOG_ERROR);
					return;
				}
				return;
			}
		}
		if (!strcmp(mode, "off")) {
			if (!termo_control_off()) {
				log_local("Fail sending TERMO_OFF command", LOG_ERROR);
				strcpy(answ, "HTTP/1.1 200 OK\r\n"
						"Content-Type: application/json; charset=UTF-8\r\n\r\n"
						"{\"result\": \"fail\"}\r\n");
				if (!tcp_client_send(client, (const void *)answ, strlen(answ))) {
					log_local("TERMO: Fail sending answ to server", LOG_ERROR);
					return;
				}
				return;
			}
		}
		strcpy(answ, "HTTP/1.1 200 OK\r\n"
					"Content-Type: application/json; charset=UTF-8\r\n\r\n"
					"{\"result\": \"ok\"}\r\n");
		puts(answ);
		if (!tcp_client_send(client, (const void *)answ, strlen(answ))) {
			log_local("TERMO: Fail sending answ to server", LOG_ERROR);
			return;
		}
	} else {
		log_local("Fail parsing TERMO request func.", LOG_ERROR);
		return;
	}
}