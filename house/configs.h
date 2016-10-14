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
	CFG_FILE_BUSY,
	CFG_PARSE_ERR,
	CFG_WRITE_ERR,
	CFG_OK
};

/**
 * Loading configs from file to RAM
 * @filename: name of configs file
 *
 * returns error codes: if fail loading
 * returns CFG_OK: if succeful loaded
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


struct meteo_cfg {
	unsigned street;
	unsigned room;
	unsigned second;
	unsigned veranda;
};

/*
 * Reading meteo sensors
 */
struct meteo_cfg *configs_get_meteo(void);


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

	unsigned alarm_room;
	unsigned alarm_street;
};

/*
 * Security configs
 */
struct security_cfg *configs_get_security(void);


struct termo_cfg {
	unsigned tpin;
};

/*
 * Termocontrol configs
 */
struct termo_cfg *configs_get_termo(void);


struct termo_ext_cfg {	
	unsigned last_status;
	unsigned heater_status;
	float temp;
};

/**
 * Loading termo configs from file to RAM
 * @tc: termo config struct
 * @filename: name of termo configs file
 *
 * returns error codes: if fail loading
 * returns CFG_OK: if succeful loading
 */
uint8_t configs_termo_load(struct termo_ext_cfg *tc, const char *filename);

/**
 * Saving termo configs from file to RAM
 * @tc: termo config struct
 * @filename: name of termo configs file
 *
 * returns error codes: if fail loading
 * returns CFG_OK: if succeful loading
 */
uint8_t configs_termo_save(struct termo_ext_cfg *tc, const char *filename);


struct security_ext_cfg {
	unsigned last_status;
};

/**
 * Loading security configs from file to RAM
 * @sc: security config struct
 * @filename: name of security configs file
 *
 * returns error codes: if fail loading
 * returns CFG_OK: if succeful loading
 */
uint8_t configs_security_load(struct security_ext_cfg *sc, const char *filename);

/**
 * Saving security configs from file to RAM
 * @sc: security config struct
 * @filename: name of security configs file
 *
 * returns error codes: if fail loading
 * returns CFG_OK: if succeful loading
 */
uint8_t configs_security_save(struct security_ext_cfg *sc, const char *filename);


#endif
