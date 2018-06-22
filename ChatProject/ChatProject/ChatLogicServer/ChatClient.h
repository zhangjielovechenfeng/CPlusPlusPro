#pragma once

#include "Message.h"
#include <string>
#include "../Util/Util.h"
#include "CSMsgBuff.h"
#include "ChatServer.h"
#include "WebSocketHandle.h"

using namespace std;

/*
	����ͻ��ˣ�websocket��Ҫ�ɹ����ֺ�Ž���������
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
	time_t& GetRecvTickTime();
	void SetRecvTickTime(time_t recvTickTime);

public:
	bool InitChatClient(SockAddr_In clientAddr);

	bool SaveMsgData(char* data, uint32_t dataLen);

	// �ͷ����Ƿ�Ͽ�����
	bool IsDisconnect(char* clientData, int dataLen);

	// ��Ϣ����
	bool HandleMsg();
private:
	// websocket���ִ���
	bool _WebSocketShakeHandsHandle();
private:
	int				m_sessionID;
	int				m_port;
	string			m_ip;
	CSMsgBuff		m_recvMsgData;
	bool			m_isBuildLongConn;	// �Ƿ����˳�����
	time_t			m_recvTickTime;		// ���յ�������ʱ�� 
	WebSocketHandle m_webSocketHandle;  // Websocket������
};

