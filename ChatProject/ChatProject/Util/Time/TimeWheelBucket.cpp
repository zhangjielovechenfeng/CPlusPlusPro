#include "TimeWheelBucket.h"
#include "../LogPrint.h"
#include "TimeWheelManager.h"


TimeWheelBucket::TimeWheelBucket(int timeWheelIndex)
{
	m_timeWheelIndex = timeWheelIndex;
	m_rootNode = new TaskQueueNode();
}


TimeWheelBucket::~TimeWheelBucket()
{
	while (!IsEmpty())
	{
		TaskQueueNode * firstNode = PopFirst();
		SAFE_DELETE(firstNode);
	}
	SAFE_DELETE(m_rootNode);
}

bool TimeWheelBucket::IsEmpty()
{
	if (m_rootNode->m_next == m_rootNode->m_prev && m_rootNode == m_rootNode->m_next && m_rootNode == m_rootNode->m_next)
	{
		return true;
	}
	return false;
}

bool TimeWheelBucket::HandleTask()
{
	// �����Ͱ��Ӧ��ʱ���ֲ�����ײ㣬�Ͱѵ�ǰͰ������������ӵ���һ���Ͱ��
	if (m_timeWheelIndex != LOWEST_LAYER_TIMEWHEEL_INDEX)
	{
		// ��������ڵ��ϵ����񣬲��Ƴ�
		while (!IsEmpty())
		{
			TaskQueueNode * firstNode = PopFirst();
			Timer* timer = firstNode->m_timer;
			if (NULL == timer)
			{
				LOG_ERR("Add Timer Error!!!");
				continue;
			}

			// �Ƶ���һ��
			timer->MoveToNextTimeWheel();
		}
	}
	else
	{
		// ��������ڵ��ϵ����񣬲��Ƴ�
		while (!IsEmpty())
		{
			TaskQueueNode * firstNode = PopFirst();
			Timer* timer = firstNode->m_timer;
			if (NULL == timer)
			{
				LOG_ERR("Add Timer Error!!!");
				continue;
			}

			timer->Trigger();
			// ���°Ѵ˽ڵ���ӵ�ʱ������
			TimeWheelManager::Instance().InsertTimer(*timer);
		}
	}
	return true;
}

TaskQueueNode * TimeWheelBucket::PopFirst()
{
	if (IsEmpty()) 
	{
		return NULL;
	}
	TaskQueueNode* firstNode = m_rootNode->m_next;
	m_rootNode->m_next = firstNode->m_next;
	firstNode->m_next->m_prev = m_rootNode;

	return firstNode;
}

void TimeWheelBucket::InsertTimer(Timer & timer)
{
	TaskQueueNode* taskQueueNode = new TaskQueueNode();
	taskQueueNode->m_timer = &timer;
	PushBack(taskQueueNode);
}

void TimeWheelBucket::PushBack(TaskQueueNode * taskQueueNode)
{
	TaskQueueNode * endNode = m_rootNode->m_prev;
	endNode->m_next = taskQueueNode;
	taskQueueNode->m_next = m_rootNode;
	m_rootNode->m_prev = taskQueueNode;
	taskQueueNode->m_prev = endNode;
}

bool TimeWheelBucket::RemoveTimerTask(Timer & timer)
{
	TaskQueueNode * tmpNode = m_rootNode;
	while (tmpNode->m_timer != &timer)
	{
		tmpNode = tmpNode->m_next;
	}
	tmpNode->m_prev->m_next = tmpNode->m_next;
	tmpNode->m_next->m_prev = tmpNode->m_prev;
	return true;
}

bool TimeWheelBucket::DistributeTimerTaskToLower()
{
	while (!IsEmpty())
	{
		TaskQueueNode * firstNode = PopFirst();
		Timer* timer = firstNode->m_timer;
		if (NULL == timer)
		{
			LOG_ERR("Add Timer Error!!!");
			continue;
		}
		TimeWheelManager::Instance().GetTimeWheelByIndex(m_timeWheelIndex - 1)->InsertTimer(*timer);
	}
	return true;
}

void TimeWheelBucket::SetRootNode(TaskQueueNode * rootNode)
{
	m_rootNode = rootNode;
}

TaskQueueNode * TimeWheelBucket::GetRootNode()
{
	return m_rootNode;
}
