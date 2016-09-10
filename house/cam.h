#ifndef __CAM_H__
#define __CAM_H__

#include "tcpclient.h"
#include <pthread.h>


bool cam_get_photo(struct tcp_client *s_client, uint8_t cam, pthread_mutex_t *mutex);


#endif