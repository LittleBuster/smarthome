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

#include "configs.h"
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>


enum {
	POS_COMMENT = -2,
	POS_NOT_FOUND = -1
};

static struct {
    char filename[255];

    struct server_cfg sc;
    struct meteo_cfg meteo;
    struct ftp_cfg fc;
    struct security_cfg sec;
    struct termo_cfg tc;
} cfg;


/*
 * Local funcs
 */
static int pos(const char *str, const char symb)
{
    int p = 0;

    while (*str) {
        /*
         * Line is comment
         */
        if (*str == '#')
            return POS_COMMENT;

        if (*str == symb)
            return p;
        str++;
        p++;
    }
    return POS_NOT_FOUND;
}

static bool parse_string(const char *str, char *out, size_t sz)
{
	int p;
    bool found = false;
    size_t len = 0;

    p = pos(str, '\"');
    if (p == POS_COMMENT)
        return false;
    if (p == POS_NOT_FOUND) {
        p = pos(str, '=');
        if (p == POS_NOT_FOUND)
            return false;
    }
    str += p + 1;

    while (*str) {
        if (*str == ' ' && !found) {
            str++;
            continue;
        }
        if (*str == '\"')
            break;
        if (len > sz)
            return false;

        found = true;
        *out = *str;
        out++;
        str++;
        len++;
    }
    *out = '\0';
    return true;
}

static bool parse_unsigned(const char *str, unsigned *out)
{
    char outs[50];

    if (!parse_string(str, outs, 50))
        return false;

    sscanf(outs, "%u", out); 
    return true;
}

static bool parse_float(const char *str, float *out)
{
    char outs[50];

    if (!parse_string(str, outs, 50))
        return false;

    sscanf(outs, "%f", out); 
    return true;
}

static bool configs_read_unsigned(FILE *restrict file, unsigned *out)
{
    char data[50];

    while (!feof(file)) {
        if (fgets(data, 50, file) == NULL)
            return false;
        if (parse_unsigned(data, out))
            return true;
    }

    return true;
}

static bool configs_read_float(FILE *restrict file, float *out)
{
    char data[50];

    while (!feof(file)) {
        if (fgets(data, 50, file) == NULL)
            return false;
        if (parse_float(data, out))
            return true;
    }
    return true;
}

static bool configs_read_string(FILE *restrict file, char *out, size_t sz)
{
    bool is_ok = false;
    char data[255];

    while (!feof(file)) {
        if (fgets(data, 255, file) == NULL)
            return false;
        if (parse_string(data, out, sz))
            return true;
    }
    if (!is_ok)
        return false;
    return true;
}

/*
 * Loading configs from file
 */
uint8_t configs_load(const char *filename)
{
    FILE *file;

    strncpy(cfg.filename, filename, 255);

    file = fopen(filename, "r");
    if (file == NULL)
        return CFG_FILE_NOT_FOUND;

    if (!configs_read_unsigned(file, &cfg.sc.port)) {
        fclose(file);
        return CFG_PARSE_ERR;
    }
    if (!configs_read_unsigned(file, &cfg.sc.max_users)) {
        fclose(file);
        return CFG_PARSE_ERR;
    }
    if (!configs_read_unsigned(file, &cfg.meteo.street)) {
        fclose(file);
        return CFG_PARSE_ERR;
    }
    if (!configs_read_unsigned(file, &cfg.meteo.room)) {
        fclose(file);
        return CFG_PARSE_ERR;
    }
    if (!configs_read_unsigned(file, &cfg.meteo.second)) {
        fclose(file);
        return CFG_PARSE_ERR;
    }
    if (!configs_read_unsigned(file, &cfg.meteo.veranda)) {
        fclose(file);
        return CFG_PARSE_ERR;
    }
    if (!configs_read_string(file, cfg.fc.ip, 15)) {
        fclose(file);
        return CFG_PARSE_ERR;
    }
    if (!configs_read_string(file, cfg.fc.user, 49)) {
        fclose(file);
        return CFG_PARSE_ERR;
    }
    if (!configs_read_string(file, cfg.fc.passwd, 49)) {
        fclose(file);
        return CFG_PARSE_ERR;
    }
    if (!configs_read_unsigned(file, &cfg.sec.move)) {
        fclose(file);
        return CFG_PARSE_ERR;
    }
    if (!configs_read_unsigned(file, &cfg.sec.door)) {
        fclose(file);
        return CFG_PARSE_ERR;
    }
    if (!configs_read_string(file, cfg.sec.sms_id, 49)) {
        fclose(file);
        return CFG_PARSE_ERR;
    }
    if (!configs_read_string(file, cfg.sec.sms_phone, 49)) {
        fclose(file);
        return CFG_PARSE_ERR;
    }
    if (!configs_read_unsigned(file, &cfg.sec.alarm_room)) {
        fclose(file);
        return CFG_PARSE_ERR;
    }
    if (!configs_read_unsigned(file, &cfg.sec.alarm_street)) {
        fclose(file);
        return CFG_PARSE_ERR;
    }
    if (!configs_read_unsigned(file, &cfg.tc.tpin)) {
        fclose(file);
        return CFG_PARSE_ERR;
    }
    fclose(file);
    return CFG_OK;
}

