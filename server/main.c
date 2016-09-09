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
#include <smarthome/house.h>


int main(void)
{
	uint8_t ret_val;

	/*
	 * Setting logs
	 */
	if (!log_set_path("/var/log/smarthome/wserver.log")) {
		puts("Fail setting SMART SERVER log path. Path is to long.");
		return -1;
	}
	if (!stlight_set_log("/var/log/smarthome/stlight.log")) {
		puts("Fail setting STREET LIGHT log path. Path is to long.");
		return -1;
	}
	if (!house_set_log("/var/log/smarthome/house.log")) {
		puts("Fail setting CAM log path. Path is to long.");
		return -1;
	}
	/*
	 * Loading configs
	 */
	ret_val = configs_load("/etc/smarthome/wserver.conf");
	if (ret_val != CFG_OK) {
		char msg[255];

		strcpy(msg, "Fail reading SMART SERVER configs: ");
		switch (ret_val) {				
			case CFG_FILE_NOT_FOUND: {
				strcat(msg, "File not found.");
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
	ret_val = stlight_load_configs("/etc/smarthome/stlight.conf");
	if (ret_val != ST_CFG_OK) {
		char msg[255];

		strcpy(msg, "Fail reading STREET LIGHT configs: ");
		switch (ret_val) {			
			case ST_CFG_FILE_NOT_FOUND: {
				strcat(msg, "File not found.");
				break;
			}
			case ST_CFG_PARSE_ERR: {
				strcat(msg, "parsing error.");
				break;
			}
		}
		log_local(msg, LOG_ERROR);
		return -1;
	}
	ret_val = house_load_configs("/etc/smarthome/house.conf");
	if (ret_val != HOUSE_CFG_OK) {
		char msg[255];

		strcpy(msg, "Fail reading STREET LIGHT configs: ");
		switch (ret_val) {			
			case HOUSE_CFG_FILE_NOT_FOUND: {
				strcat(msg, "File not found.");
				break;
			}
			case HOUSE_CFG_PARSE_ERR: {
				strcat(msg, "parsing error.");
				break;
			}
		}
		log_local(msg, LOG_ERROR);
		return -1;
	}
	
	if (!web_server_start()) {
		log_local("Fail starting smart web server.", LOG_ERROR);
		return -1;
	}
	return 0;
}
