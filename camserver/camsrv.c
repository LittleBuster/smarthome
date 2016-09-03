/* Smart Home: CAM server
 *
 * Copyright (C) 2016 Sergey Denisov.
 * Written by Sergey Denisov aka LittleBuster (DenisovS21@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence 3
 * as published by the Free Software Foundation; either version 3
 * of the Licence, or (at your option) any later version.
 */

#include "camsrv.h"
#include "configs.h"
#include "log.h"
#include "tcpserver.h"
#include "tcpclient.h"
#include "filetransfer.h"
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>


static struct {
	pthread_mutex_t mutex;
	struct tcp_server server;
} cam;


static void new_session(struct tcp_client *s_client, void *data)
{
	struct command cmd;
	struct command answ;
	struct file_transfer ftransfer;

	if (!tcp_client_recv(s_client, &cmd, sizeof(struct command))) {
		pthread_mutex_lock(&cam.mutex);
		log_local("Fail receiving cmd.", LOG_ERROR);
		pthread_mutex_unlock(&cam.mutex);
		return;
	}
	if (cmd.code != GET_PHOTO) {
		answ.code = PHOTO_FAIL;
		if (!tcp_client_send(s_client, &answ, sizeof(struct command))) {
			pthread_mutex_lock(&cam.mutex);
			log_local("Fail sending answ.", LOG_ERROR);
			pthread_mutex_unlock(&cam.mutex);
		}
		return;
	}

	CvCapture* capture = cvCreateCameraCapture(0);
	if (capture == NULL) {
		answ.code = PHOTO_FAIL;
		if (!tcp_client_send(s_client, &answ, sizeof(struct command))) {
			pthread_mutex_lock(&cam.mutex);
			log_local("Fail sending answ.", LOG_ERROR);
			pthread_mutex_unlock(&cam.mutex);
			return;
		}
	}
	
	IplImage* frame = cvQueryFrame( capture );
	if (frame == NULL) {
		answ.code = PHOTO_FAIL;
		if (!tcp_client_send(s_client, &answ, sizeof(struct command))) {
			pthread_mutex_lock(&cam.mutex);
			log_local("Fail sending answ.", LOG_ERROR);
			pthread_mutex_unlock(&cam.mutex);
			return;
		}
	}
	cvSaveImage("photo.jpg", frame, 0);
	cvReleaseImage(&frame);
	cvReleaseCapture(&capture);

	answ.code = PHOTO_OK;
	if (!tcp_client_send(s_client, &answ, sizeof(struct command))) {
		pthread_mutex_lock(&cam.mutex);
		log_local("Fail sending answ.", LOG_ERROR);
		pthread_mutex_unlock(&cam.mutex);
		return;
	}

	file_transfer_init(&ftransfer, s_client);
	if (file_transfer_send_file(&ftransfer, "photo.jpg") != FT_SEND_OK) {
		pthread_mutex_lock(&cam.mutex);
		log_local("Fail sending file.", LOG_ERROR);
		pthread_mutex_unlock(&cam.mutex);
	}
}

bool cam_server_start(void)
{
	struct server_cfg *sc = configs_get_server();

	puts("Starting CAM server...");
	pthread_mutex_init(&cam.mutex, NULL);

	tcp_server_set_newsession_cb(&cam.server, new_session, NULL);
	if (!tcp_server_bind(&cam.server, sc->port, sc->max_users)) {
		log_local("Fail binding CAM server.", LOG_ERROR);
		return false;
	}
	pthread_mutex_destroy(&cam.mutex);
	return true;
}
