#include "ChatServer.h"
#include <fcntl.h>
#include <sys/socket.h> 
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "ChatClientManager.h"
#include "../Util/LogPrint.h"
#include "../Util/ErrDefine.h"
#include "../Util/Util.h"
#include "WebSocketHandle.h"

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

	// 初始化心跳机制
	ChatClientManager::Instance().InitTickTimer();

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
				int sessionID = _SocketAccept();
				if (sessionID < 0)
				{
					return ERROR_CODE_SOCKET_ACCEPT_FAILED;
				}

				if (!_SetNonBlock(sessionID))
				{
					return ERROR_CODE_SET_NONBLOCK_FAILED;
				}

				if (!m_epoll->EpollAdd(sessionID))
				{
					return ERROR_CODE_EPOLL_ADD_FAILED;
				}
			}
			else if (m_epoll->CanReadData(i)) // 用户已连接进行读取
			{
				int sessionID = m_epoll->GetRecvEvents()[i].data.fd;
				if (sessionID < 0)
				{
					continue;
				}

				int ret = _RecvMsg(sessionID);
				if (ret != ERROR_CODE_NONE)
				{
					m_epoll->EpollDel(sessionID);
					close(sessionID);
					continue;
				}

				if (!m_epoll->EpollMod(sessionID))
				{
					return ERROR_CODE_EPOLL_MOD_FAILED;
				}
			}
			//else if(m_epoll->CanWriteData(i))
			//{
			//	int sessionID = m_epoll->GetRecvEvents()[i].data.fd;
			//	if (sessionID < 0)
			//	{
			//		continue;
			//	}
			//	ChatClient* chatClient = ChatClientManager::Instance().GetChatClient(sessionID);
			//	ASSERT_RETURN(chatClient != NULL, ERROR_CODE_CLIENT_NOT_EXIST);

			//	//给发送消息加锁
			//	ThreadLock lock();
			//	if (!_SendMsg(sessionID))
			//	{
			//		LOG_ERR("Send Message Error!!!");
			//		return ERROR_CODE_SEND_MSG_ERROR;
			//	}
			//	
			//	if (!m_epoll->EpollMod(sessionID, EPOLLIN))
			//	{
			//		return ERROR_CODE_EPOLL_MOD_FAILED;
			//	}
			//}
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

	if (bind(m_socketFd, (SockAddr*)&serverAddr, sizeof(SockAddr)) < 0)
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
	int sessionID = accept(m_socketFd, (SockAddr*)&clientAddr, &clientAddSize);
	if (sessionID < 0)
	{
		LOG_ERR("Socket Accept Client Failed!!!,error:[%s]", strerror(errno));
		return ERROR_CODE_SOCKET_ACCEPT_FAILED;
	}
	// 相当于顶号
	ChatClient* chatClient = ChatClientManager::Instance().GetChatClient(sessionID);
	if (chatClient != NULL)
	{
		close(chatClient->GetSessionID());
		ChatClientManager::Instance().DelChatClient(sessionID);
	}

	if (!ChatClientManager::Instance().AddChatClient(sessionID, clientAddr))
	{
		return ERROR_CODE_INSERT_ONLINE_FAILED;
	}
	return sessionID;
}

void ChatServer::Stop()
{
	close(m_socketFd);
}

bool ChatServer::_SetNonBlock(int fd)
{
	int opt = fcntl(fd, F_GETFL);
	if (opt < 0)
	{
		LOG_ERR("fcntl(%d, F_GETFL) failed! error:%s", m_socketFd, strerror(errno));
		return false;
	}

	opt = opt | O_NONBLOCK | O_NDELAY;
	if (fcntl(fd, F_SETFL, opt) < 0)
	{
		LOG_ERR("fcntl(%d, F_GETFL, %d) failed! error:%s", m_socketFd, opt, strerror(errno));
		return false;
	}
	return true;
}

