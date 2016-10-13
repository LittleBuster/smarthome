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

#include <stdint.h>
#include <pthread.h>

/*
 * Starting security module
 */
void security_start(pthread_mutex_t *mutex);

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
void security_get_status(uint8_t *status);


#endif