struct server_cfg *configs_get_server(void)
{
    return &cfg.sc;
}

struct ftp_cfg *configs_get_ftp(void)
{
    return &cfg.fc;
}

struct security_cfg *configs_get_security(void)
{
    return &cfg.sec;
}

struct termo_cfg *configs_get_termo(void)
{
    return &cfg.tc;
}

struct meteo_cfg *configs_get_meteo(void)
{
	return &cfg.meteo;
}


static bool configs_write_float(FILE *file, const char *param, const float value)
{
    fprintf(file, "%s = %.2f\n", param, value);
    return true;
}

static bool configs_write_unsigned(FILE *file, const char *param, const unsigned value)
{
    fprintf(file, "%s = %u\n", param, value);
    return true;
}


/*
 * Termo extended configs
 */
uint8_t configs_termo_load(struct termo_ext_cfg *tc, const char *filename)
{
    FILE *file;

    file = fopen(filename, "r");
    if (file == NULL)
        return CFG_FILE_NOT_FOUND;

    if (!configs_read_unsigned(file, &tc->last_state)) {
        fclose(file);
        return CFG_PARSE_ERR;
    }
    if (!configs_read_float(file, &tc->temp)) {
        fclose(file);
        return CFG_PARSE_ERR;
    }
    fclose(file);
    return CFG_OK;
}

uint8_t configs_termo_save(struct termo_ext_cfg *tc, const char *filename)
{
    FILE *file;

    file = fopen(filename, "w");
    if (file == NULL)
        return CFG_FILE_BUSY;

    fputs("*********************************\n*\n* Termocontrol extended configs\n*\n"
          "*********************************\n\n", file);
    if (!configs_write_unsigned(file, "last_state", tc->temp)) {
        fclose(file);
        return CFG_WRITE_ERR;
    }
    if (!configs_write_float(file, "temp", tc->temp)) {
        fclose(file);
        return CFG_WRITE_ERR;
    }
    fclose(file);
    return CFG_OK;
}

/*
 * Security extended configs
 */
uint8_t configs_security_load(struct security_ext_cfg *sc, const char *filename)
{
    FILE *file;

    file = fopen(filename, "r");
    if (file == NULL)
        return CFG_FILE_NOT_FOUND;

    if (!configs_read_unsigned(file, &sc->last_state)) {
        fclose(file);
        return CFG_PARSE_ERR;
    }
    fclose(file);
    return CFG_OK;
}

uint8_t configs_security_save(struct security_ext_cfg *sc, const char *filename)
{
    FILE *file;

    file = fopen(filename, "w");
    if (file == NULL)
        return CFG_FILE_BUSY;

    fputs("*********************************\n*\n* Security extended configs\n*\n"
          "*********************************\n\n", file);
    if (!configs_write_unsigned(file, "last_state", sc->last_state)) {
        fclose(file);
        return CFG_WRITE_ERR;
    }
    fclose(file);
    return CFG_OK;
}
