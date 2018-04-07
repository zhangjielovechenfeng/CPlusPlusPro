#include "TimeWheel.h"
#include "Time.h"
#include "TimeWheelManager.h"
#include <math.h>

TimeWheel::TimeWheel(int index)
{
	m_index = index;
	m_cursor = -1;
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
	return false;
}

bool TimeWheel::Run()
{
	for (int i = 0; i < (int)m_timeWheelBucketVec.size(); ++i)
	{
		// 只最低层次轮子调用MSleep，先MSleep后再进行处理
		Time::MSleep(TimeWheelManager::Instance().GetTickIntervalMs());

		// 处理桶上的任务
		TimeWheelBucket* timeWheelBucket = m_timeWheelBucketVec[i];
		if (!timeWheelBucket->IsEmpty())
		{
			timeWheelBucket->HandleTask();
		}
		SetCursor(GetCursor() + 1);
	}
	return true;
}

void TimeWheel::InsertTimer(Timer & timer)
{
	TimerInfo* timerInfo = new TimerInfo;
	timerInfo->m_timeWheelIndex = m_index;


	int bucketIndex = 0;
	if (timer.GetTimerInfoTrackList().empty())
	{
		bucketIndex = (timer.GetTriggerIntervalMTime() + TimeWheelManager::Instance().GetCurrTime()) / GetLowerIndexTimeSpan();
		timerInfo->m_leftMTimeToTrigger = (timer.GetTriggerIntervalMTime() + TimeWheelManager::Instance().GetCurrTime()) % GetLowerIndexTimeSpan();
	}
	else
	{
		TimerInfo* oldTimerInfo = timer.GetLastTimerTrackInfo();
		ASSERT_RETURN_VOID(oldTimerInfo != NULL);

		bucketIndex = oldTimerInfo->m_leftMTimeToTrigger / GetLowerIndexTimeSpan();
		timerInfo->m_leftMTimeToTrigger = oldTimerInfo->m_leftMTimeToTrigger % GetLowerIndexTimeSpan();
	}

	timerInfo->m_bucketIndex = bucketIndex;
	GetTimeWheelBucketByIndex(bucketIndex)->InsertTimer(timer);
}

int TimeWheel::GetCursor()
{
	return m_cursor;
}

void TimeWheel::SetCursor(int cursor)
{
	m_cursor = cursor;
	// 此时需要进位
	if (m_cursor == (int)m_timeWheelBucketVec.size() )
	{
		TimeWheel* timeWheel = TimeWheelManager::Instance().GetTimeWheelByIndex(m_index + 1);
		if (NULL == timeWheel)
		{
			// 已经是最上层时间轮，运行结束，重新开始
			TimeWheelManager::Instance().ResetTimeWheel();
			TimeWheelManager::Instance().Run();
			return;
		}
		timeWheel->SetCursor(timeWheel->GetCursor() + 1);
		m_cursor = 0;
	}
}

time_t TimeWheel::GetLowerIndexTimeSpan()
{

	return pow(TimeWheelManager::Instance().GetTickIntervalMs() * m_timeWheelBucketVec.size(), m_index);
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
