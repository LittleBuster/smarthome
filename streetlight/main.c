/* SmartHome: Street Light application
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
#include "stlight.h"
#include "stlight.h"
#include "movetime.h"
#include <stdio.h>
#include <string.h>
#include <wiringPi.h>


int main(void)
{
	uint8_t ret_val;

	wiringPiSetup();
	if (!log_set_path("/var/log/stlight.log")) {
		puts("Fail setting log path. Path is to long.");
		return -1;
	}
	if (!move_time_load("/etc/mtime.conf")) {
		log_local("Fail loading move time conf. Created new.", LOG_ERROR);
		return -1;
	}
	ret_val = configs_load("/etc/stlight.conf");
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
	move_detect_start();
	return stlight_start();
}
