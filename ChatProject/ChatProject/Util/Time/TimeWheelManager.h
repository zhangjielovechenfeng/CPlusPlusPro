#pragma once

#include "TimeWheel.h"
#include <vector>
#include "../Singleton.h"

using namespace std;

typedef vector<TimeWheel*> TimeWheelVec;

/*
	时间轮管理器
*/
class TimeWheelManager : public Singleton<TimeWheelManager>
{
public:
	TimeWheelManager();
	~TimeWheelManager();
	
public:
	bool InitTimeWheelManager(int timeWheelNum, time_t tickIntervalMs, int bucketNum);

	// 运行时间轮
	void Run();

	time_t GetTickIntervalMs();

	// 插入定时器
	bool InsertTimer(Timer& timer);

	// 获取当前时间，通过计算各时间轮上游标的位置
	time_t GetCurrTime();

	// 通过索引获取定时器
	TimeWheel* GetTimeWheelByIndex(int index);

	// 重置时间轮
	bool ResetTimeWheel();

	// 移除定时器任务
	bool RemoveTimerTask(Timer & timer);

private:
	TimeWheelVec		m_timeWheelVec;
	time_t				m_tickIntervalMs;
	int					m_bucketNum;		// 每个时间轮桶的个数
};

