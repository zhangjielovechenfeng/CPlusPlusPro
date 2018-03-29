#include "Timer.h"
#include <sys/time.h>
#include <cstddef>

Timer::Timer()
{
}


Timer::~Timer()
{
}

void Timer::MSleep(UINT msecond)
{
	struct timeval tv;

	tv.tv_sec = msecond/1000;
	tv.tv_usec = (msecond % 1000) * 1000; //’‚¿Ô «Œ¢√Î

	select(0, NULL, NULL, NULL, &tv);
}
