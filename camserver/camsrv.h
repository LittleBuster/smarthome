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

#ifndef __CAMSRV_H__
#define __CAM_SRV_H__

#include <stdbool.h>
#include <stdint.h>

enum recv_cmd {
	GET_PHOTO,
	PHOTO_OK,
	PHOTO_FAIL
};

enum cam_list {
	CAM_1 = 0,
	CAM_2 = 1
};

struct command {
	uint8_t code;
};


bool cam_server_start(void);


#endif
