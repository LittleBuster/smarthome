/* SmartHome: server application
 *
 * Copyright (C) 2016 Sergey Denisov.
 * Written by Sergey Denisov aka LittleBuster (DenisovS21@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version 3
 * of the Licence, or (at your option) any later version.
 */

#include "log.h"
#include "configs.h"
#include "webserver.h"
#include <stdio.h>
#include <string.h>
#include <smarthome/stlight.h>

static int exit_cfg_error(const char *who, int ret_val)
{
	char msg[255];

	strcpy(msg, "Fail reading ");
	strcat(msg, who);
	strcat(msg, " configs: ");

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


int main(void)
{
	uint8_t ret_val;

	if (!log_set_path("/var/log/wserver.log")) {
		puts("Fail setting log path. Path is to long.");
		return -1;
	}
	if (!stlight_set_log("/var/log/stlight.log")) {
		puts("Fail setting log path. Path is to long.");
		return -1;
	}
	
	ret_val = configs_load("/etc/smarthome/wserver.conf");
	if (ret_val != CFG_OK)
		return exit_cfg_error("WEB server", ret_val);
	ret_val = stlight_load_configs("/etc/smarthome/stlight.conf");
	if (ret_val != CFG_OK)
		return exit_cfg_error("StreetLight", ret_val);
	
	if (!web_server_start()) {
		log_local("Fail starting web server.", LOG_ERROR);
		return -1;
	}
	return 0;
}
