#include "Timer.h"
#include <sys/time.h>
#include <cstddef>
#include "Time.h"
#include "TimeWheelManager.h"

Timer::Timer()
{
	m_triggerIntervalMTime = 0;
	m_nextTriggerMTime = 0;
	m_stopFlag = true;
	m_isTrigger = false;
}


Timer::~Timer()
{
	Stop();
}

void Timer::SetIsTrigger(bool isTrigger)
{
	m_isTrigger = isTrigger;
}

bool Timer::IsTrigger()
{
	return m_isTrigger;
}

void Timer::Start(time_t msecond, CallbackFunc func)
{
	m_stopFlag = false;
	m_triggerIntervalMTime = msecond;
	m_func = func;
	
	TimeWheelManager::Instance().InsertTimer(*this);
}

bool Timer::Going()
{
	return !m_stopFlag;
}

void Timer::Stop()
{
	TimeWheelManager::Instance().RemoveTimerTask(*this);
	m_stopFlag = true;
}

void Timer::Trigger()
{
	m_isTrigger = true;
	m_func();
}

time_t Timer::GetNextTriggerMTime()
{
	return Time::GetCurrMTime() + GetTriggerIntervalMTime();
}

time_t Timer::GetTriggerIntervalMTime()
{
	return m_triggerIntervalMTime;
}

TimerInfo * Timer::GetLastTimerTrackInfo()
{
	TimerTrackInfoList::iterator it = m_timerTrackInfoList.end();
	if (NULL == *(--it))
	{
		return NULL;
	}
	return *it;
}

void Timer::MoveToNextTimeWheel()
{
	TimeWheelManager::Instance().InsertTimer(*this);
}

TimerTrackInfoList & Timer::GetTimerInfoTrackList()
{
	return m_timerTrackInfoList;
}

void Timer::AddTimerInfoToList(TimerInfo* timerInfo)
{
	ASSERT_RETURN_VOID(timerInfo != NULL);
	m_timerTrackInfoList.push_back(timerInfo);
}
