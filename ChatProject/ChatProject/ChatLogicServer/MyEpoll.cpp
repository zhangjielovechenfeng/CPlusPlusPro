#include "MyEpoll.h"
#include "../Util/LogPrint.h"
#include <string.h>
#include "../Util/ErrDefine.h"

MyEpoll::MyEpoll()
{
	m_epollFd = 0;
	memset(&m_listenEvent, 0, sizeof(m_listenEvent));
	memset(m_recvEvent, 0, sizeof(*m_recvEvent));

	EpollCreate();
}


MyEpoll::~MyEpoll()
{
}

int MyEpoll::GetEpollFd()
{
	return m_epollFd;
}

EpollEvent * MyEpoll::GetRecvEvents()
{
	return m_recvEvent;
}

bool MyEpoll::EpollCreate()
{
	m_epollFd = epoll_create(MAX_FD);
	if (m_epollFd < 0)
	{
		LOG_ERR("Epoll Create Failed!!!");
		return false;
	}
	return true;
}

bool MyEpoll::EpollAdd(int sessionID, uint32_t listenEvent)
{
	m_listenEvent.events = listenEvent;
	m_listenEvent.data.fd = sessionID;

	int result = epoll_ctl(m_epollFd, EPOLL_CTL_ADD, sessionID, &m_listenEvent);
	if (result < 0)
	{
		LOG_ERR("Add Epoll Event Failed!!!");
		return false;
	}
	return true;
}

bool MyEpoll::EpollMod(int sessionID, uint32_t listenEvent)
{
	m_listenEvent.events = listenEvent;
	m_listenEvent.data.fd = sessionID;

	int result = epoll_ctl(m_epollFd, EPOLL_CTL_MOD, sessionID, &m_listenEvent);
	if (result < 0)
	{
		LOG_ERR("Mod Epoll Event Failed!!!");
		return false;
	}
	return true;
}

bool MyEpoll::EpollDel(int sessionID)
{
	m_listenEvent.data.fd = sessionID;

	int result = epoll_ctl(m_epollFd, EPOLL_CTL_DEL, sessionID, NULL);
	if (result < 0)
	{
		LOG_ERR("Del Epoll Event Failed!!!");
		return false;
	}
	return true;
}

int MyEpoll::EpollWait()
{
	return epoll_wait(m_epollFd, m_recvEvent, MAX_EVENTS, 500); //ÕâÀïÊÇ500ms
}

bool MyEpoll::CanReadData(int eventIndex)
{
	if (eventIndex < 0 || eventIndex > MAX_EVENTS)
	{
		LOG_ERR("EventIndex Ivaild!!!");
		return false;
	}
	return (m_recvEvent[eventIndex].events & EPOLLIN);
}

bool MyEpoll::CanWriteData(int eventIndex)
{
	if (eventIndex < 0 || eventIndex > MAX_EVENTS)
	{
		LOG_ERR("EventIndex Ivaild!!!");
		return false;
	}
	return (m_recvEvent[eventIndex].events & EPOLLOUT);
}
