#pragma once

#include <iostream>
#include "MyEpoll.h"
#include <netinet/in.h>
#include <map>
#include "Message.h"
#include <string>
#include <boost/thread.hpp>
#include "../Util/Singleton.h"

#define SERVER_PORT 8888		// �������˿ں�
#define LISTEN_LEN 1024			// �������г���
#define DATA_BUFF_SIZE 8 * 1024	    // ����buff��С

using namespace std;
/*
	ChatServer :  ���������
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

	// ��ʼ������ϵͳ
	bool Init();

	// ��ʼ���������������
	int InitChatServerConn();

	// ��������ϵͳ
	bool Run();

	// ���з�����
	int RunChatServer();

	// �Ͽ�������
	void Stop();

	// �ɷ���Ϣ
	bool SendMessage(Message* message);

	bool SendMessage(int sessionID, string& message);

	// ����ʱ����
	bool RunTimeWheelThread();

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

private:
	MyEpoll*		m_epoll;
	int				m_socketFd;
	boost::thread*  m_timerWheelThread;
};

