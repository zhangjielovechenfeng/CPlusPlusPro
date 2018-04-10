#pragma once

#include "TimeWheel.h"
#include <vector>
#include "../Singleton.h"

using namespace std;

typedef vector<TimeWheel*> TimeWheelVec;

/*
	ʱ���ֹ�����
*/
class TimeWheelManager : public Singleton<TimeWheelManager>
{
public:
	TimeWheelManager();
	~TimeWheelManager();
	
public:
	bool InitTimeWheelManager(int timeWheelNum, time_t tickIntervalMs, int bucketNum);

	// ����ʱ����
	void Run();

	time_t GetTickIntervalMs();

	// ���붨ʱ��
	bool InsertTimer(Timer& timer);

	// ��ȡ��ǰʱ�䣬ͨ�������ʱ�������α��λ��
	time_t GetCurrTime();

	// ͨ��������ȡ��ʱ��
	TimeWheel* GetTimeWheelByIndex(int index);

	// ����ʱ����
	bool ResetTimeWheel();

	// �Ƴ���ʱ������
	bool RemoveTimerTask(Timer & timer);

private:
	TimeWheelVec		m_timeWheelVec;
	time_t				m_tickIntervalMs;
	int					m_bucketNum;		// ÿ��ʱ����Ͱ�ĸ���
};

