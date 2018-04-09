#pragma once
#include "../Util.h"
#include <iostream>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <list>

using namespace std;

#define TIMER_IND_FUNC(func) boost::bind(func, this)

/*
	Timer在时间轮上的信息
*/
class TimerInfo
{
public: 
	TimerInfo() 
	{
		m_timeWheelIndex = -1;
		m_bucketIndex = -1;
		m_leftMTimeToTrigger = -1;
	}
	~TimerInfo() {}
public:
	int		m_timeWheelIndex;		// 时间轮索引
	int		m_bucketIndex;			// 桶索引
	time_t	m_leftMTimeToTrigger;	// 当位于某个时间轮和时间桶时，剩下到触发的时间间隔；方便把任务往下一级时间轮移动
};

/*
	定时器：使用linux中select的第五个参数实现
*/
typedef boost::function<void()> CallbackFunc;
typedef list<TimerInfo*> TimerTrackInfoList;

class Timer
{
public:
	Timer();
	~Timer();

public:
	void SetIsTrigger(bool isTrigger);

	bool IsTrigger();

public:
	void Start(time_t msecond, CallbackFunc func);

	bool Going();

	void Stop();

	void Trigger();

	// 获取接下来的时间触发点
	time_t GetNextTriggerMTime();

	// 获取触发时间间隔
	time_t GetTriggerIntervalMTime();

	// 添加Timer信息到列表
	void AddTimerInfoToList(TimerInfo* timerInfo);

	// 获取最后的timer跟踪信息
	TimerInfo* GetLastTimerTrackInfo();

	// 移动到下一个时间轮
	void MoveToNextTimeWheel();

	// 获取Timer信息跟踪列表
	TimerTrackInfoList& GetTimerInfoTrackList();

private:
	CallbackFunc		m_func;					// 回调函数
	time_t				m_triggerIntervalMTime; // 触发间隔时间
	time_t				m_nextTriggerMTime;		// 下次触发时间
	bool				m_stopFlag;
	bool				m_isTrigger;			// 是否触发
	TimerTrackInfoList	m_timerTrackInfoList;	// Timer跟踪信息列表
};

