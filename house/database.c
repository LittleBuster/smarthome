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

#include <stdio.h>
#include <stdlib.h>
#include "database.h"
#include <string.h>


bool database_load(struct database *db, const char *filename)
{
	int ret;

	ret = sqlite3_open(filename, &db->base);
	if (ret)
		return false;
	return true;
}

bool database_add_meteo(struct database *db, float temp, float hum, uint8_t room)
{
	int ret;
	char num[10];
	char sql[1024];
	char tnow[TIME_SIZE];
	char dnow[DATE_SIZE];

	strcpy(sql, "INSERT INTO ");
	if (room == DB_STREET)
		strcat(sql, "street");
	if (room == DB_MAIN_ROOM)
		strcat(sql, "room");

	strcat(sql, "(temp, hum, now_date, now_time) VALUES (");

	sprintf(num, "%.1f", temp);
	strcat(sql, num);
	strcat(sql, ", ");

	sprintf(num, "%.1f", hum);
	strcat(sql, num);
	strcat(sql, ", \"");

	time_now(tnow);
	date_now(dnow);

	strcat(sql, dnow);
	strcat(sql, "\", \"");
	strcat(sql, tnow);
	strcat(sql, "\")");

	ret = sqlite3_exec(db->base, sql, NULL, NULL, NULL);
	if(ret != SQLITE_OK)
		return false;
	return true;
}

struct meteo_list *database_meteo_day(struct database *db, uint8_t room)
{
	int ret;
	sqlite3_stmt* stmt;
	struct meteo_list *mlist = NULL;
    char sql[1024];

    if (room == DB_MAIN_ROOM)
    	strncpy(sql, "SELECT * FROM room WHERE date(now_date) >= date('now','-0 days')", 1024);
    if (room == DB_STREET)
    	strncpy(sql, "SELECT * FROM street WHERE date(now_date) >= date('now','-0 days')", 1024);

    do
    {
        sqlite3_prepare(db->base, sql, -1, &stmt, 0);
        while(sqlite3_step(stmt) == SQLITE_ROW)
        { 
        	struct meteo *mdata = (struct meteo *)malloc(sizeof(struct meteo));

        	mdata->temp = sqlite3_column_double(stmt, 1);
        	mdata->hum = sqlite3_column_double(stmt, 2);
        	strncpy(mdata->date, (const char *)sqlite3_column_text(stmt, 3), DATE_SIZE);
        	strncpy(mdata->time, (const char *)sqlite3_column_text(stmt, 4), TIME_SIZE);

        	mlist = meteo_list_append(mlist, mdata);
        }
        ret = sqlite3_finalize(stmt);
    }
    while(ret == SQLITE_SCHEMA);
    return mlist;
}

void database_close(struct database *db)
{
	sqlite3_close(db->base);
}
