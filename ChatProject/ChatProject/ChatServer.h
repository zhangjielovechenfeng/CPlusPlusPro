#pragma once

#include <iostream>
#include "MyEpoll.h"
#include <netinet/in.h>

#define SERVER_PORT 8888		// �������˿ں�
#define LISTEN_LEN 1024			// �������г���

using namespace std;
/*
	ChatServer :  ���������
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

	//���з�����
	bool Run();

	//�Ͽ�������
	void Stop();

private:
	//����socket����
	bool _CreateScoketConnect();

	//socket��
	bool _SocketBind();

	//socket����
	bool _SocketListen();

	//�ȴ�client��������
	bool _SocketAccept();

	// ���÷�����ģʽ
	bool _SetNonBlock();

private:
	MyEpoll*	m_epoll;
	int			m_socketFd;
};

