#pragma once

#include "ServerDataDefine.h"
#include <iostream>
#include <sys/epoll.h>

using namespace std;
/*
	ChatServer :  ���������
*/
class ChatServer
{
public:
	ChatServer();
	~ChatServer();

public:
	bool InitChatServer();


};

