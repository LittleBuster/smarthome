#ifndef __STLIGHT_H__
#define __STLIGHT_H__

#include <stdbool.h>

enum {
	SWITCH_ON,
	SWITCH_OFF,
	GET_STATUS,
	SET_STATUS
};

struct command {
	char code;
	char lamp;
};

struct switch_answ {
	char code;
};

struct status_data {
	char lamps[8]; 
};


bool stlight_start(void);


#endif
