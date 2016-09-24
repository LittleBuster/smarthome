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

#ifndef __CAM_H__
#define __CAM_H__

#include "tcpclient.h"
#include <pthread.h>


/**
 * Getting photo from WEB camera
 * @s_client: tcp client
 * @cam: number of WEB camera
 * @mutex: thread lock mutex
 *
 * returns true: if getting photo ok
 * returns false: if getting fail
 */
bool cam_get_photo(struct tcp_client *s_client, uint8_t cam, pthread_mutex_t *mutex);


#endif