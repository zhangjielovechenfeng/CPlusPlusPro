#pragma once

#include "ThreadLock.h"

template<typename T>
class Singleton
{
protected:
	Singleton() {};
	virtual		~Singleton() {};

private:
	Singleton(const Singleton&) ;
	Singleton&	operator=(const Singleton&);

public:
	static T&	Instance();

private:
	static T* volatile m_instance;
	static bool volatile m_flag;
};

