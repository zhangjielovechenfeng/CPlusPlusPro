#pragma once

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
	
	virtual void SetMsgData(char* data) = 0;

	char* GetMsgData();

protected:
	char* m_data;		
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
	// 解析消息,并通知消息处理handle
	bool ParseMsgToPkg();

	// 当buff中数据够一个包时，把包存储在此data中,并通知子类解析
	virtual void SetMsgData(char* data);

};

