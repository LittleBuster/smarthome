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

#include "cam.h"
#include "log.h"
#include "tcpclient.h"
#include "configs.h"
#include "house.h"
#include <ftplib.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdio.h>
#include <string.h>


bool cam_get_photo(struct tcp_client *s_client, uint8_t cam, pthread_mutex_t *mutex)
{
	netbuf *nb;
	char filename[255];
	struct command answ;
	struct ftp_cfg *fc = configs_get_ftp();

	CvCapture* capture = cvCreateCameraCapture(cam);
	if (capture == NULL) {
		answ.code = PHOTO_FAIL;
		if (!tcp_client_send(s_client, &answ, sizeof(struct command))) {
			pthread_mutex_lock(mutex);
			log_local("Fail sending answ.", LOG_ERROR);
			pthread_mutex_unlock(mutex);
			return false;
		}
		return false;
	}
	
	IplImage* frame = cvQueryFrame(capture);
	if (frame == NULL) {
		answ.code = PHOTO_FAIL;
		if (!tcp_client_send(s_client, &answ, sizeof(struct command))) {
			pthread_mutex_lock(mutex);
			log_local("Fail sending answ.", LOG_ERROR);
			pthread_mutex_unlock(mutex);
			return false;
		}
		return false;
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
			pthread_mutex_lock(mutex);
			log_local("Fail sending answ.", LOG_ERROR);
			pthread_mutex_unlock(mutex);
			return false;
		}
		return false;
	}
	if (!FtpLogin(fc->user, fc->passwd, nb)) {
		log_local("Fail ftp login data", LOG_ERROR);
		FtpQuit(nb);
		answ.code = PHOTO_FAIL;
		if (!tcp_client_send(s_client, &answ, sizeof(struct command))) {
			pthread_mutex_lock(mutex);
			log_local("Fail sending answ.", LOG_ERROR);
			pthread_mutex_unlock(mutex);
			return false;
		}
		return false;
	}
	char num[4];
	sprintf(num, "%u", cam);
	strcpy(filename, "/video/photo");
	strcat(filename, num);
	strcat(filename, ".jpg");
			
	if (!FtpPut("photo.jpg", filename, FTPLIB_BINARY, nb)) {
		log_local("Fail uploading photo on ftp", LOG_ERROR);
		FtpQuit(nb);
		answ.code = PHOTO_FAIL;
		if (!tcp_client_send(s_client, &answ, sizeof(struct command))) {
			pthread_mutex_lock(mutex);
			log_local("Fail sending answ.", LOG_ERROR);
			pthread_mutex_unlock(mutex);
			return false;
		}
		return false;
	}
	FtpQuit(nb);
	return true;
}