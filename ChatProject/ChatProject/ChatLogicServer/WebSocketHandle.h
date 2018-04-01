#pragma once
#include <string>
#include <websocketpp/sha1/sha1.hpp>
#include <websocketpp/base64/base64.hpp>

#define STORE_SHA1_RESULT_BUFF_SIZE 64	// 存储sha1结果的buff大小 

using namespace websocketpp::sha1;
using namespace websocketpp;
using namespace std;
/*
	WebSocket处理器
*/
class WebSocketHandle
{
public:
	WebSocketHandle();
	~WebSocketHandle();

	// 处理
	bool Handle();

	string& GetServerShakeHandsMsg();

	// 是否是websocket连接
	bool IsWebSocketConn();

	void SetShakeHandsMsg(char* shakeHandsMsg, int len);

public:

	bool _FetchClientKey();

	bool _GenerateServerKey();

	bool _GenerateServerShakeHandsMsg();
private:
	char*		m_clientShakeHandsMsg; // client握手数据
	string		m_serverShakeHandsMsg;
	int			m_len;
	string		m_clientKeyBody;
	string		m_serverKeyBody;
};

