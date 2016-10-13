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


#ifndef __METEO_LIST_H__
#define __METEO_LIST_H__

#include <stdio.h>
#include "utils.h"

struct meteo {
	float temp;
	float hum;
	char date[DATE_SIZE];
	char time[TIME_SIZE];
};

struct meteo_list {
    struct meteo *mdata;	 /* meteo data */
    struct meteo_list *next; /* next meteo data */
};


/**
 * Add new meteo data in list
 * @mlist: meteo data list struct
 * @mdata: meteo data
 *
 * returns meteo data list with added meteo data
 */
struct meteo_list *meteo_list_append(struct meteo_list *mlist, struct meteo *mdata);

/*
 * Next meteo data of list
 */
struct meteo_list *meteo_list_next(struct meteo_list *mlist);

/*
 * Getting current meteo data
 */
struct meteo *meteo_list_get_meteo(struct meteo_list *mlist);

/*
 * Getting count of meteo data
 */
unsigned meteo_list_get_count(struct meteo_list *mlist);

/*
 * Free list of meteo data
 */
void meteo_list_free_all(struct meteo_list *mlist);


#endif