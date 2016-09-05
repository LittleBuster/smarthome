/* SmartHome: StreetLight server library
 *
 * Copyright (C) 2016 Sergey Denisov.
 * Written by Sergey Denisov aka LittleBuster (DenisovS21@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public Licence
 * as published by the Free Software Foundation; either version 3
 * of the Licence, or (at your option) any later version.
 */

#include <smarthome/stlight.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


void exit_fail(const char *message)
{
	printf("[ERROR] %s\n", message);
	puts("--------------------------------------------");
	puts("[FAIL] Test crashed!");
	exit(-1);
}


int main(void)
{
	struct status_data sdata;
	
	puts("Starting Street Light test...");
	puts("--------------------------------------------");

	if (!stlight_set_log("stlight.log")) {
		exit_fail("Setting log path.");
		return -1;
	}
	puts("[OK] Setting log path.");

	if (stlight_load_configs("/etc/smarthome/stlight.conf") != ST_CFG_OK) {
		exit_fail("Loading configs.");
		return -1;
	}
	puts("[OK] Loading configs.");

	for (size_t i = 0; i < 8; i++) {
		if (!stlight_switch_on(i))
			exit_fail("Sending SWITCH_ON command.");
		printf("[OK] Sending SWITCH_ON command to #%lu.", i);
		printf("\n");
		sleep(1);
	}

	if (!stlight_get_status(&sdata))
		exit_fail("Reading lamps status.");
	printf("[OK] Reading lamps status: ");
	for (size_t i = 0; i < 8; i++)
		printf("L%lu=%u ", i, sdata.lamps[i]);
	printf("\n");

	for (size_t i = 0; i < 8; i++) {
		if (!stlight_switch_off(i))
			exit_fail("Sending SWITCH_OFF command.");
		printf("[OK] Sending SWITCH_OFF command to #%lu.", i);
		printf("\n");
		sleep(1);
	}	

	for (size_t i = 0; i < 8; i++) 
		sdata.lamps[i] = 0;

	if (!stlight_set_status(&sdata))
		exit_fail("Setting lamps status.");
	puts("[OK] Setting lamps status.");

	puts("--------------------------------------------");
	puts("[PASSED] Test finished!");
	return 0;
}
