#include "TimeWheelManager.h"
#include <boost/thread.hpp>
#include <math.h>
#include "../ErrDefine.h"

TimeWheelManager::TimeWheelManager()
{
	m_timeWheelVec.clear();
	m_tickIntervalMs = 0;
	m_bucketNum = 0;
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
	// 线程锁
	ThreadLock ThreadLock();

	int timeWheelIndex = 0;
	if (timer.GetTimerInfoTrackList().empty() || timer.IsTrigger())
	{
		for ( ; timeWheelIndex < (int)m_timeWheelVec.size(); ++timeWheelIndex)
		{
			if (1 <= (GetCurrTime() + timer.GetTriggerIntervalMTime()) / GetBucketTimeSpan(timeWheelIndex)) 
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
		// 表示时间轮还没开始运行，直接返回0,方便计算位置
		if (timeWheel->GetCursor() < 0)
		{
			return 0;
		}
		currTime += timeWheel->GetCursor() * m_tickIntervalMs * pow(m_bucketNum, i);  
	}
	return currTime;
}

TimeWheel* TimeWheelManager::GetTimeWheelByIndex(int index)
{
	if (index == m_timeWheelVec.size())
	{
		return NULL;
	}
	return m_timeWheelVec[index];
}

bool TimeWheelManager::ResetTimeWheel()
{
	for (int i = 0; i < (int)m_timeWheelVec.size(); ++i)
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

time_t TimeWheelManager::GetBucketTimeSpan(int timeWheelIndex)
{
	return TimeWheelManager::Instance().GetTickIntervalMs() * pow(m_bucketNum, timeWheelIndex);
}
