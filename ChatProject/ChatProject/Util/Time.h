#pragma once
#include "sys/time.h"
#include "Util.h"
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
	// ��ȡ��ǰʱ���΢��
	static ULONG GetCurrMTime();

	// ��ȡ��ǰ����
	static string GetCurrDate();
};

