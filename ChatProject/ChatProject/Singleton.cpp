#include "Singleton.h"

template<typename T>
T* volatile Singleton<T>::m_instance = NULL;

template<typename T>
bool volatile Singleton<T>::m_flag = false;

template<typename T>
Singleton & Singleton<T>::operator=(const Singleton & obj)
{
	return obj;
}

template<typename T>
T & Singleton<T>::Instance()
{
	if (false == m_flag)
	{
		ThreadLock lock();
		if (false == m_flag)
		{
			m_instance = new T;
			m_flag = true;               
		}
	}

	return *m_instance;
}
