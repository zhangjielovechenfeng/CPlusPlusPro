#pragma once

#include "Timer.h"

/*
	������нڵ�
*/
class TaskQueueNode
{
public:
	TaskQueueNode * m_prev;		// ��һ���ڵ�
	TaskQueueNode * m_next;		// ��һ���ڵ�
	Timer*			m_timer;	// �ڵ��еĶ�ʱ������
	TaskQueueNode()
	{
		m_prev = m_next = this;
		m_timer = NULL;
	}
};


#define LOWEST_LAYER_TIMEWHEEL_INDEX 0	// ��ײ�ʱ��������

/*
	ʱ�����е�Ͱ��Ҳ��ʱ���
*/
class TimeWheelBucket
{
public:
	TimeWheelBucket(int	timeWheelIndex);
	~TimeWheelBucket();

	// �Ƿ�Ϊ��
	bool IsEmpty();

	// ��������
	bool HandleTask();

	// ��������rootNode���
	TaskQueueNode* PopFirst();

	//���붨ʱ������
	void InsertTimer(Timer& timer);

	// ��ӣ���rootNodeǰ
	void PushBack(TaskQueueNode* taskQueueNode);

	// �Ƴ���ʱ������
	bool RemoveTimerTask(Timer & timer);

private:
	int					m_timeWheelIndex;	// ʱ��������
	TaskQueueNode*		m_rootNode;			// ���������ڵ�
};

