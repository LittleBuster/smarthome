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


#ifndef __DATABASE_H__
#define __DATABASE_H__

#include <stdbool.h>
#include <stdint.h>
#include <sqlite3.h>
#include "utils.h"
#include "mlist.h"


enum room {
	DB_STREET,
	DB_MAIN_ROOM
};

struct database {
	sqlite3 *base;
};

/**
 * Loading database from file
 * @db: database struct
 * @filename: file of database
 *
 * returns true: if loading ok
 * returns false: if fail loading
 */
bool database_load(struct database *db, const char *filename);

/**
 * Adding meteo data to base
 * @db: database struct
 * @temp: temperature
 * @hum: humidity
 * @room: place of sensor
 *
 * returns true: if adding ok
 * returns false: if adding fail
 */
bool database_add_meteo(struct database *db, float temp, float hum, uint8_t room);

/**
 * Getting all day temperatures and humidities
 * @db: database struct
 * @room: place of sensor
 *
 * returns: meteo data list
 */
struct meteo_list *database_meteo_day(struct database *db, uint8_t room);

/*
 * Closing base
 */
void database_close(struct database *db);


#endif