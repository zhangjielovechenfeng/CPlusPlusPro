#include "TimeWheelManager.h"
#include <boost/thread.hpp>
#include <math.h>
#include "../ErrDefine.h"

TimeWheelManager::TimeWheelManager()
{
	m_timeWheelVec.clear();
}


TimeWheelManager::~TimeWheelManager()
{
	for (int i = 0; i < (int)m_timeWheelVec.size(); ++i)
	{
		TimeWheel* timeWheel = m_timeWheelVec[i];
		SAFE_DELETE(timeWheel);
	}
}

bool TimeWheelManager::InitTimeWheelManager(int timeWheelNum, time_t tickIntervalMs, int bucketNum)
{
	m_tickIntervalMs = tickIntervalMs;
	m_bucketNum = bucketNum;
	for (int i = 0; i < timeWheelNum; ++i)
	{
		TimeWheel* timeWheel = new TimeWheel(i);
		timeWheel->InitTimeWheel(bucketNum);

		m_timeWheelVec.push_back(timeWheel);
	}
	return true;
}

void TimeWheelManager::Run()
{
	// 让最底层时间轮运行起来，上层通过进位使之运行起来
	m_timeWheelVec[LOWEST_LAYER_TIMEWHEEL_INDEX]->Run();
}

time_t TimeWheelManager::GetTickIntervalMs()
{
	return m_tickIntervalMs;
}

bool TimeWheelManager::InsertTimer(Timer & timer)
{
	int timeWheelIndex = 0;

	if (timer.GetTimerInfoTrackList().empty())
	{
		for ( ; timeWheelIndex < (int)m_timeWheelVec.size(); ++timeWheelIndex)
		{
			if (1 < (GetCurrTime() + timer.GetTriggerIntervalMTime()) / pow(m_tickIntervalMs * m_bucketNum, timeWheelIndex + 1))
			{
				continue;
			}
			break;
		}
	}
	else
	{
		TimerInfo * oldTimer = timer.GetLastTimerTrackInfo();
		ASSERT_RETURN(oldTimer != NULL, false);

		timeWheelIndex = oldTimer->m_timeWheelIndex;	
	}
	
	//timerInfo->m_timeWheelIndex = timeWheelIndex - 1;
	//timer.AddTimerInfoToList(timerInfo);
	m_timeWheelVec[timeWheelIndex - 1]->InsertTimer(timer);

	return true;
}

time_t TimeWheelManager::GetCurrTime()
{
	time_t currTime = 0;
	for (int i = 0; i < (int)m_timeWheelVec.size(); ++i)
	{
		TimeWheel* timeWheel = m_timeWheelVec[i];
		if (NULL == timeWheel)
		{
			return 0;
		}
		currTime += timeWheel->GetCursor() * m_tickIntervalMs * pow(m_bucketNum, i);
	}
	return currTime;
}

TimeWheel * TimeWheelManager::GetTimeWheelByIndex(int index)
{
	return m_timeWheelVec[index];
}

bool TimeWheelManager::ResetTimeWheel()
{
	for (int i = 0; i < m_timeWheelVec.size(); ++i)
	{
		m_timeWheelVec[i]->SetCursor(-1);
	}
	return true;
}

bool TimeWheelManager::RemoveTimerTask(Timer & timer)
{
	m_timeWheelVec[timer.GetLastTimerTrackInfo()->m_timeWheelIndex]->RemoveTimerTask(timer);
	return true;
}
