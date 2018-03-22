#pragma once

#include <iostream>
#include "MyEpoll.h"
#include <netinet/in.h>

#define SERVER_PORT 8888		// 服务器端口号
#define LISTEN_LEN 1024			// 监听队列长度
#define DATA_BUFF_SIZE 1024	    // 数据buff大小

using namespace std;
/*
	ChatServer :  聊天服务器
*/

typedef struct sockaddr_in SockAddr_In;
typedef struct sockaddr SockAddr;

class ChatServer
{
public:
	ChatServer();
	~ChatServer();

public:
	int InitChatServer();

	// 运行服务器
	int Run();

	// 断开服务器
	void Stop();

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
	int _RecvMsg(int connFd);

	bool _SendMsg(int connFd);

private:
	MyEpoll*	m_epoll;
	int			m_socketFd;
};

