#pragma once

#include "Winsock2.h"
#include "ServerDataDefine.h"

/*
	ChatServer :  ���������
*/
class ChatServer
{
public:
	ChatServer();
	~ChatServer();

public:
	SOCKET InitChatServer();


};

