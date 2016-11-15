#include "hcam.h"
#include "../clients/chouse.h"
#include <stdio.h>
#include <string.h>
#include "log.h"


void cam_handle(const char *command, struct tcp_client *restrict client)
{
	char func[50];
	char answ[255];
	unsigned cam;

	char *params = strtok((const char *)command, "=");
	if (params == NULL) {
		log_local("Fail parsing CAM request params.", LOG_ERROR);
		return;
	}
	strcpy(func, params);
	params = strtok(NULL, "=");
	if (params == NULL) {
		log_local("Fail parsing CAM request params.", LOG_ERROR);
		return;
	}
	sscanf(params, "%u", &cam);

	if (!strcmp(func, "get_photo")) {
		if (!cam_get_photo(cam)) {
			log_local("Fail sending GET_PHOTO command", LOG_ERROR);

			strcpy(answ, "HTTP/1.1 200 OK\r\n"
						"Content-Type: application/json; charset=UTF-8\r\n\r\n"
						"{\"result\": \"fail\"}\r\n");
			if (!tcp_client_send(client, (const void *)answ, strlen(answ))) {
				log_local("CAM: Fail sending answ to server", LOG_ERROR);
				return;
			}
			return;
		}
		strcpy(answ, "HTTP/1.1 200 OK\r\n"
					"Content-Type: application/json; charset=UTF-8\r\n\r\n"
					"{\"result\": \"ok\"}\r\n");
		puts(answ);
		if (!tcp_client_send(client, (const void *)answ, strlen(answ))) {
			log_local("CAM: Fail sending answ to server", LOG_ERROR);
			return;
		}
	} else {
		log_local("Fail parsing CAM request func.", LOG_ERROR);
		return;
	}
}