#include "MessageHandleSubClass.h"

bool MessageHandleHeartBeatReq::Handle()
{
	SCMessage scMsg(m_msg->GetSessionID());

	m_msg->GetMsgPkg();
	
	return true;;
}
