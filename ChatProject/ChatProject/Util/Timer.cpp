#include "Timer.h"
#include <sys/time.h>
#include <cstddef>
#include "Time.h"

Timer::Timer()
{
	m_triggerIntervalMTime = 0;
	m_nextTriggerMTime = 0;
	m_stopFlag = false;
}


Timer::~Timer()
{
}

void Timer::Sleep()
{
	struct timeval tv;

	tv.tv_sec = m_triggerIntervalMTime / 1000;
	tv.tv_usec = (m_triggerIntervalMTime % 1000) * 1000; //’‚¿Ô «Œ¢√Î

	select(0, NULL, NULL, NULL, &tv);
}

void Timer::Start(ULONG msecond, CallbackFunc * func)
{
	m_stopFlag = true;

	m_triggerIntervalMTime = msecond;

	while (1)
	{
		if (!m_stopFlag)
		{
			return;
		}
		m_nextTriggerMTime = Time::GetCurrMTime();

		Sleep();

		(*func)();
	}
}

bool Timer::Going()
{
	return !m_stopFlag;
}

void Timer::Stop()
{
	m_stopFlag = true;
}

ULONG Timer::GetNextTriggerMTime()
{
	return m_nextTriggerMTime;
}
