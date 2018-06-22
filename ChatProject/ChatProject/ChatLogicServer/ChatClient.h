#pragma once

#include "Message.h"
#include <string>
#include "../Util/Util.h"
#include "CSMsgBuff.h"
#include "ChatServer.h"
#include "WebSocketHandle.h"

using namespace std;

/*
	聊天客户端：websocket需要成功握手后才建立长连接
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

	// 客服端是否断开连接
	bool IsDisconnect(char* clientData, int dataLen);

	// 消息处理
	bool HandleMsg();
private:
	// websocket握手处理
	bool _WebSocketShakeHandsHandle();
private:
	int				m_sessionID;
	int				m_port;
	string			m_ip;
	CSMsgBuff		m_recvMsgData;
	bool			m_isBuildLongConn;	// 是否建立了长连接
	time_t			m_recvTickTime;		// 接收到心跳的时间 
	WebSocketHandle m_webSocketHandle;  // Websocket处理器
};

