#include "ChatServer.h"
#include <fcntl.h>
#include "../Util/LogPrint.h"
#include <sys/socket.h> 
#include <string.h>
#include <unistd.h>
#include "../Util/ErrDefine.h"
#include <errno.h>
#include "ChatClientManager.h"
#include "../Util/Util.h"

ChatServer::ChatServer()
{
	m_socketFd = 0;
}


ChatServer::~ChatServer()
{
	SAFE_DELETE(m_epoll);
}

int ChatServer::InitChatServer()
{
	if (!_CreateScoketConnect())
	{
		return ERROR_CODE_CREATE_SOCKET_FAILED;
	}

	if (!_SetNonBlock(m_socketFd))
	{
		return ERROR_CODE_SET_NONBLOCK_FAILED;
	}

	int optval = 1;
	if (setsockopt(m_socketFd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
	{
		LOG_ERR("Set listen socket<%d> failed! error:%s", m_socketFd, strerror(errno));
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

	LOG_RUN("Chat Server Init OK!!!");
	cout << "Chat Server Init OK!!!" << endl;

	return ERROR_CODE_NONE;
}

int ChatServer::Run()
{
	m_epoll = new MyEpoll();

	// 给socket描述符注册事件
	if (!m_epoll->EpollAdd(m_socketFd))
	{
		return ERROR_CODE_EPOLL_ADD_FAILED;
	}
	while (1)
	{
		int readyFdNum = m_epoll->EpollWait();// 等待epoll事件发生

		for (int i = 0; i < readyFdNum; ++i)
		{
			if (m_epoll->GetRecvEvents()[i].data.fd == m_socketFd) // 检测到有client连接
			{
				int connFd = _SocketAccept();
				if (connFd < 0)
				{
					return ERROR_CODE_SOCKET_ACCEPT_FAILED;
				}

				if (!_SetNonBlock(connFd))
				{
					return ERROR_CODE_SET_NONBLOCK_FAILED;
				}

				if (!m_epoll->EpollAdd(connFd))
				{
					return ERROR_CODE_EPOLL_ADD_FAILED;
				}
			}
			else if (m_epoll->CanReadData(i)) // 用户已连接进行读取
			{
				int connFd = m_epoll->GetRecvEvents()[i].data.fd;
				if (connFd < 0)
				{
					continue;
				}

				int ret = _RecvMsgData(connFd);
				if (ret != ERROR_CODE_NONE)
				{
					close(connFd);
					continue;
				}

				if (!m_epoll->EpollMod(connFd))
				{
					return ERROR_CODE_EPOLL_MOD_FAILED;
				}
			}
			else if(m_epoll->CanWriteData(i))
			{
				int connFd = m_epoll->GetRecvEvents()[i].data.fd;
				if (connFd < 0)
				{
					continue;
				}

				//给发送消息加锁
				ThreadLock lock();

				if (!_SendMsgData(connFd))
				{
					LOG_ERR("Send Message Error!!!");
					return ERROR_CODE_SEND_MSG_ERROR;
				}

				if (!m_epoll->EpollMod(connFd, EPOLLIN))
				{
					return ERROR_CODE_EPOLL_MOD_FAILED;
				}
			}
		}
	}
	return ERROR_CODE_NONE;
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

int ChatServer::_SocketAccept()
{
	SockAddr_In clientAddr;
	socklen_t clientAddSize = sizeof(SockAddr_In);
	int connFd = accept(m_socketFd, (SockAddr*)&clientAddr, &clientAddSize);
	if (connFd < 0)
	{
		LOG_ERR("Socket Accept Client Failed!!!,error:[%s]", strerror(errno));
		return ERROR_CODE_SOCKET_ACCEPT_FAILED;
	}

	if (!ChatClientManager::Instance().AddChatClient(connFd, clientAddr))
	{
		return ERROR_CODE_INSERT_ONLINE_FAILED;
	}
	return connFd;
}

void ChatServer::Stop()
{
	close(m_socketFd);
}

bool ChatServer::_SetNonBlock(int fd)
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

int ChatServer::_RecvMsgData(int connFd)
{
	ChatClient* chatClient = ChatClientManager::Instance().GetChatClient(connFd);
	ASSERT_RETURN(chatClient != NULL, ERROR_CODE_CLIENT_NOT_EXIST);

	char recvMsgBuff[DATA_BUFF_SIZE];
	memset(recvMsgBuff, 0, DATA_BUFF_SIZE);
	ssize_t  factRecvSize = recv(connFd, recvMsgBuff, DATA_BUFF_SIZE, 0);
	if (0 == factRecvSize)
	{
		// Client 主动断开连接

		if (!ChatClientManager::Instance().DelChatClient(connFd))
		{
			return ERROR_CODE_CLIENT_OFFLINE_FAILED;
		}
		return ERROR_CODE_CLIENT_HAD_OFFLINE;
	}
	else if (factRecvSize < 0)
	{
		// 此三种情况连接为正常
		if (EINTR == errno || EWOULDBLOCK == errno || EAGAIN == errno)
		{
			return ERROR_CODE_NONE;
		}
		LOG_ERR("Recv Data Error!!!error: %s", strerror(errno));
		return ERROR_CODE_RECV_MSG_ERROR;
	}

	// 存储接收的消息
	if(!chatClient->SaveMsgData(recvMsgBuff, (UINT)factRecvSize))
	{
		LOG_ERR("Insert Msg Buff Failed!!!");
		return ERROR_CODE_INSERT_MSG_TO_BUFF_FAILED;
	}
	return ERROR_CODE_NONE;
}

bool ChatServer::_SendMsgData(int connFd)
{
	return true;
}
