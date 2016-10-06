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

#ifndef __HL_LOG_H__
#define __HL_LOG_H__

#include <stdbool.h>

enum {
	HL_LOG_ERROR,
	HL_LOG_WARNING,
	HL_LOG_INFO
};


/**
 * Setting path to log file
 * @path: path with filename
 *
 * returns true: if setted
 * returns false: if path size > max_path_size
 */
bool hl_log_set_path(const char *path);

/**
 * Writing message to local log file
 * @message: writing message
 * @log_type: type of log message
 *
 * returns true: if message writed
 * returns false: if message writing error
 */
bool hl_log_local(const char *message, unsigned log_type);


#endif
