#pragma once

#include <iostream>
#include "MyEpoll.h"
#include <netinet/in.h>
#include <map>
#include "Message.h"
#include <string>

#define SERVER_PORT 8888		// 服务器端口号
#define LISTEN_LEN 1024			// 监听队列长度
#define DATA_BUFF_SIZE 1024	    // 数据buff大小

using namespace std;
/*
	ChatServer :  聊天服务器
*/

typedef struct sockaddr_in	SockAddr_In;
typedef struct sockaddr		SockAddr;
typedef map<int /*fd*/,Message*>	SendMsgMap;	// 消息发送map

class ChatServer
{
public:
	ChatServer();
	~ChatServer();

public:
	// 初始化聊天服务器
	int InitChatServer();

	// 运行服务器
	int Run();

	// 断开服务器
	void Stop();

	// 通过sessionID获取message
	Message* GetMessageBySessionID(int sessionID);

	// 通过sessionID删除message
	bool DelMessageBySessionID(int sessionID);

	// 派发消息
	bool SendMessage(Message* message);

private:
	// 创建socket连接
	bool _CreateScoketConnect();

	// socket绑定
	bool _SocketBind();
	 
	// socket监听
	bool _SocketListen();

	// 等待client练剑请求
	int _SocketAccept();

	// 设置非阻塞模式
	bool _SetNonBlock(int fd);

	// 接收消息
	int _RecvMsg(int sessionID);

	// 发送消息
	bool _SendMsg(int sessionID);

	// 发送握手消息
	bool _SendShakeHandsMsg(int sessionID, string& msg);

	// websocket握手处理
	bool _WebSocketShakeHandsHandle();

	// 添加新的message到Map
	bool _AddMessageToMessage(int sessionID, Message* message);
private:
	MyEpoll*		m_epoll;
	int				m_socketFd;
	SendMsgMap		m_sendMsgMap; // 消息发送map
};

