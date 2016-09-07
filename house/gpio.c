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

#include <errno.h>
#include <sched.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include "gpio.h"


#define BASE 0x20000000
#define GPIO_BASE (BASE + 0x200000)
#define GPIO_LENGTH 4096

static struct {
    volatile uint32_t *m_gpio;
} pins = {
    .m_gpio = NULL
};


bool gpio_init(void)
{
    int fd;

    if (pins.m_gpio != NULL)
        return false;

    fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (fd == -1)
        return false;

    pins.m_gpio = (uint32_t *)mmap(NULL, GPIO_LENGTH, PROT_READ | PROT_WRITE, MAP_SHARED, fd, GPIO_BASE);
    close(fd);
    if (pins.m_gpio == MAP_FAILED) {
        pins.m_gpio = NULL;
        return false;
    }
    return true;
}

void gpio_set_input(const int gpio)
{
    *(pins.m_gpio + ((gpio) / 10)) &= ~(7 << (((gpio) % 10) * 3));
}

void gpio_set_output(const int gpio)
{
    gpio_set_input(gpio);
    *(pins.m_gpio + ((gpio) / 10)) |=  (1 << (((gpio) % 10) * 3));
}

void gpio_set_high(const int gpio)
{
    *(pins.m_gpio + 7) = 1 << gpio;
}

void gpio_set_low(const int gpio)
{
    *(pins.m_gpio + 10) = 1 << gpio;
}

uint32_t gpio_read(const int gpio)
{
    return *(pins.m_gpio + 13) & (1 << gpio);
}

void gpio_wait_millis(uint32_t millis)
{
    struct timeval deltatime;
    struct timeval walltime;
    struct timeval endtime;

    deltatime.tv_sec = millis / 1000;
    deltatime.tv_usec = (millis % 1000) * 1000;
    /* Get current time and add delay to find end time. */
    gettimeofday(&walltime, NULL);
    timeradd(&walltime, &deltatime, &endtime);
    /* Tight loop to waste time (and CPU) until enough time as elapsed. */
    while (timercmp(&walltime, &endtime, <))
        gettimeofday(&walltime, NULL);
}

void gpio_sleep_millis(uint32_t millis)
{
    struct timespec sleep;

    sleep.tv_sec = millis / 1000;
    sleep.tv_nsec = (millis % 1000) * 1000000L;
    while (clock_nanosleep(CLOCK_MONOTONIC, 0, &sleep, &sleep) && errno == EINTR);
}

void gpio_max_priority(void)
{
    struct sched_param sched;

    memset(&sched, 0, sizeof(sched));
    /* Use FIFO scheduler with highest priority for the lowest chance of the
     * kernel context switching
     */
    sched.sched_priority = sched_get_priority_max(SCHED_FIFO);
    sched_setscheduler(0, SCHED_FIFO, &sched);
}

void gpio_default_priority(void)
{
    struct sched_param sched;

    memset(&sched, 0, sizeof(sched));
    /* Go back to default scheduler with default 0 priority. */
    sched.sched_priority = 0;
    sched_setscheduler(0, SCHED_OTHER, &sched);
}
