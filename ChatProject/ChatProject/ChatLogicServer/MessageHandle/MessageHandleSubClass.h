#pragma once

#include "MessageHandle.h"

/*
	��Ϣ����ľ�������
*/

class MessageHandleNone : public MessageHandle
{
public:
	MessageHandleNone() {}
	virtual ~MessageHandleNone() {}
public:
	virtual bool Handle() { return true;  }
};

class MessageHandleHeartBeatReq : public MessageHandle
{
public:
	MessageHandleHeartBeatReq() {}
	virtual ~MessageHandleHeartBeatReq() {}
public:
	virtual bool Handle();
};