#ifndef __LOG_H__
#define __LOG_H__

#include <stdbool.h>

enum {
	LOG_ERROR,
	LOG_WARNING,
	LOG_INFO
};


/**
 * Setting path to log file
 * @path: path with filename
 *
 * returns true: if setted
 * returns false: if path size > max_path_size
 */
bool log_set_path(const char *path);

/**
 * Writing message to local log file
 * @message: writing message
 * @log_type: type of log message
 *
 * returns true: if message writed
 * returns false: if message writing error
 */
bool log_local(const char *message, unsigned log_type);


#endif
