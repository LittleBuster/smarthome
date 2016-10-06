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

#ifndef __STLIGHT_H__
#define __STLIGHT_H__

#include <stdbool.h>
#include <stdint.h>


#define MOVE_TIMEOUT 60 //5 min


void move_detect_start(void);


bool stlight_start(void);


#endif