int ChatServer::_RecvMsg(int sessionID)
{
 	ChatClient* chatClient = ChatClientManager::Instance().GetChatClient(sessionID);
	ASSERT_RETURN(chatClient != NULL, ERROR_CODE_CLIENT_NOT_EXIST);

	char recvMsgBuff[DATA_BUFF_SIZE];
	memset(recvMsgBuff, 0, DATA_BUFF_SIZE);

	ssize_t recvSize = 0;
	while ((recvSize = recv(sessionID, recvMsgBuff, DATA_BUFF_SIZE, 0)) > 0)
	{
		if (0 == recvSize)
		{
			// Client 主动断开连接
			if (!ChatClientManager::Instance().DelChatClient(sessionID))
			{
				return ERROR_CODE_CLIENT_OFFLINE_FAILED;
			}
			return ERROR_CODE_CLIENT_HAD_OFFLINE;
		}
		else if (recvSize < 0)
		{
			// 此三种情况连接为正常
			if (EINTR == errno)
			{
				continue;
			}
			else if(EWOULDBLOCK == errno || EAGAIN == errno)
			{
				return ERROR_CODE_NONE;
			}
			LOG_ERR("Recv Data Error!!!error: %s", strerror(errno));
			return ERROR_CODE_RECV_MSG_ERROR;
		}

		// 存储接收的消息
		if (!chatClient->SaveMsgData(recvMsgBuff, (uint32_t)recvSize))
		{
			LOG_ERR("Insert Msg Buff Failed!!!");
			return ERROR_CODE_INSERT_MSG_TO_BUFF_FAILED;
		}
		memset(recvMsgBuff, 0, DATA_BUFF_SIZE);
	}
	

	if (!chatClient->IsBuildLongConn())
	{
		if (!_WebSocketShakeHandsHandle(chatClient))
		{
			return ERROR_CODE_WEBSOCKET_SHAKE_HANDS_FAILED;
		}
	}

	return ERROR_CODE_NONE;
}

bool ChatServer::_WebSocketShakeHandsHandle(ChatClient* chatClient)
{
	ASSERT_RETURN(chatClient != NULL, false);
	// 还没有建立长连接，处理握手包
	WebSocketHandle webSocketHandle;
	CSMsgBuff& csMsgBuff = chatClient->GetCSMsgBuff();
	webSocketHandle.SetClientShakeHandsMsg(csMsgBuff.GetRecvBuff(), csMsgBuff.GetCurrBuffLen());
	if (!webSocketHandle.IsWebSocketConn())
	{
		// 普通socket连接，本身就是长链接
		chatClient->SetIsBuildLongConn(true);
	}
	else
	{
		// websocket连接，处理握手包
		if (!webSocketHandle.Handle())
		{
			LOG_ERR("Server Shake Hands Msg Generate Failed!!!");
			return ERROR_CODE_GENERATE_SHAKE_HANDS_MSG_FAILED;
		}
		string& msg = webSocketHandle.GetServerShakeHandsMsg();
		if (!SendMessage(chatClient->GetSessionID(), msg)) // 发送握手包信息， 直接send
		{
			LOG_ERR("Send Shake Hands Msg Failed");
			return ERROR_CODE_SEND_SHAKE_HANDS_MSG_FAILED;
		}
		LOG_RUN("The Handshake With Websocket[ip: %s, port: %d] Has Been Established!!!", chatClient->GetIP().c_str(), chatClient->GetPort());
		printf("The Handshake With Websocket[ip: %s, port: %d] Has Been Established!!!", chatClient->GetIP().c_str(), chatClient->GetPort());
	}
	return false;
}


bool ChatServer::SendMessage(Message * message)
{
	ASSERT_RETURN(message != NULL, false);
	SCMessage* scMsg = dynamic_cast<SCMessage*>(message);
	if (NULL == scMsg)
	{
		LOG_ERR("Dynamic_cast Failed!!!");
		return false;
	}
	string& data = scMsg->GetSerializeData();

	if (!SendMessage(scMsg->GetSessionID(), data))
	{
		return false;
	}
	return true;
}

bool ChatServer::SendMessage(int sessionID, string & message)
{
	size_t msgSize = message.size();
	ssize_t factSendSize = 0;

	// 发送数据， ET模式下，当数据没有发完时，不会继续触发EPOLLOUT事件，因此需要循环检查发送数据
	while (msgSize > 0)
	{
		factSendSize = send(sessionID, message.c_str() + message.size() - msgSize, msgSize, 0);
		if (factSendSize < 0 && errno != EAGAIN)
		{
			LOG_ERR("Send Data Error!!!, error: %s", strerror(errno));
			return false;
		}
		msgSize -= factSendSize;
	}
	return true;
}
