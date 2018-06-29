#include "MessageHandle.h"



MessageHandle::MessageHandle()
{
	m_msg = NULL;
}


MessageHandle::~MessageHandle()
{
}

bool MessageHandle::InitMessgeHandle(MessageBase * msg)
{
	m_msg = msg;
	return true;
}
