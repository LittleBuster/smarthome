/* SmartHome: CAM server library
 *
 * Copyright (C) 2016 Sergey Denisov.
 * Written by Sergey Denisov aka LittleBuster (DenisovS21@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public Licence
 * as published by the Free Software Foundation; either version 3
 * of the Licence, or (at your option) any later version.
 */

#ifndef __CAM_H__
#define __CAM_H__

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Setting log path for CAM module
 * @filename: path to log file
 *
 * returns false: if path is to long
 * returns true: if ok
 */
bool cam_set_log(const char *filename);


enum cam_error_codes {
    CAM_CFG_FILE_NOT_FOUND,
    CAM_CFG_PARSE_ERR,
    CAM_CFG_OK
};

/**
 * Loading configs of CAM module
 * @filename: path to configs file
 *
 * returns CFG_OK: if reading ok
 * returns cfg_err codes: if fail reading
 */
uint8_t cam_load_configs(const char *filename);

/**
 * Getting photo from camera
 * @cam_num: number of camera
 *
 * returns true: if getting photo ok
 * returns false: if fail getting photo
 */
bool cam_get_photo(uint8_t cam_num);


#ifdef __cplusplus
}
#endif

#endif
