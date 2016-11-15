#include "hsecurity.h"
#include "../clients/chouse.h"
#include <stdio.h>
#include <string.h>
#include "log.h"


void security_handle(const char *command, struct tcp_client *client)
{
	char func[50];
	char answ[255];

	if (!strcmp(command, "get_status")) {
		uint8_t stat;

		if (security_get_status(&stat)) {
			log_local("Fail sending SECURITY_SET_ON command", LOG_ERROR);

			strcpy(answ, "HTTP/1.1 200 OK\r\n"
					"Content-Type: application/json; charset=UTF-8\r\n\r\n"
					"{\"result\": \"fail\"}\r\n");

			if (!tcp_client_send(client, (const void *)answ, strlen(answ))) {
				log_local("Security: Fail sending on answ to server", LOG_ERROR);
				return;
			}
			return;
		}
		if (stat) {
			strcpy(answ, "HTTP/1.1 200 OK\r\n"
						"Content-Type: application/json; charset=UTF-8\r\n\r\n"
						"{\"result\": \"ok\", \"status\": 1}\r\n");
		} else {
			strcpy(answ, "HTTP/1.1 200 OK\r\n"
						"Content-Type: application/json; charset=UTF-8\r\n\r\n"
						"{\"result\": \"ok\", \"status\": 0}\r\n");
		}
		puts(answ);
		if (!tcp_client_send(client, (const void *)answ, strlen(answ))) {
			log_local("Security: Fail sending on answ to server", LOG_ERROR);
			return;
		}
		return;
	}

	char *params = strtok((const char *)command, "=");
	if (params == NULL) {
		log_local("Fail parsing Security request params.", LOG_ERROR);
		return;
	}
	strcpy(func, params);
	params = strtok(NULL, "=");
	if (params == NULL) {
		log_local("Fail parsing Security request params.", LOG_ERROR);
		return;
	}
	if (!strcmp(func, "mode")) {
		if (!strcmp(params, "on")) {
			if (security_set_on()) {
				log_local("Fail sending SECURITY_SET_ON command", LOG_ERROR);

				strcpy(answ, "HTTP/1.1 200 OK\r\n"
						"Content-Type: application/json; charset=UTF-8\r\n\r\n"
						"{\"result\": \"fail\"}\r\n");
				if (!tcp_client_send(client, (const void *)answ, strlen(answ))) {
					log_local("Security: Fail sending on answ to server", LOG_ERROR);
					return;
				}
				return;
			}
		}
		if (!strcmp(params, "off")) {
			if (security_set_off()) {
				log_local("Fail sending SECURITY_SET_OFF command", LOG_ERROR);

				strcpy(answ, "HTTP/1.1 200 OK\r\n"
						"Content-Type: application/json; charset=UTF-8\r\n\r\n"
						"{\"result\": \"fail\"}\r\n");
				if (!tcp_client_send(client, (const void *)answ, strlen(answ))) {
					log_local("Security: Fail sending off answ to server", LOG_ERROR);
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
			log_local("Security: Fail sending answ to server", LOG_ERROR);
			return;
		}
	} else {
		log_local("Fail parsing Security request func.", LOG_ERROR);
		return;
	}
}