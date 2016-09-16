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

#ifndef __TERMO_TEMP_H__
#define __TERMO_TEMP_H__

#include <stdbool.h>


/**
 * Loading configs termo temp from file
 * @filename: meteo time file
 *
 * returns true: if loading ok
 * returns false: if loading fail
 */
bool termo_temp_load(const char *filename);

/*
 * Termo control temp
 */
float termo_temp_get_temp(void);

/*
 * Termo control set temp
 */
void termo_temp_set_temp(const float temp);


#endif