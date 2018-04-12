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
	// 反序
	static char* PositiveOrder(char* data);

	// 正序
	static char* NegativeOrder(char* data);

private:
	static int m_key[];
};

