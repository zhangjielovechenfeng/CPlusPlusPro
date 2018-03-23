#pragma once

#include "ThreadLock.h"

template<typename T>
class Singleton
{
protected:
	Singleton() {};
	virtual		~Singleton() {};

private:
	Singleton(const Singleton&) {};
	Singleton&	operator=(const Singleton&) {};

public:
	static T&	Instance()
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

private:
	static T* volatile m_instance;
	static bool volatile m_flag;
};

template<typename T>
T* volatile Singleton<T>::m_instance = NULL;

template<typename T>
bool volatile Singleton<T>::m_flag = false;
