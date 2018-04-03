#pragma once
#include <string>
#include <websocketpp/sha1/sha1.hpp>
#include <websocketpp/base64/base64.hpp>

#define STORE_SHA1_RESULT_BUFF_SIZE 64	// 存储sha1结果的buff大小 
#define MAX_KEY_LEN 64

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

	// 获取服务器握手信息
	string& GetServerShakeHandsMsg();

	// 是否是websocket连接
	bool IsWebSocketConn();

	// 设置 client握手信息
	void SetClientShakeHandsMsg(char* clientShakeHandsMsg, int len);

public:
	// 抓取客户端握手key
	bool _FetchClientKey();

	// 生成服务器握手key
	bool _GenerateServerKey();

	// 生成服务器握手信息
	bool _GenerateServerShakeHandsMsg();
private:
	char*		m_clientShakeHandsMsg; // client握手数据
	string		m_serverShakeHandsMsg;
	int			m_len;
	char		m_clientKeyBody[MAX_KEY_LEN];
	string		m_serverKeyBody;
};

