#pragma once
#include "sys/time.h"
#include "../Util.h"
#include <string>

using namespace std;

/*
	时间处理器
*/
class Time
{
public:
	Time();
	~Time();

public:
	// 获取当前时间戳毫秒
	static uint64_t GetCurrMTime();

	// 获取当前日期
	static string GetCurrDate();

	static void MSleep(time_t msecond);
};

