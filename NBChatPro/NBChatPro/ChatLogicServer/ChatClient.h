#pragma once

#include "Message.h"
#include <string>
#include "../Util/Util.h"
#include "RecvMsgBuff.h"
#include "ChatServer.h"

using namespace std;

/*
	ÁÄÌì¿Í»§¶Ë
*/
class ChatClient
{
public:
	ChatClient(int connFd);
	~ChatClient();
public:
	string & GetIP();

public:
	bool InitChatClient(SockAddr_In clientAddr);

	bool SaveMsgData(char* data, UINT dataLen);
private:
	int				m_connFd;
	int				m_port;
	string			m_ip;
	Message			m_msg;
	RecvMsgBuff		m_recvMsgData;
};

