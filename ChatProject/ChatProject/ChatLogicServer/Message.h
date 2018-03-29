#pragma once

#include "../Protocol/ProtoDest/CSProtoPkgDef.pb.h"


using namespace CSProtocol;

#define MAX_ONE_PKG_SIZE 1024 // 一个包最大大小
/*
	Message : 聊天消息类
*/
class Message
{
public:
	Message();
	virtual ~Message();

public:
	// 消息处理函数，子类自己实现
	virtual void HandleMsgData(char* data) = 0;		
};

/*
	C->S过程中S收到的消息
*/
class CSMessage : public Message
{
public:
	CSMessage();
	virtual ~CSMessage();
public:
	CSMsgPkg& GetMsgPkg();

public:
	// 通知消息处理handle
	bool HandleMsg();

	// 解析
	virtual void HandleMsgData(char* data);

private:
	CSMsgPkg		m_csMsgPkg;
};


/*
S->C过程中S发送的消息
*/
class SCMessage : public Message
{
public:
	SCMessage();
	virtual ~SCMessage();

public:
	// 解析消息,并通知消息处理handle
	bool ParseMsgToPkg();

	// 
	virtual void HandleMsgData(char* data);

};

