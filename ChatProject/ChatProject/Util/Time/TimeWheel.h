#pragma once

#include <vector>
#include "TimeWheelBucket.h"

using namespace std;

/*
	时间轮
*/
typedef vector<TimeWheelBucket*>  TimeWheelBucketVec;
class TimeWheel
{
public:
	TimeWheel(int index);
	~TimeWheel();

	int GetCursor();

	// 返回false表示时间轮走完一轮，需重置重新开始
	bool SetCursor(int cursor);

	// 初始化轮子
	bool InitTimeWheel(int bucketNum);

	// 运行轮子
	bool Run();

	// 插入定时器
	void InsertTimer(Timer& timer);

	// 根据索引获取时间轮桶
	TimeWheelBucket* GetTimeWheelBucketByIndex(int index);

	// 移除定时器任务
	bool RemoveTimerTask(Timer & timer);

private:
	int					m_index;				// 时间轮的索引号，从0开始，方便数组索引
	int					m_cursor;				// 轮上游标，相当于时钟的指针针
	TimeWheelBucketVec	m_timeWheelBucketVec;	// 时间轮桶vec
};

