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

#ifndef __SECURITY_H__
#define __SECURITY_H__

#include <stdbool.h>

/*
 * Starting security module
 */
void security_start(void);

/*
 * On security
 */
void security_set_on(void);

/*
 * Off security
 */
void security_set_off(void);

/*
 * Get On/Off status
 */
bool security_get_status(void);


#endif