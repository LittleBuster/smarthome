/* Smart Home: security house key
 *
 * Copyright (C) 2016 Sergey Denisov.
 * Rewritten by Sergey Denisov aka LittleBuster (DenisovS21@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version 3
 * of the Licence, or (at your option) any later version.
 *
 * Original library written by admin@kibermaster.net (http://kibermaster.net)
 */

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "onewire.h"
#include "uart.h"

#define LED_RED 1
#define LED_GREEN 2

static uint8_t is_open = 0;
static uint8_t key1[8] = {0, 0, 0, 0, 14, 0, 0, 0};
static uint8_t key2[8] = {0, 0, 0, 0, 14, 0, 0, 0};


static uint8_t check_key(const uint8_t *key)
{
	for (size_t i = 0; i < 8; i++) {
	    if (key[i] == key1[i])
	      continue;
	    if (key[i] == key2[i])
	      continue;
	    return 0;
	}
	return 1;
}

int main(void)
{
	uint8_t addr[8];

	DDRB |= (LED_RED << 1);
	DDRB |= (LED_GREEN << 1);
	PORTB |= (LED_RED << 1);
	PORTB &= ~(LED_GREEN << 1);
	uart_init();

	for (;;) {
		if (!is_open)
		    uart_send("SECURE$CLOSE\n");
		else
		    uart_send("SECURE$OPEN\n");

		if (onewire_read_rom(addr) == 0) {
			_delay_ms(1000);
		    continue;
		}

		if (!check_key(addr))
			continue;

		if (!is_open) {
			is_open = 1;
			PORTB |= (LED_GREEN << 1);
			PORTB &= ~(LED_RED << 1);
		}
		else {
			is_open = 0;
			PORTB |= (LED_RED << 1);
			PORTB &= ~(LED_GREEN << 1);
		}
		_delay_ms(1000);
	}
	return 0;
}
