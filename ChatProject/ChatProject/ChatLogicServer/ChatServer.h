#pragma once

#include <iostream>
#include "MyEpoll.h"
#include <netinet/in.h>
#include <map>
#include "Message.h"
#include <string>

#define SERVER_PORT 8888		// �������˿ں�
#define LISTEN_LEN 1024			// �������г���
#define DATA_BUFF_SIZE 8 * 1024	    // ����buff��С

using namespace std;
/*
	ChatServer :  ���������
*/

typedef struct sockaddr_in	SockAddr_In;
typedef struct sockaddr		SockAddr;

class ChatServer
{
public:
	ChatServer();
	~ChatServer();

public:
	// ��ʼ�����������
	int InitChatServer();

	// ���з�����
	int Run();

	// �Ͽ�������
	void Stop();

	// �ɷ���Ϣ
	bool SendMessage(Message* message);

	bool SendMessage(int sessionID, string& message);

private:
	// ����socket����
	bool _CreateScoketConnect();

	// socket��
	bool _SocketBind();
	 
	// socket����
	bool _SocketListen();

	// �ȴ�client��������
	int _SocketAccept();

	// ���÷�����ģʽ
	bool _SetNonBlock(int fd);

	// ������Ϣ
	int _RecvMsg(int sessionID);

	// websocket���ִ���
	bool _WebSocketShakeHandsHandle(ChatClient* chatClient);

private:
	MyEpoll*		m_epoll;
	int				m_socketFd;
};

