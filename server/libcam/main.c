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
#include "cam.h"
#include <stdio.h>
#include <string.h>


int main(void)
{
	if (!cam_set_log("/var/log/cam.log")) {
		puts("Fail setting log path. Path is to long.");
		return -1;
	}/*
	ret_val = cam_load_configs("/etc/smarthome/cam.conf");
	if (ret_val != CFG_OK) {
		puts("Loading configs fail.");
		return -1;
	}*/
	
	cam_get_photo("photo.jpg", 0);
	return 0;
}
