#pragma once
#include "Util.h"
#include <iostream>
#include <boost/function.hpp>

using namespace std;

/*
	定时器：使用linux中select的第五个参数实现
*/
typedef boost::function<void()> CallbackFunc;

class Timer
{
public:
	Timer();
	~Timer();

public:
	void MSleep(UINT msecond);

	void SStart(UINT msecond) {}
	void MStart(UINT second) {}
	void SStart(UINT msecond, CallbackFunc* func) {}
	void MStart(UINT msecond, CallbackFunc* func) {}
	
};

