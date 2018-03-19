#pragma once

#include "ServerDataDefine.h"
#include <iostream>
#include <sys/epoll.h>

using namespace std;
/*
	ChatServer :  ÁÄÌì·şÎñÆ÷
*/
class ChatServer
{
public:
	ChatServer();
	~ChatServer();

public:
	bool InitChatServer();


};

