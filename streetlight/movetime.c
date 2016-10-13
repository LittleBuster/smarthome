/* SmartHome: Street Light application
 *
 * Copyright (C) 2016 Sergey Denisov.
 * Written by Sergey Denisov aka LittleBuster (DenisovS21@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version 3
 * of the Licence, or (at your option) any later version.
 */

#include "movetime.h"
#include <string.h>
#include <stdio.h>

static struct {
	char filename[255];

	struct time_on ton;

	struct time_off toff;

} mtime = {
	.ton.on_lh1 = 19,
	.ton.on_lh2 = 19,
	.toff.off_lh1 = 7,
	.toff.off_lh2 = 7
};


static bool move_time_save(void)
{
	char num[5];
	FILE *f = fopen(mtime.filename, "w");
	if (f == NULL)
		return false;

	sprintf(num, "%hhu", mtime.ton.on_lh1);
	fputs(num, f);
	sprintf(num, "%hhu", mtime.ton.on_lh2);
	fputs(num, f);
	sprintf(num, "%hhu", mtime.toff.off_lh1);
	fputs(num, f);
	sprintf(num, "%hhu", mtime.toff.off_lh2);
	fputs(num, f);

	fclose(f);
	return true;
}

bool move_time_load(const char *filename)
{
	char num[5];
	if (!strncpy(mtime.filename, filename, 254))
		return false;

	FILE *f = fopen(mtime.filename, "rb");
	if (f == NULL) {
		move_time_save();
		return false;
	}

	fgets(num, 1, f);
	sscanf(num, "%hhu", &mtime.ton.on_lh1);
	fgets(num, 1, f);
	sscanf(num, "%hhu", &mtime.ton.on_lh2);
	fgets(num, 1, f);
	sscanf(num, "%hhu", &mtime.toff.off_lh1);
	fgets(num, 1, f);
	sscanf(num, "%hhu", &mtime.toff.off_lh2);

	fclose(f);
	return true;
}

struct time_on *move_time_get_on(void)
{
	return &mtime.ton;
}

struct time_off *move_time_get_off(void)
{
	return &mtime.toff;
}

bool move_time_set_on(const struct time_on *ton)
{
	memcpy(&mtime.ton, ton, sizeof(struct time_on));
	return move_time_save();
}

bool move_time_set_off(const struct time_off *toff)
{
	memcpy(&mtime.toff, toff, sizeof(struct time_off));
	return move_time_save();
}
