#include "hhlight.h"
#include "../clients/chlight.h"
#include <stdio.h>
#include <string.h>
#include "log.h"


void hlight_handle(const char *command, struct tcp_client *client)
{
	char func[50];
	char answ[255];
	unsigned lamp;
	
	char *params = strtok((const char *)command, "=");
	if (params == NULL) {
		log_local("Hlight: Fail parsing request params.", LOG_ERROR);
		return;
	}
	/*
	 * Calling get status command
	 */
	if (!strcmp(params, "get_status")) {
		struct hl_status_data stat;

		if (!hlight_get_status(&stat)) {
			log_local("Hlight: Fail sending GET_STATUS command", LOG_ERROR);
			strcpy(answ, "HTTP/1.1 200 OK\r\n"
					"Content-Type: application/json; charset=UTF-8\r\n\r\n"
					"{\"result\": \"fail\"}\r\n");
			if (!tcp_client_send(client, (const void *)answ, strlen(answ)))
				log_local("Hlight: Fail sending answ to server", LOG_ERROR);
		}
		/*
		 * Sending json answ with array
		 */
		strcpy(answ, "HTTP/1.1 200 OK\r\n"
				"Content-Type: application/json; charset=UTF-8\r\n\r\n"
				"{\"result\": \"ok\", \"lamps\":[");
		for (size_t i = 0; i < HL_LAMPS; i++) {
			char num[2];
			
			sprintf(num, "%u", stat.lamps[i]);
			strcat(answ, num);
			if (i != HL_LAMPS-1)
				strcat(answ, ",");
		}
		strcat(answ, "]}\r\n");
		puts(answ);
		if (!tcp_client_send(client, (const void *)answ, strlen(answ)))
			log_local("Hlight: Fail sending get_status answ to server", LOG_ERROR);
		return;
	}
	/*
	 * If not get status
	 */	
	strcpy(func, params);
	params = strtok(NULL, "=");
	if (params == NULL) {
		log_local("Hlight: Fail parsing request params.", LOG_ERROR);
		return;
	}
	sscanf(params, "%u", &lamp);

	/*
	 * Switching on lamp
	 */
	if (!strcmp(func, "switch_on")) {
		if (!hlight_switch_on(lamp)) {
			log_local("Hlight: Fail sending SWITCH_ON command", LOG_ERROR);
			strcpy(answ, "HTTP/1.1 200 OK\r\n"
					"Content-Type: application/json; charset=UTF-8\r\n\r\n"
					"{\"result\": \"fail\"}\r\n");
			if (!tcp_client_send(client, (const void *)answ, strlen(answ)))
				log_local("Hlight: Fail sending answ to server", LOG_ERROR);
		}
		printf("Hlight: Switch ON #%u sended.\n", lamp);
	}

	/*
	 * Switching off lamp
	 */
	if (!strcmp(func, "switch_off")) {
		if (!hlight_switch_off(lamp)) {
			log_local("Hlight: Fail sending SWITCH_OFF command", LOG_ERROR);
			strcpy(answ, "HTTP/1.1 200 OK\r\n"
					"Content-Type: application/json; charset=UTF-8\r\n\r\n"
					"{\"result\": \"fail\"}\r\n");
			if (!tcp_client_send(client, (const void *)answ, strlen(answ)))
				log_local("Hlight: Fail sending answ to server", LOG_ERROR);
		}
		printf("Hlight: Switch OFF #%u sended.\n", lamp);
	}

	/*
	 * Sending OK result
	 */
	strcpy(answ, "HTTP/1.1 200 OK\r\n"
			"Content-Type: application/json; charset=UTF-8\r\n\r\n"
			"{\"result\": \"ok\"}\r\n");
	puts(answ);
	if (!tcp_client_send(client, (const void *)answ, strlen(answ)))
		log_local("Hlight: Fail sending answ to server", LOG_ERROR);
}