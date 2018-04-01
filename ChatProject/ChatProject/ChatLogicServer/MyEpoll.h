#pragma once

#include <sys/epoll.h>
#include "../Util/Util.h"

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
	bool EpollAdd(int sessionID, UINT listenEvent = EPOLLIN | EPOLLET);
	bool EpollMod(int sessionID, UINT listenEvent = EPOLLOUT | EPOLLET);
	bool EpollDel(int sessionID);
	int  EpollWait();
	bool CanReadData(int eventIndex);
	bool CanWriteData(int eventIndex);

private:
	int				m_epollFd;					// epoll����
	EpollEvent		m_listenEvent;				// �����¼�
	EpollEvent		m_recvEvent[MAX_EVENTS];	// �����¼�
};

