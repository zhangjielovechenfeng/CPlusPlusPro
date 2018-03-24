#pragma once

/*
	Message : 聊天消息类
*/
class Message
{
public:
	Message();
	~Message();
private:
	char* m_data;
};

/*
	C->S过程中S收到的消息
*/
class CSRecvMessage : public Message
{

};

