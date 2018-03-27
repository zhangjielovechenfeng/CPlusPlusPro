#pragma once

#include "Log.h"
#include <stdio.h>
#include <string.h>

#define LOG_SIZE 1024

#define LOG_ERR(_s_, ...) do{\
	char _msg_[LOG_SIZE];\
	char _log_[LOG_SIZE];\
	snprintf(_msg_, sizeof(_msg_)-1, _s_, ##__VA_ARGS__);\
	_msg_[LOG_SIZE - 1] = 0;\
	snprintf(_log_, sizeof(_log_)-1, "%s------[%s:%s():%d]\n",\
	_msg_, __FILE__, __FUNCTION__, __LINE__);\
	_log_[LOG_SIZE - 1] = 0;\
	LogErr::WriteLogToFile(_log_, strnlen(_log_, sizeof(_log_)));\
}while(0)

#define LOG_RUN(_s_, ...) do{\
	char _msg_[LOG_SIZE];\
	char _log_[LOG_SIZE];\
	snprintf(_msg_, sizeof(_msg_)-1, _s_, ##__VA_ARGS__);\
	_msg_[LOG_SIZE - 1] = 0;\
	snprintf(_log_, sizeof(_log_)-1, "%s------[%s:%s():%d]\n",\
	_msg_, __FILE__, __FUNCTION__, __LINE__);\
	_log_[LOG_SIZE - 1] = 0;\
	LogErr::WriteLogToFile(_log_, strnlen(_log_, sizeof(_log_)));\
}while(0)

	
