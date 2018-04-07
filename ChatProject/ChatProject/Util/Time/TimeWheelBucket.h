#pragma once

#include "Timer.h"

/*
	任务队列节点
*/
class TaskQueueNode
{
public:
	TaskQueueNode * m_prev;		// 上一个节点
	TaskQueueNode * m_next;		// 下一个节点
	Timer*			m_timer;	// 节点中的定时器任务
	TaskQueueNode()
	{
		m_prev = m_next = this;
		m_timer = NULL;
	}
};


#define LOWEST_LAYER_TIMEWHEEL_INDEX 0	// 最底层时间轮索引

/*
	时间轮中的桶，也即时间格
*/
class TimeWheelBucket
{
public:
	TimeWheelBucket(int	timeWheelIndex);
	~TimeWheelBucket();

	// 是否为空
	bool IsEmpty();

	// 处理任务
	bool HandleTask();

	// 弹出，在rootNode后的
	TaskQueueNode* PopFirst();

	//插入定时器任务
	void InsertTimer(Timer& timer);

	// 添加，在rootNode前
	void PushBack(TaskQueueNode* taskQueueNode);

	// 移除定时器任务
	bool RemoveTimerTask(Timer & timer);

private:
	int					m_timeWheelIndex;	// 时间轮索引
	TaskQueueNode*		m_rootNode;			// 任务链根节点
};

