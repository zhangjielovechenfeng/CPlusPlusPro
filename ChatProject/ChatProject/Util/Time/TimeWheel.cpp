#include "TimeWheel.h"
#include "Time.h"
#include "TimeWheelManager.h"
#include <math.h>

TimeWheel::TimeWheel(int index)
{
	m_index = index;
	m_cursor = -1;
	m_taskCount = 0;
	m_timeWheelBucketVec.clear();
}


TimeWheel::~TimeWheel()
{
	for (int i = 0; i < (int)m_timeWheelBucketVec.size(); ++i)
	{
		TimeWheelBucket* timeWheelBucket = m_timeWheelBucketVec[m_index];
		SAFE_DELETE(timeWheelBucket);
	}
}

bool TimeWheel::InitTimeWheel(int bucketNum)
{
	for (int i = 0; i < bucketNum; ++i)
	{
		TimeWheelBucket* timeWheelBucket = new TimeWheelBucket(m_index);
		m_timeWheelBucketVec.push_back(timeWheelBucket);
	}
	return true;
}

bool TimeWheel::Run()
{
	while(1)
	{
		// 只最低层次轮子调用MSleep，先MSleep后再进行处理
		Time::MSleep(TimeWheelManager::Instance().GetTickIntervalMs());

		if (!SetCursor(GetCursor() + 1))
		{
			// 重置轮子
			TimeWheelManager::Instance().ResetTimeWheel();
			continue;
		}
	}
	return true;
}

void TimeWheel::InsertTimer(Timer & timer)
{
	TimerInfo* timerInfo = new TimerInfo;
	timerInfo->m_timeWheelIndex = m_index;


	int bucketIndex = 0;
	if (timer.GetTimerInfoTrackList().empty() || timer.IsTrigger())
	{
		bucketIndex = (timer.GetTriggerIntervalMTime() + TimeWheelManager::Instance().GetCurrTime()) / GetBucketTimeSpan() ;
		timerInfo->m_leftMTimeToTrigger = (timer.GetTriggerIntervalMTime() + TimeWheelManager::Instance().GetCurrTime()) % GetBucketTimeSpan();
		timer.SetIsTrigger(false);
	}
	else
	{
		TimerInfo* oldTimerInfo = timer.GetLastTimerTrackInfo();
		ASSERT_RETURN_VOID(oldTimerInfo != NULL);

		bucketIndex = oldTimerInfo->m_leftMTimeToTrigger / GetBucketTimeSpan();
		timerInfo->m_leftMTimeToTrigger = oldTimerInfo->m_leftMTimeToTrigger % GetBucketTimeSpan();
	}

	timerInfo->m_bucketIndex = bucketIndex;
	timer.AddTimerInfoToList(timerInfo);

	GetTimeWheelBucketByIndex(bucketIndex)->InsertTimer(timer);

	// 如果添加的时间段正在触发，立即向下一轮移动，最低层直接处理
	if (bucketIndex == GetCursor())
	{
		if (m_index != LOWEST_LAYER_TIMEWHEEL_INDEX)
		{
			timer.MoveToNextTimeWheel();
		}
		else
		{
			m_timeWheelBucketVec[bucketIndex]->HandleTask();
		}
	}
}

int TimeWheel::GetCursor()
{
	return m_cursor;
}

bool TimeWheel::SetCursor(int cursor)
{
	m_cursor = cursor;

	if (m_cursor == 0)
	{
		TimeWheel* timeWheel = TimeWheelManager::Instance().GetTimeWheelByIndex(m_index + 1);
		if (timeWheel != NULL)
		{
			timeWheel->SetCursor(0);
		}
	}

	if (m_cursor == (int)m_timeWheelBucketVec.size())
	{
		TimeWheel* timeWheel = TimeWheelManager::Instance().GetTimeWheelByIndex(m_index + 1);
		if (NULL == timeWheel)
		{
			return false;
		}
		
		if (!timeWheel->SetCursor(timeWheel->GetCursor() + 1))
		{
			return false;
		}
		m_cursor = 0;
	}

	// 处理桶上的任务
	TimeWheelBucket* timeWheelBucket = m_timeWheelBucketVec[m_cursor];
	if (!timeWheelBucket->IsEmpty())
	{
		timeWheelBucket->HandleTask();
	}
	return true;
}

time_t TimeWheel::GetBucketTimeSpan()
{

	return pow(TimeWheelManager::Instance().GetTickIntervalMs(), m_index + 1);
}

TimeWheelBucket * TimeWheel::GetTimeWheelBucketByIndex(int index)
{
	return m_timeWheelBucketVec[index];
}

bool TimeWheel::RemoveTimerTask(Timer & timer)
{
	m_timeWheelBucketVec[timer.GetLastTimerTrackInfo()->m_bucketIndex]->RemoveTimerTask(timer);
	return true;
}
