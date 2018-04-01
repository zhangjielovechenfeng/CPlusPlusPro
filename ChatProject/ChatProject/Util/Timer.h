#pragma once
#include "Util.h"
#include <iostream>
#include <boost/function.hpp>

/*
	定时器：使用linux中select的第五个参数实现
*/
typedef boost::function<void()> CallbackFunc;
#define TIMER_IND_FUNC(func) boost::bind(func, this)

class Timer
{
public:
	Timer();
	~Timer();

public:
	void Sleep();

	void Start(ULONG msecond, CallbackFunc* func);
	bool Going();
	void Stop();

	// 获取接下来的时间触发点
	ULONG GetNextTriggerMTime();
private:
	ULONG		m_triggerIntervalMTime; // 触发间隔时间
	ULONG		m_nextTriggerMTime;		// 下次触发时间
	bool		m_stopFlag;
	
};

