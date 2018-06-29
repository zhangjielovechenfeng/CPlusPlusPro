#pragma once

#include "../Protocol/CSMsgPkg/CSMsgPkgDef.h"
#include <string>

using namespace std;

#define MAX_ONE_PKG_SIZE 1024 // 一个包最大大小
/*
	Message : 聊天消息类 
*/
class MessageBase
{
public:
	MessageBase();
	virtual ~MessageBase();

public:
	CSMsgPkg & GetMsgPkg();

	int GetSessionID();

public:
	// 消息处理函数，子类自己实现
	virtual void HandleMsgData() = 0;		

protected:
	CSMsgPkg		m_csMsgPkg;
	int				m_sessionID;
};

/*
	C->S过程中S收到的消息
*/
class CSMessage : public MessageBase
{
public:
	CSMessage(int session);
	virtual ~CSMessage();

public:
	// 通知消息处理handle
	bool HandleMsg();

	// 解析
	virtual void HandleMsgData();

	void SetMsgData(string& data);
private:
	string				m_data;

};


/*
S->C过程中S发送的消息
*/
class SCMessage : public MessageBase
{
public:
	SCMessage(int sessionID);
	virtual ~SCMessage();

public:
	// 处理发送msg
	virtual void HandleMsgData();

	string& GetSerializeData();
private:
	string			m_serializeData; // 序列化后的数据

};

