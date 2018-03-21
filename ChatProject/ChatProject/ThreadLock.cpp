#include "ThreadLock.h"

ThreadLock::ThreadLock()
{
	pthread_mutex_init(&m_mutex, NULL);
	pthread_mutex_lock(&m_mutex);
}


ThreadLock::~ThreadLock()
{
	pthread_mutex_unlock(&m_mutex);
	pthread_mutex_destroy(&m_mutex);
}
