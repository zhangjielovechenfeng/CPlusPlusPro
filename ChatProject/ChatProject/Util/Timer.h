#pragma once
#include "Util.h"
#include <iostream>
#include <boost/function.hpp>
#include <boost/bind.hpp>

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

	void Start(uint64_t msecond, CallbackFunc func);
	bool Going();
	void Stop();

	// ��ȡ��������ʱ�䴥����
	uint64_t GetNextTriggerMTime();
private:
	uint64_t		m_triggerIntervalMTime; // �������ʱ��
	uint64_t		m_nextTriggerMTime;		// �´δ���ʱ��
	bool			m_stopFlag;
	
};

