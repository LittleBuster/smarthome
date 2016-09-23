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

#include "termotemp.h"
#include <string.h>
#include <stdio.h>

static struct {
	char filename[255];

	float temp;
} ttemp = {
	.temp = 22.0f
};


static bool termo_temp_save(void)
{
	char num[20];

	FILE *f = fopen(ttemp.filename, "w");
	if (f == NULL)
		return false;

	sprintf(num, "%f", ttemp.temp);
	fputs(num, f);

	fclose(f);
	return true;
}

bool termo_temp_load(const char *filename)
{
	char num[20];
	if (!strncpy(ttemp.filename, filename, 254))
		return false;

	FILE *f = fopen(ttemp.filename, "r");
	if (f == NULL) {
		termo_temp_save();
		return false;
	}

	fgets(num, 1, f);
	sscanf(num, "%f", &ttemp.temp);

	fclose(f);
	return true;
}

float termo_temp_get_temp(void)
{
	return ttemp.temp;
}

void termo_temp_set_temp(const float temp)
{
	ttemp.temp = temp;
	termo_temp_save();
}
