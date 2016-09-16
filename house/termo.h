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

#ifndef __TERMO_H__
#define __TERMO_H__


/*
 * Starting termo control module
 */
void termo_start(void);

/*
 * Termo control on
 */
void termo_auto_on(void);

/*
 * Termo control off
 */
void termo_auto_off(void);


#endif