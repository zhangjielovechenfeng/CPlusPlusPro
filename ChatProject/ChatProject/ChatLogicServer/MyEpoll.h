#pragma once

#include <sys/epoll.h>
#include "../Util/Util.h"

#define MAX_FD 1024		// 最大句柄数
#define MAX_EVENTS 10	// 最大处理事件数

/*
	IO多路复用实现(epoll)
*/

typedef struct epoll_event EpollEvent;
class MyEpoll
{
public:
	MyEpoll();
	~MyEpoll();

	int GetEpollFd();
	EpollEvent* GetRecvEvents();

public:
	bool EpollCreate();
	bool EpollAdd(int sessionID, UINT listenEvent = EPOLLIN | EPOLLET);
	bool EpollMod(int sessionID, UINT listenEvent = EPOLLOUT | EPOLLET);
	bool EpollDel(int sessionID);
	int  EpollWait();
	bool CanReadData(int eventIndex);
	bool CanWriteData(int eventIndex);

private:
	int				m_epollFd;					// epoll对象
	EpollEvent		m_listenEvent;				// 监听事件
	EpollEvent		m_recvEvent[MAX_EVENTS];	// 接收事件
};

