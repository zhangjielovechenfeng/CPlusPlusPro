#pragma once
#include "sys/time.h"
#include "Util.h"
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
	// 获取当前时间戳微秒
	static ULONG GetCurrMTime();

	// 获取当前日期
	static string GetCurrDate();
};

