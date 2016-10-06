/* SmartHome: HomeLight server library
 *
 * Copyright (C) 2016 Sergey Denisov.
 * Written by Sergey Denisov aka LittleBuster (DenisovS21@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public Licence
 * as published by the Free Software Foundation; either version 3
 * of the Licence, or (at your option) any later version.
 */

#include "log.h"
#include <stdio.h>
#include <string.h>
#include "utils.h"

#define PATH_SZIE 255

static struct {
	char path[PATH_SZIE];
} hl_log;


bool hl_log_set_path(const char *path)
{
	if (strlen(path) >= PATH_SZIE)
		return false;
	strncpy(hl_log.path, path, PATH_SZIE);
	return true;
}

bool hl_log_local(const char *message, unsigned log_type)
{
	FILE *file;
	char out_msg[255];
	char date[DATE_SIZE];	
	char tm[TIME_SIZE];	

	date_now(date);
	time_now(tm);

	strcpy(out_msg, "[");
	strcat(out_msg, date);
	strcat(out_msg, "][");
	strcat(out_msg, tm);
	strcat(out_msg, "][");

	switch(log_type) {
		case HL_LOG_ERROR: {
			strcat(out_msg, "ERROR] ");
			break;
		}
		case HL_LOG_WARNING: {
			strcat(out_msg, "WARNING] ");
			break;
		}
		case HL_LOG_INFO: {
			strcat(out_msg, "INFO] ");
			break;
		}
	}
	strcat(out_msg, message);
	strcat(out_msg, "\n");
	printf("%s", out_msg);

	file = fopen(hl_log.path, "a");
	if (file == NULL)
		return false;
	if (!fputs(out_msg, file)) {
		fclose(file);
		return false;
	}
	fclose(file);
	return true;
}
