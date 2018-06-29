#include "Time.h"
#include "stddef.h"
#include <errno.h>
#include <string.h>
#include "../LogPrint.h"
#include "../ErrDefine.h"
#include "../Util.h"

Time::Time()
{
}


Time::~Time()
{
}

uint64_t Time::GetCurrMTime()
{
	struct timeval tv;

	if (gettimeofday(&tv, NULL) < 0)
	{
		LOG_ERR("Get Curr Time Failed!!!, error: %s", strerror(errno));
		return ERROR_CODE_GET_CURR_TIME_FAILED;
	}
	return (tv.tv_sec * 1000000 + tv.tv_usec) / 1000;
}

string Time::GetCurrDate()
{
	struct timeval tv;

	if (gettimeofday(&tv, NULL) < 0)
	{
		LOG_ERR("Get Curr Time Failed!!!, error: %s", strerror(errno));
		return NULL;
	}
	
	struct tm* tm = localtime(&tv.tv_sec);
	ASSERT_RETURN(tm != NULL, NULL);

	string time = asctime(tm);
	time = time.substr(0, time.length() - 1);
	return time;
}

void Time::MSleep(time_t msecond)
{
	struct timeval tv;

	tv.tv_sec = msecond / 1000;
	tv.tv_usec = (msecond % 1000) * 1000; //ÕâÀïÊÇÎ¢Ãë

	select(0, NULL, NULL, NULL, &tv);
}
