#include "MessageHandle.h"



MessageHandle::MessageHandle()
{
	m_msg = NULL;
}


MessageHandle::~MessageHandle()
{
}

bool MessageHandle::InitMessgeHandle(Message * msg)
{
	m_msg = msg;
	return false;
}
