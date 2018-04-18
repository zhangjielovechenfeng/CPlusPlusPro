#include "CSMsgPkgDef.h"

uint32_t CSMsgPkg::GetMsgID()
{
	return m_msgID;
}

void CSMsgPkg::SetMsgID(uint32_t msgID)
{
	m_msgID = msgID;

	// ������ϢIDʵ������Ϣ�İ���
	m_msgPkgBody = CSMsgPkgBodyFactory::Instance().CreateMsgPkgBody(msgID);
	m_msgPkgBody->Clear();
}

uint32_t CSMsgPkg::GetMsgPkgLen()
{
	return m_msgPkgLen;
}

void CSMsgPkg::SetMsgPkgLen(uint32_t msgPkgLen)
{
	m_msgPkgLen = msgPkgLen;
}

Message * CSMsgPkg::GetMsgPkgBody()
{
	return m_msgPkgBody;
}