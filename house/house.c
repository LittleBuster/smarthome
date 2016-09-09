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

#include "house.h"
#include "configs.h"
#include "log.h"
#include "tcpserver.h"
#include "tcpclient.h"
#include "meteo.h"
#include <pthread.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <ftplib.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>


static struct {
	pthread_mutex_t mutex;
	struct tcp_server server;
} house;


static void new_session(struct tcp_client *s_client, void *data)
{
	struct command cmd;

	if (!tcp_client_recv(s_client, (void *)&cmd, sizeof(struct command))) {
		pthread_mutex_lock(&house.mutex);
		log_local("Fail receiving command.", LOG_ERROR);
		pthread_mutex_unlock(&house.mutex);
		return;
	}
	switch (cmd.code) {
		case GET_METEO: {
			struct meteo_answ answ;

			meteo_get_street_data(&answ.street_temp, &answ.street_hum);
			meteo_get_street_data(&answ.room_temp, &answ.room_hum);
			meteo_get_street_data(&answ.veranda_temp, &answ.veranda_hum);
			meteo_get_street_data(&answ.second_temp, &answ.second_hum);

			if (!tcp_client_send(s_client, (const void *)&answ, sizeof(struct meteo_answ))) {
				pthread_mutex_lock(&house.mutex);
				log_local("Fail sending meteo answ.", LOG_ERROR);
				pthread_mutex_unlock(&house.mutex);
				return;
			}
			break;
		}
		case GET_CAM_PHOTO: {
			netbuf *nb;
			char filename[255];
			struct command answ;
			struct ftp_cfg *fc = configs_get_ftp();
			/*
			 * Receiving CAM number
			 */
			if (!tcp_client_recv(s_client, (void *)&cmd, sizeof(struct command))) {
				pthread_mutex_lock(&house.mutex);
				log_local("Fail receiving cam number.", LOG_ERROR);
				pthread_mutex_unlock(&house.mutex);
				return;
			}
			/*
	 		 * Getting photo fron CAM by OpenCV
	 		 */
			CvCapture* capture = cvCreateCameraCapture(cmd.code);
			if (capture == NULL) {
				answ.code = PHOTO_FAIL;
				if (!tcp_client_send(s_client, &answ, sizeof(struct command))) {
					pthread_mutex_lock(&house.mutex);
					log_local("Fail sending answ.", LOG_ERROR);
					pthread_mutex_unlock(&house.mutex);
					return;
				}
				return;
			}
	
			IplImage* frame = cvQueryFrame(capture);
			if (frame == NULL) {
				answ.code = PHOTO_FAIL;
				if (!tcp_client_send(s_client, &answ, sizeof(struct command))) {
					pthread_mutex_lock(&house.mutex);
					log_local("Fail sending answ.", LOG_ERROR);
					pthread_mutex_unlock(&house.mutex);
					return;
				}
				return;
			}
			cvSaveImage("photo.jpg", frame, 0);
			cvReleaseImage(&frame);
			cvReleaseCapture(&capture);

			/*
	 		 * Uploading photo on FTP
	 		 */
			FtpInit();
			if (!FtpConnect(fc->ip, &nb)) {
				log_local("Fail connecting to ftp server", LOG_ERROR);
				FtpQuit(nb);
				answ.code = PHOTO_FAIL;
				if (!tcp_client_send(s_client, &answ, sizeof(struct command))) {
					pthread_mutex_lock(&house.mutex);
					log_local("Fail sending answ.", LOG_ERROR);
					pthread_mutex_unlock(&house.mutex);
					return;
				}
				return;
			}
			if (!FtpLogin(fc->user, fc->passwd, nb)) {
				log_local("Fail ftp login data", LOG_ERROR);
				FtpQuit(nb);
				answ.code = PHOTO_FAIL;
				if (!tcp_client_send(s_client, &answ, sizeof(struct command))) {
					pthread_mutex_lock(&house.mutex);
					log_local("Fail sending answ.", LOG_ERROR);
					pthread_mutex_unlock(&house.mutex);
					return;
				}
				return;
			}
			char num[4];
			sprintf(num, "%u", cmd.code);
			strcpy(filename, "/video/photo");
			strcat(filename, num);
			strcat(filename, ".jpg");
			
			if (!FtpPut("photo.jpg", filename, FTPLIB_BINARY, nb)) {
				log_local("Fail uploading photo on ftp", LOG_ERROR);
				FtpQuit(nb);
				answ.code = PHOTO_FAIL;
				if (!tcp_client_send(s_client, &answ, sizeof(struct command))) {
					pthread_mutex_lock(&house.mutex);
					log_local("Fail sending answ.", LOG_ERROR);
					pthread_mutex_unlock(&house.mutex);
					return;
				}
				return;
			}
			FtpQuit(nb);

			/*
	 		 * Sending answ to client
	 		 */
			answ.code = PHOTO_OK;
			if (!tcp_client_send(s_client, &answ, sizeof(struct command))) {
				pthread_mutex_lock(&house.mutex);
				log_local("Fail sending answ.", LOG_ERROR);
				pthread_mutex_unlock(&house.mutex);
				return;
			}
			break;
		}
	}
}

bool house_server_start(void)
{
	struct server_cfg *sc = configs_get_server();

	puts("Starting House server...");
	pthread_mutex_init(&house.mutex, NULL);

	/*
	 * Meteo module init
	 */
	if (!meteo_sensors_init()) {
		log_local("Fail initialization GPIO ports.", LOG_ERROR);
		return false;
	}
	meteo_sensors_start_timer();

	/*
	 * TCP server init
	 */
	tcp_server_set_newsession_cb(&house.server, new_session, NULL);
	if (!tcp_server_bind(&house.server, sc->port, sc->max_users)) {
		log_local("Fail binding House server.", LOG_ERROR);
		return false;
	}
	pthread_mutex_destroy(&house.mutex);
	return true;
}
