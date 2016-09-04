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


struct lamp_cfg {
	unsigned lamps[8];
};

/*
 * Get server configs
 */
struct lamp_cfg *configs_get_lamps(void);


struct move_detect {
	unsigned lamp1;
	unsigned lamp2;
	unsigned mov1;
	unsigned mov2;
	
};
/*
 * Move detect configs
 */
struct move_detect *configs_get_md(void);


#endif
