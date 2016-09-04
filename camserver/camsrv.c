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
#include <pthread.h>
#include <stdio.h>
#include <ftplib.h>
#include <string.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>


static struct {
	pthread_mutex_t mutex;
	struct tcp_server server;
} cam;


static void send_answ(struct tcp_client *s_client, bool ok)
{
	struct command answ;

	if (ok)
		answ.code = PHOTO_OK;
	else
		answ.code = PHOTO_FAIL;
	if (!tcp_client_send(s_client, &answ, sizeof(struct command))) {
		pthread_mutex_lock(&cam.mutex);
		log_local("Fail sending answ.", LOG_ERROR);
		pthread_mutex_unlock(&cam.mutex);
		return;
	}
}

static void new_session(struct tcp_client *s_client, void *data)
{
	struct command cmd;
	netbuf *nb;
	char filename[50];
	struct ftp_cfg *fc = configs_get_ftp();

	if (!tcp_client_recv(s_client, &cmd, sizeof(struct command))) {
		pthread_mutex_lock(&cam.mutex);
		log_local("Fail receiving cmd.", LOG_ERROR);
		pthread_mutex_unlock(&cam.mutex);
		return;
	}
	if (cmd.code != GET_PHOTO) {
		send_answ(s_client, false);
		return;
	}

	/*
	 * Getting photo fron CAM by OpenCV
	 */
	CvCapture* capture = cvCreateCameraCapture(cmd.cam);
	if (capture == NULL) {
		send_answ(s_client, false);
		return;
	}
	
	IplImage* frame = cvQueryFrame(capture);
	if (frame == NULL) {
		send_answ(s_client, false);
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
		send_answ(s_client, false);
		return;
	}
	if (!FtpLogin(fc->user, fc->passwd, nb)) {
		log_local("Fail ftp login data", LOG_ERROR);
		FtpQuit(nb);
		send_answ(s_client, false);
		return;
	}
	if (cmd.cam == CAM_1)
		strcpy(filename, "/video/photo1.jpg");
	else
		strcpy(filename, "/video/photo2.jpg");

	if (!FtpPut("photo.jpg", filename, FTPLIB_BINARY, nb)) {
		log_local("Fail uploading photo on ftp", LOG_ERROR);
		send_answ(s_client, false);
		FtpQuit(nb);
		return;
	}
	FtpQuit(nb);

	/*
	 * Sending answ to client
	 */
	send_answ(s_client, true);
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
