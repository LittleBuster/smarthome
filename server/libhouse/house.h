/* SmartHome: house server library
 *
 * Copyright (C) 2016 Sergey Denisov.
 * Written by Sergey Denisov aka LittleBuster (DenisovS21@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public Licence
 * as published by the Free Software Foundation; either version 3
 * of the Licence, or (at your option) any later version.
 */

#ifndef __HOUSE_H__
#define __HOUSE_H__

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Setting log path for house module
 * @filename: path to log file
 *
 * returns false: if path is to long
 * returns true: if ok
 */
bool house_set_log(const char *filename);


enum house_error_codes {
    HOUSE_CFG_FILE_NOT_FOUND,
    HOUSE_CFG_PARSE_ERR,
    HOUSE_CFG_OK
};

/**
 * Loading configs of house module
 * @filename: path to configs file
 *
 * returns CFG_OK: if reading ok
 * returns cfg_err codes: if fail reading
 */
uint8_t house_load_configs(const char *filename);

/**
 * Getting photo from camera
 * @house_num: number of houseera
 *
 * returns true: if getting photo ok
 * returns false: if fail getting photo
 */
bool house_cam_get_photo(uint8_t house_num);

/**
 * Getting meteo data array
 * [0] street temp
 * [1] street hum
 * [2] room temp
 * [3] room hum
 * [4] 2nd floor temp
 * [5] 2nd floor hum
 * [6] veranda temp
 * [7] veranda hum
 *
 * returns true: if getting photo ok
 * returns false: if fail getting photo
 */
bool house_meteo_get_data(float *out_meteo);


#ifdef __cplusplus
}
#endif

#endif
