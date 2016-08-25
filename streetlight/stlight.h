#ifndef __STLIGHT_H__
#define __STLIGHT_H__

#include <stdbool.h>
#include <stdint.h>

enum {
	SWITCH_ON,
	SWITCH_OFF,
	GET_STATUS,
	SET_STATUS
};

struct command {
	uint8_t code;
	unsigned lamp;
};

struct switch_answ {
	uint8_t code;
};

struct status_data {
	unsigned lamps[8]; 
};


bool stlight_start(void);


#endif
