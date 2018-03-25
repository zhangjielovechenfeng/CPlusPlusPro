#include "RecvMsgBuff.h"
#include <string.h>
#include <cstdio>

RecvMsgBuff::RecvMsgBuff()
{
	memset(m_recvBuff, 0, MAX_DATA_LEN);
	m_currBuffLen = 0;
}


RecvMsgBuff::~RecvMsgBuff()
{
}

bool RecvMsgBuff::InsertDataToBuff(char * data, UINT dataLen)
{
	if (dataLen > MAX_DATA_LEN || dataLen + m_currBuffLen > MAX_DATA_LEN)
	{
		return false;
	}
	memcpy(m_recvBuff + m_currBuffLen, data, dataLen);
	m_currBuffLen += dataLen;
	m_recvBuff[m_currBuffLen] = 0;

	printf("the data: %s\n", m_recvBuff);
	if (IsNeedParseBuff())
	{
		
	}

	return true;
}

bool RecvMsgBuff::IsNeedParseBuff()
{
	return true;
}
