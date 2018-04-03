#pragma once

#include "Message.h"
#include <string>
#include "../Util/Util.h"
#include "CSMsgBuff.h"
#include "ChatServer.h"

using namespace std;

/*
	聊天客户端
*/
class ChatClient
{
public:
	ChatClient(int sessionID);
	~ChatClient();
public:
	string & GetIP();
	int GetPort();
	int GetSessionID();
	bool IsBuildLongConn();
	void SetIsBuildLongConn(bool isBuildLongConn);
	CSMsgBuff& GetCSMsgBuff();

public:
	bool InitChatClient(SockAddr_In clientAddr);

	bool SaveMsgData(char* data, uint32_t dataLen);
private:
	int				m_sessionID;
	int				m_port;
	string			m_ip;
	CSMsgBuff		m_recvMsgData;
	bool			m_isBuildLongConn; // 是否建立了长连接
};

