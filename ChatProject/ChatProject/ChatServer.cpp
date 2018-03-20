#include "ChatServer.h"
#include <fcntl.h>
#include "LogPrint.h"
#include <sys/socket.h> 
#include <string.h>
#include <unistd.h>
#include "ErrDefine.h"

ChatServer::ChatServer()
{
	m_socketFd = 0;
}


ChatServer::~ChatServer()
{
}

int ChatServer::InitChatServer()
{
	if (!_CreateScoketConnect())
	{
		return ERROR_CODE_CREATE_SOCKET_FAILED;
	}

	if (!_SetNonBlock())
	{
		return ERROR_CODE_SET_NONBLOCK_FAILED;
	}

	int optval = 1;
	if (setsockopt(m_socketFd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
	{
		LOG_ERR("Set listen socket<%d> failed! error:%s", listenSocketFd, strerror(errno));
		close(m_socketFd);
		return ERROR_CODE_SET_REUSEADDR_FAILED;
	}
	
	if (!_SocketBind())
	{
		return ERROR_CODE_SOCKET_BIND_FAILED;
	}

	if (!_SocketListen())
	{
		return ERROR_CODE_SOCKET_LISTEN_FAILED;
	}

	if (!_SocketAccept())
	{
		return ERROR_CODE_SOCKET_ACCEPT_FAILED;
	}

	cout << "等待client请求" << endl;

	return ERROR_CODE_NONE;
}

bool ChatServer::Run()
{
	return false;
}

bool ChatServer::_CreateScoketConnect()
{
	m_socketFd = socket(PF_INET, SOCK_STREAM, 0);
	if (m_socketFd < 0)
	{
		LOG_ERR("Create Scoket Connect Failed!!!");
		return false;
	}
	return true;
}

bool ChatServer::_SocketBind()
{
	// 填充服务器信息
	SockAddr_In serverAddr;
	memset(&serverAddr, 0, sizeof(SockAddr_In));
	serverAddr.sin_family = AF_INET;	// 使用 IPv4 进行通信
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(SERVER_PORT);

	if (bind(m_socketFd, (SockAddr*)&serverAddr, sizeof(SockAddr))< 0)
	{
		LOG_ERR("Bind Socket Failed!!!");
		return false;
	}
	return true;
}

bool ChatServer::_SocketListen()
{
	if (listen(m_socketFd, LISTEN_LEN) < 0)
	{
		LOG_ERR("Listen Socket Connect Failed!!!");
		return false;
	}
	return true;
}

bool ChatServer::_SocketAccept()
{
	SockAddr_In clientAddr;
	socklen_t clientAddSize = sizeof(SockAddr_In);
	if (accept(m_socketFd, (SockAddr*)&clientAddr, &clientAddSize) < 0)
	{
		LOG_ERR("Socket Accept Client Failed!!!");
		return false;
	}
	return true;
}

void ChatServer::Stop()
{
	close(m_socketFd);
}

bool ChatServer::_SetNonBlock()
{
	int opt = fcntl(m_socketFd, F_GETFL);
	if (opt < 0)
	{
		LOG_ERR("fcntl(%d, F_GETFL) failed! error:%s", m_socketFd, strerror(errno));
		return false;
	}

	opt = opt | O_NONBLOCK | O_NDELAY;
	if (fcntl(m_socketFd, F_SETFL, opt) < 0)
	{
		LOG_ERR("fcntl(%d, F_GETFL, %d) failed! error:%s", m_socketFd, opt, strerror(errno));
		return false;
	}
	return true;
}
