/* OneWire AVR library
 *
 * Copyright (C) 2016 Sergey Denisov.
 * Rewritten by Sergey Denisov aka LittleBuster (DenisovS21@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public Licence
 * as published by the Free Software Foundation; either version 3
 * of the Licence, or (at your option) any later version.
 *
 * Original library written by admin@kibermaster.net (http://kibermaster.net)
 */

#include "onewire.h"
#include <util/delay.h>

#define sbi(reg,bit) reg |= (1<<bit)
#define cbi(reg,bit) reg &= ~(1<<bit)
#define ibi(reg,bit) reg ^= (1<<bit)
#define check_bit(reg,bit) (reg&(1<<bit))


static void onewire_set(uint8_t mode)
{
	if (mode) {
		cbi(OW_PORT, OW_BIT);
		sbi(OW_DDR, OW_BIT);
	}
	else {
		cbi(OW_PORT, OW_BIT);
		cbi(OW_DDR, OW_BIT);
	}
}

static uint8_t one_wire_check_in(void)
{
	return check_bit(OW_PIN, OW_BIT);
}

uint8_t onewire_reset(void)
{
	uint8_t status;

	onewire_set(1);
	_delay_us(480);
	onewire_set(0);
	_delay_us(60);
	/* Store line value and wait until the completion of 480uS period */
	status = one_wire_check_in();
	_delay_us(420);
	/* Return the value read from the presence pulse (0=OK, 1=WRONG) */
	return !status;
}

void onewire_write_bit(uint8_t bit)
{
	/* Pull line low for 1uS */
	onewire_set(1);
	_delay_us(1);
	/* If we want to write 1, release the line (if not will keep low) */
	if(bit) onewire_set(0);
	/* Wait for 60uS and release the line */
	_delay_us(60);
	onewire_set(0);
}

uint8_t onewire_read_bit(void)
{
	uint8_t bit=0;

	onewire_set(1);
	_delay_us(1);
	onewire_set(0);
	_delay_us(14);
	if(one_wire_check_in())
		bit = 1;
	_delay_us(45);

	return bit;
}

void onewire_write_byte(uint8_t byte)
{
	for (uint8_t i = 0; i < 8; i++)
		onewire_write_bit(check_bit(byte, i));
}

uint8_t onewire_read_byte(void)
{
	uint8_t n = 0;

	for (uint8_t i = 0; i < 8; i++)
		if (onewire_read_bit())
			sbi(n, i);

	return n;
}

uint8_t onewire_read_rom(uint8_t *buffer)
{
	if (!onewire_reset())
		return 0;
	onewire_write_byte(OW_CMD_READROM);

	for (uint8_t i = 0; i < 8; i++)
		buffer[i] = onewire_read_byte();

	return 1;
}
