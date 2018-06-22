#pragma once

#include <iostream>
#include "MyEpoll.h"
#include <netinet/in.h>
#include <map>
#include "Message.h"
#include <string>
#include <boost/thread.hpp>
#include "../Util/Singleton.h"

#define SERVER_PORT 8888		// 服务器端口号
#define LISTEN_LEN 1024			// 监听队列长度
#define DATA_BUFF_SIZE 8 * 1024	    // 数据buff大小

using namespace std;
/*
	ChatServer :  聊天服务器
*/

typedef struct sockaddr_in	SockAddr_In;
typedef struct sockaddr		SockAddr;

class ChatClient;
class ChatServer : public Singleton<ChatServer>
{
public:
	ChatServer();
	~ChatServer();

public:

	// 初始化聊天系统
	bool Init();

	// 初始化聊天服务器连接
	int InitChatServerConn();

	// 运行聊天系统
	bool Run();

	// 运行服务器
	int RunChatServer();

	// 断开服务器
	void Stop();

	// 派发消息
	bool SendMessage(Message* message);

	bool SendMessage(int sessionID, string& message);

	// 运行时间轮
	bool RunTimeWheelThread();

private:
	// 创建socket连接
	bool _CreateScoketConnect();

	// socket绑定
	bool _SocketBind();
	 
	// socket监听
	bool _SocketListen();

	// 等待client连接请求
	int _SocketAccept();

	// 设置非阻塞模式
	bool _SetNonBlock(int fd);

	// 接收消息
	int _RecvMsg(int sessionID);

private:
	MyEpoll*		m_epoll;
	int				m_socketFd;
	boost::thread*  m_timerWheelThread;
};

