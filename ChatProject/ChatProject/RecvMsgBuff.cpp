#include "RecvMsgBuff.h"
#include <string.h>


RecvMsgBuff::RecvMsgBuff()
{
	memset(m_recvBuff, 0, MAX_DATA_LEN);
	m_currBuffLen = 0;
}


RecvMsgBuff::~RecvMsgBuff()
{
}

void RecvMsgBuff::InsertDataToBuff(const char * data, UINT dataLen)
{
	if (dataLen > MAX_DATA_LEN || dataLen + m_currBuffLen > MAX_DATA_LEN)
	{
		return;
	}
	memcpy(m_recvBuff + m_currBuffLen, data, dataLen);
	m_currBuffLen += dataLen;
	m_recvBuff[m_currBuffLen] = 0;
}
