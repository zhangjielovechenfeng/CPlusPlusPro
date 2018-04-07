#pragma once

#include "../Message.h"

/*
	消息处理基类
*/
class MessageHandle
{
public:
	MessageHandle();
	virtual ~MessageHandle();

public:
	bool InitMessgeHandle(Message* msg);

public:
	virtual bool Handle() = 0;

protected:
	Message*	m_msg;
};

