#ifndef __HANDLE_METEO_H__
#define __HANDLE_METEO_H__

#include "../tcpclient.h"


void meteo_handle(const char *command, struct tcp_client *client);


#endif