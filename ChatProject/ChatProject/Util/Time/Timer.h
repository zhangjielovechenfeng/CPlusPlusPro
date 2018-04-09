#pragma once
#include "../Util.h"
#include <iostream>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <list>

using namespace std;

#define TIMER_IND_FUNC(func) boost::bind(func, this)

/*
	Timer��ʱ�����ϵ���Ϣ
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
	int		m_timeWheelIndex;		// ʱ��������
	int		m_bucketIndex;			// Ͱ����
	time_t	m_leftMTimeToTrigger;	// ��λ��ĳ��ʱ���ֺ�ʱ��Ͱʱ��ʣ�µ�������ʱ�������������������һ��ʱ�����ƶ�
};

/*
	��ʱ����ʹ��linux��select�ĵ��������ʵ��
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

	// ��ȡ��������ʱ�䴥����
	time_t GetNextTriggerMTime();

	// ��ȡ����ʱ����
	time_t GetTriggerIntervalMTime();

	// ���Timer��Ϣ���б�
	void AddTimerInfoToList(TimerInfo* timerInfo);

	// ��ȡ����timer������Ϣ
	TimerInfo* GetLastTimerTrackInfo();

	// �ƶ�����һ��ʱ����
	void MoveToNextTimeWheel();

	// ��ȡTimer��Ϣ�����б�
	TimerTrackInfoList& GetTimerInfoTrackList();

private:
	CallbackFunc		m_func;					// �ص�����
	time_t				m_triggerIntervalMTime; // �������ʱ��
	time_t				m_nextTriggerMTime;		// �´δ���ʱ��
	bool				m_stopFlag;
	bool				m_isTrigger;			// �Ƿ񴥷�
	TimerTrackInfoList	m_timerTrackInfoList;	// Timer������Ϣ�б�
};

