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

#ifndef __CONFIGS_H__
#define __CONFIGS_H__

#include <stdbool.h>
#include <stdint.h>


enum error_codes {
	CFG_FILE_NOT_FOUND,
	CFG_PARSE_ERR,
	CFG_OK
};

/**
 * Loading configs from file to RAM
 * @filename: name of configs file
 *
 * returns error codes: if fail loading
 * returns CFG_OK: if succeful loading
 */
uint8_t configs_load(const char *filename);


struct server_cfg {
	unsigned port;
	unsigned max_users;
};

/*
 * Get server configs
 */
struct server_cfg *configs_get_server(void);


struct ftp_cfg {
	char ip[16];
	char user[50];
	char passwd[50];
};

/*
 * Get FTP configs
 */
struct ftp_cfg *configs_get_ftp(void);


struct security_cfg {
	unsigned move;
	unsigned door;

	char sms_id[50];
	char sms_phone[50];
};

/*
 * Security configs
 */
struct security_cfg *configs_get_security(void);


struct termo_cfg {
	unsigned warm;
};

/*
 * Get termo control configs
 */
struct termo_cfg *configs_get_termo(void);


#endif
