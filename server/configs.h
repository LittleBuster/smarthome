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

#ifndef __CONFIGS_H__
#define __CONFIGS_H__

#include <stdbool.h>
#include <stdint.h>


/**
 * Loading configs from file to RAM
 * @filename: name of configs file
 *
 * returns error codes: if fail loading
 * returns CFG_OK: if succeful loading
 */
uint8_t wconfigs_load(const char *filename);


struct server_cfg {
	unsigned port;
	unsigned max_users;
};

/*
 * Get server configs
 */
struct server_cfg *wconfigs_get_server(void);


#endif
