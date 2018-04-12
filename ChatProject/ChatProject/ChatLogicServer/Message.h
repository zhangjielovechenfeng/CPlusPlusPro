#pragma once

#include "../Protocol/ProtoDest/CSProtoPkgDef.pb.h"
#include <string>

using namespace std;

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
	CSMsgPkg& GetMsgPkg();

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
class CSMessage : public Message
{
public:
	CSMessage(int session);
	virtual ~CSMessage();
public:
	CSMsgPkg& GetMsgPkg();

public:
	// 通知消息处理handle
	bool HandleMsg();

	// 解析
	virtual void HandleMsgData();

	void SetMsgData(char* data);
private:
	string				m_data;

};


/*
S->C过程中S发送的消息
*/
class SCMessage : public Message
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

