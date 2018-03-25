#pragma once

#include <pthread.h>

class ThreadLock
{
public:
	ThreadLock();
	~ThreadLock();

private:
	pthread_mutex_t	m_mutex;
};

