#pragma once

#include <iostream>
#include "MyEpoll.h"
#include <netinet/in.h>
#include <map>
#include "Message.h"
#include <string>

#define SERVER_PORT 8888		// �������˿ں�
#define LISTEN_LEN 1024			// �������г���
#define DATA_BUFF_SIZE 1024	    // ����buff��С

using namespace std;
/*
	ChatServer :  ���������
*/

typedef struct sockaddr_in	SockAddr_In;
typedef struct sockaddr		SockAddr;
typedef map<int /*fd*/,Message*>	SendMsgMap;	// ��Ϣ����map

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

	// ͨ��sessionID��ȡmessage
	Message* GetMessageBySessionID(int sessionID);

	// ͨ��sessionIDɾ��message
	bool DelMessageBySessionID(int sessionID);

	// �ɷ���Ϣ
	bool SendMessage(Message* message);

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

	// ������Ϣ
	bool _SendMsg(int sessionID);

	// ����������Ϣ
	bool _SendShakeHandsMsg(int sessionID, string& msg);

	// websocket���ִ���
	bool _WebSocketShakeHandsHandle();

	// ����µ�message��Map
	bool _AddMessageToMessage(int sessionID, Message* message);
private:
	MyEpoll*		m_epoll;
	int				m_socketFd;
	SendMsgMap		m_sendMsgMap; // ��Ϣ����map
};

