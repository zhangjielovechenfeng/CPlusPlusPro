#pragma once

#include <sys/epoll.h>

#define MAX_FD 1024		// �������
#define MAX_EVENTS 10	// ������¼���

/*
	IO��·����ʵ��(epoll)
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
	int				m_epollFd;					// epoll����
	EpollEvent		m_listenEvent;				// �����¼�
	EpollEvent		m_recvEvent[MAX_EVENTS];	// �����¼�
};

