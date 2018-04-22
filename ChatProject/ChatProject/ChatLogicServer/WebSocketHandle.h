#pragma once
#include <string>
#include <websocketpp/sha1/sha1.hpp>
#include <websocketpp/base64/base64.hpp>
#include "../Util/Singleton.h"

#define STORE_SHA1_RESULT_BUFF_SIZE 64	// 存储sha1结果的buff大小 

using namespace websocketpp::sha1;
using namespace websocketpp;
using namespace std;

/*
	WebSocket处理器
*/
class WebSocketHandle : public Singleton<WebSocketHandle>
{
public:
	WebSocketHandle() {}
	virtual ~WebSocketHandle() {}

	// 是否是websocket连接
	bool IsWebSocketConn(char* clientData);

	// 解析客户端数据
	string ParseClientData(char* clientdata, int dataLen);

	// 打包服务器数据
	string PackServerData(string& serverData);

	// 生成服务器握手信息
	string GenerateServerShakeHandsMsg(char* clientData, int dataLen);

private:
	// 抓取客户端握手key
	string _FetchClientKey(char* clientData, int dataLen);

	// 生成服务器握手key
	string _GenerateServerKey(string& clientKeyBody);
};

