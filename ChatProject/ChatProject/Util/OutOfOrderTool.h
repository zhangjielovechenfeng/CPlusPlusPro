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
	// �����൱������(len: ��������ݳ���)
	static char* NegativeOrder(char* data, int len);

	// �����൱�ڽ�����(len: ��������ݳ���)
	static char* PositiveOrder(char* data, int len);

private:
	static int m_key[];
};

