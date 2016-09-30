/* Smart Home: House server
 *
 * Copyright (C) 2016 Sergey Denisov.
 * Written by Sergey Denisov aka LittleBuster (DenisovS21@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence 3
 * as published by the Free Software Foundation; either version 3
 * of the Licence, or (at your option) any later version.
 */

#include <stdio.h>
#include <string.h>
#include <wiringPi.h>
#include "house.h"
#include "tcpserver.h"
#include "log.h"
#include "configs.h"
#include "security.h"
#include "termo.h"


int main(void)
{
	uint8_t ret_val;

	wiringPiSetup();

	if (!log_set_path("/var/log/house.log")) {
		puts("Fail setting log path. Path is to long.");
		return -1;
	}
	if (configs_termo_load("/etc/termo.conf") != CFG_OK)
		log_local("Fail loading termo configs.", LOG_WARNING);

	ret_val = configs_load("/etc/house.conf");
	if (ret_val != CFG_OK) {
		char msg[255];

		strcpy(msg, "Fail reading configs: ");
		switch (ret_val) {
			case CFG_FILE_NOT_FOUND: {
				strcat(msg, "file not found.");
				break;
			} 
			case CFG_PARSE_ERR: {
				strcat(msg, "parsing error.");
				break;
			} 
		}
		log_local(msg, LOG_ERROR);
		return -1;
	}
	security_start();
	termo_start();
	return house_server_start();
}
