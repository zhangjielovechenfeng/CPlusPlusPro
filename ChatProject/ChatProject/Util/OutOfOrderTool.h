#pragma once

/*
	���򹤾���
*/

class OutOfOrderTool
{
public:
	OutOfOrderTool() {}
	~OutOfOrderTool() {}

public:
	// ����
	static char* PositiveOrder(char* data);

	// ����
	static char* NegativeOrder(char* data);

private:
	static int m_key[];
};

