#include "CSMsgBuff.h"
#include <string.h>
#include <cstdio>

CSMsgBuff::CSMsgBuff()
{
	memset(m_recvBuff, 0, MAX_DATA_LEN);
	m_currBuffLen = 0;
}


CSMsgBuff::~CSMsgBuff()
{
}

bool CSMsgBuff::InsertDataToBuff(char * data, UINT dataLen)
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

bool CSMsgBuff::IsNeedParseBuff()
{
	return true;
}
