#pragma once

#include "Message.h"
#include <string>
#include "../Util/Util.h"
#include "CSMsgBuff.h"
#include "ChatServer.h"

using namespace std;

/*
	����ͻ���
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
	CSMsgBuff		m_recvMsgData;
};
