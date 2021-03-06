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
#include "../Util/Time/TimeWheelManager.h"

ChatServer::ChatServer()
{
	m_socketFd = 0;
}


ChatServer::~ChatServer()
{
	SAFE_DELETE(m_epoll);
}

bool ChatServer::Init()
{
	if (ERROR_CODE_NONE != InitChatServerConn())
	{
		LOG_ERR("Init Chat Server Connect Failed!!!");
		return false;
	}
	if (!TimeWheelManager::Instance().InitTimeWheelManager(4, 100, 100))
	{
		LOG_ERR("Init TimeWheel Manager Failed!!!");
		return false;
	}

	return true;
}

int ChatServer::InitChatServerConn()
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

bool ChatServer::Run()
{
	if (!RunTimeWheelThread())
	{
		return false;
	}
	if (!ChatClientManager::Instance().RunTickTimer())
	{
		LOG_ERR("Run Tick Timer Failed!!!");
		return false;
	}

	if (ERROR_CODE_NONE != RunChatServer())
	{
		LOG_ERR("Chat Server Run Error!!!");
		return false;
	}

	return true;
}

int ChatServer::RunChatServer()
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
				int ret = _SocketAccept();
				if (ret != ERROR_CODE_NONE)
				{
					LOG_ERR("Socket Accpet Error!!!, errno : %d", ret);
					continue;
				}
			}
			else if (m_epoll->CanReadData(i)) // 用户已连接进行读取
			{
				int sessionID = m_epoll->GetRecvEvents()[i].data.fd;
				if (sessionID < 0)
				{
					LOG_ERR("Socket Session[%d] Error!!!", sessionID);
					close(sessionID);
					continue;
				}

				int ret = _RecvMsg(sessionID);
				if (ret != ERROR_CODE_NONE)
				{
					m_epoll->EpollDel(sessionID);
					ChatClientManager::Instance().DelChatClient(sessionID);
					continue;
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
	int sessionID = 0;
	while ((sessionID = accept(m_socketFd, (SockAddr*)&clientAddr, &clientAddSize)) > 0)
	{
		// 相当于顶号
		ChatClient* chatClient = ChatClientManager::Instance().GetChatClient(sessionID);
		if (chatClient != NULL)
		{
			ChatClientManager::Instance().DelChatClient(sessionID);
		}

		if (!ChatClientManager::Instance().AddChatClient(sessionID, clientAddr))
		{
			return ERROR_CODE_INSERT_ONLINE_FAILED;
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
	if (sessionID == -1)
	{
		if (EAGAIN != errno && ECONNABORTED != errno && EPROTO != errno && EINTR != errno)
		{
			LOG_ERR("Socket Accept Client Failed!!!,error:[%s]", strerror(errno));
			return ERROR_CODE_SOCKET_ACCEPT_FAILED;
		}
	}
	
	return ERROR_CODE_NONE;
}

void ChatServer::Stop()
{
	m_timerWheelThread->interrupt();
	m_timerWheelThread->join();
	close(m_socketFd);
}

bool ChatServer::_SetNonBlock(int fd)
{
	int opt = fcntl(fd, F_GETFL);
	if (opt < 0)
	{
		LOG_ERR("fcntl(%d, F_GETFL) failed! error:%s", fd, strerror(errno));
		return false;
	}

	opt = opt | O_NONBLOCK | O_NDELAY;
	if (fcntl(fd, F_SETFL, opt) < 0)
	{
		LOG_ERR("fcntl(%d, F_GETFL, %d) failed! error:%s", fd, opt, strerror(errno));
		return false;
	}
	return true;
}

int ChatServer::_RecvMsg(int sessionID)
{
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
				continue;
			}
			LOG_ERR("Recv Data Error!!!error: %s", strerror(errno));
			return ERROR_CODE_RECV_MSG_ERROR;
		}
		ChatClient* chatClient = ChatClientManager::Instance().GetChatClient(sessionID);
		ASSERT_RETURN(chatClient != NULL, ERROR_CODE_CLIENT_NOT_EXIST);

		// 判断是否断开连接
		if (chatClient->IsDisconnect(recvMsgBuff, recvSize))
		{
			// 外层会进行断开连接返回false
			return false;
		}

		// 存储接收的消息
		if (!chatClient->SaveMsgData(recvMsgBuff, (uint32_t)recvSize))
		{
			LOG_ERR("Insert Msg Buff Failed!!!");
			return ERROR_CODE_INSERT_MSG_TO_BUFF_FAILED;
		}
		memset(recvMsgBuff, 0, DATA_BUFF_SIZE);

		// 消息处理
		if (!chatClient->HandleMsg())
		{
			LOG_ERR("Handle Msg Fail!!!");
			return ERROR_CODE_MSG_HANDLE_FAIL;
		}
	}
	return ERROR_CODE_NONE;
}

bool ChatServer::RunTimeWheelThread()
{
	TimeWheelManager& timeWheelManager = TimeWheelManager::Instance();
	m_timerWheelThread = new boost::thread(boost::bind(&TimeWheelManager::Run, &timeWheelManager));
	return true;
}

bool ChatServer::SendMessage(MessageBase * message)
{
	ASSERT_RETURN(message != NULL, false);
	message->HandleMsgData();// 消息处理

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
	// websocket协议打包
	string factData = WebSocketHandle::Instance().PackServerData(message);
	if (factData.empty())
	{
		LOG_ERR("Pack Server Data Failed!!!");
		return false;
	}

	size_t msgSize = factData.size();
	ssize_t factSendSize = 0;

	// 发送数据， ET模式下，当数据没有发完时，不会继续触发EPOLLOUT事件，因此需要循环检查发送数据
	while (msgSize > 0)
	{
		factSendSize = send(sessionID, factData.c_str() + factData.size() - msgSize, msgSize, 0);
		if (factSendSize < 0 && errno != EAGAIN)
		{
			LOG_ERR("Send Data Error!!!, error: %s", strerror(errno));
			return false;
		}
		msgSize -= factSendSize;
	}
	return true;
}
