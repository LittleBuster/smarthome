/* Smart Home: House server
 *
 * Copyright (C) 2016 Sergey Denisov, Adafruit Industries.
 * Written by Sergey Denisov aka LittleBuster (DenisovS21@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence 3
 * as published by the Free Software Foundation; either version 3
 * of the Licence, or (at your option) any later version.
 */

#ifndef __PI_IO_H__
#define __PI_IO_H__

#include <stdint.h>
#include <stdbool.h>


enum io_errs {
    ERR_DEVMEM,
    ERR_MMAP
};

/**
 * IO library initialization
 *
 * returns true: if succeful map init
 * returns false: if fail init
 */
bool gpio_init(void);

/**
 * Set port in input mode
 * @gpio: port number
 */
void gpio_set_input(const int gpio);

/**
 * Set port in input mode
 * @gpio: port number
 */
void gpio_set_output(const int gpio);

/**
 * Set port in high level
 * @gpio: port number
 */
void gpio_set_high(const int gpio);

/**
 * Set port in low level
 * @gpio: port number
 */
void gpio_set_low(const int gpio);

/**
 * Reading data from port
 * @gpio: port number
 *
 * returns port value
 */
uint32_t gpio_read(const int gpio);

/**
 * Busy wait delay for most accurate timing, but high CPU usage.
 * Only use this for short periods of time (a few hundred milliseconds at most)!
 * @millis: wait milliseconds
 */
void gpio_wait_millis(uint32_t millis);

/**
 * General delay that sleeps so CPU usage is low,
 * but accuracy is potentially bad.
 * @millis: sleep milliseconds
 */
void gpio_sleep_millis(uint32_t millis);

/*
 * Increase scheduling priority and algorithm to try to get 'real time' results.
 */
void gpio_max_priority(void);

/*
 * Drop scheduling priority back to normal/default.
 */
void gpio_default_priority(void);


#endif