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
	static void NegativeOrder(char* data, int len);

	// �����൱�ڽ�����(len: ��������ݳ���)
	static void PositiveOrder(char* data, int len);

private:
	static int m_key[];
};

