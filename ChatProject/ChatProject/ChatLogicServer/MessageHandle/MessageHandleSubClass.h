#pragma once

#include "MessageHandle.h"

/*
	消息处理的具体子类
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