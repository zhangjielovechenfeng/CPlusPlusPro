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

	// 是否断开连接,数据包类型（frame type）,占4bits
	// 0x0：标识一个中间数据包
	// 0x1：标识一个text类型数据包
	// 0x2：标识一个binary类型数据包
	// 0x3 - 7：保留
	// 0x8：标识一个断开连接类型数据包
	// 0x9：标识一个ping类型数据包
	// 0xA：表示一个pong类型数据包
	// 0xB - F：保留
	bool IsDisconnect(char* clientData, int dataLen);

private:
	// 抓取客户端握手key
	string _FetchClientKey(char* clientData, int dataLen);

	// 生成服务器握手key
	string _GenerateServerKey(string& clientKeyBody);
};

