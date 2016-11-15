#include "hstlight.h"
#include "../clients/cstlight.h"
#include <stdio.h>
#include <string.h>
#include "log.h"


void stlight_handle(const char *command, struct tcp_client *client)
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
			if (!tcp_client_send(client, (const void *)answ, strlen(answ)))
				log_local("Stlight: Fail sending answ to server", LOG_ERROR);
		}
		/*
		 * Sending json answ with array
		 */
		strcpy(answ, "HTTP/1.1 200 OK\r\n"
				"Content-Type: application/json; charset=UTF-8\r\n\r\n"
				"{\"result\": \"ok\", \"lamps\":[");
		for (size_t i = 0; i < ST_LAMPS; i++) {
			char num[2];
			
			sprintf(num, "%u", stat.lamps[i]);
			strcat(answ, num);
			if (i != ST_LAMPS-1)
				strcat(answ, ",");
		}
		strcat(answ, "]}\r\n");
		puts(answ);
		if (!tcp_client_send(client, (const void *)answ, strlen(answ)))
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
			if (!tcp_client_send(client, (const void *)answ, strlen(answ)))
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
			if (!tcp_client_send(client, (const void *)answ, strlen(answ)))
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
	puts(answ);
	if (!tcp_client_send(client, (const void *)answ, strlen(answ)))
		log_local("Stlight: Fail sending answ to server", LOG_ERROR);
}