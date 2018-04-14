#pragma once

/*
	乱序工具类
*/

class OutOfOrderTool
{
public:
	OutOfOrderTool() {}
	~OutOfOrderTool() {}

public:
	// 反序（相当于乱序）(len: 反序的数据长度)
	static char* NegativeOrder(char* data, int len);

	// 正序（相当于解乱序）(len: 正序的数据长度)
	static char* PositiveOrder(char* data, int len);

private:
	static int m_key[];
};

