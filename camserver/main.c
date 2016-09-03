/* Smart Home: CAM server
 *
 * Copyright (C) 2016 Sergey Denisov.
 * Written by Sergey Denisov aka LittleBuster (DenisovS21@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence 3
 * as published by the Free Software Foundation; either version 3
 * of the Licence, or (at your option) any later version.
 */

#include "log.h"
#include "configs.h"
#include <stdio.h>
#include <string.h>
#include "camsrv.h"
#include "tcpserver.h"


int main(void)
{
	uint8_t ret_val;

	if (!log_set_path("/var/log/camsrv.log")) {
		puts("Fail setting log path. Path is to long.");
		return -1;
	}
	ret_val = configs_load("/etc/camsrv.conf");
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
	return cam_server_start();
}
