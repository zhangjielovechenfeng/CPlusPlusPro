#include "stdafx.h"
#include "ChatServer.h"


ChatServer::ChatServer()
{
}


ChatServer::~ChatServer()
{
}

SOCKET ChatServer::InitChatServer()
{
	WORD sockVersion = MAKEWORD(2, 2);//Winsock�汾
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{

	}
	return SOCKET();
}
