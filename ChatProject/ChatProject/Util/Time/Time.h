#pragma once
#include "sys/time.h"
#include "../Util.h"
#include <string>

using namespace std;

/*
	ʱ�䴦����
*/
class Time
{
public:
	Time();
	~Time();

public:
	// ��ȡ��ǰʱ�������
	static uint64_t GetCurrMTime();

	// ��ȡ��ǰ����
	static string GetCurrDate();

	static void MSleep(time_t msecond);
};

