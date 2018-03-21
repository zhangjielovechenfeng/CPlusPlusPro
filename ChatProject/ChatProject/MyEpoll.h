#pragma once

#include <sys/epoll.h>

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
	bool EpollAdd(int socketFd, uint32_t listenEvent = EPOLLIN);
	bool EpollMod(int socketFd, uint32_t listenEvent = EPOLLOUT);
	bool EpollDel(int socketFd, uint32_t listenEvent);
	int  EpollWait();
	bool CanReadData(int eventIndex);
	bool CanWriteData(int eventIndex);

private:
	int				m_epollFd;					// epoll对象
	EpollEvent		m_listenEvent;				// 监听事件
	EpollEvent		m_recvEvent[MAX_EVENTS];	// 接收事件
};

