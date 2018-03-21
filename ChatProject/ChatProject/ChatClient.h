#pragma once

#include "Message.h"
#include <string>

using namespace std;

/*
	����ͻ���
*/
class ChatClient
{
public:
	ChatClient();
	~ChatClient();

public:
	bool InitChatClient();

	bool RecvMsgData();
private:
	int			m_socketFd;
	string		m_ip;
	Message		m_msg;

};

