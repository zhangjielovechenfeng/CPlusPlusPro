#pragma once
#include "Util.h"
#include <iostream>
#include <boost/function.hpp>

/*
	��ʱ����ʹ��linux��select�ĵ��������ʵ��
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

	// ��ȡ��������ʱ�䴥����
	ULONG GetNextTriggerMTime();
private:
	ULONG		m_triggerIntervalMTime; // �������ʱ��
	ULONG		m_nextTriggerMTime;		// �´δ���ʱ��
	bool		m_stopFlag;
	
};

