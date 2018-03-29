#include "CSMsgBuff.h"
#include <string.h>
#include <cstdio>
#include "Message.h"

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
	// 如果可以解析，就解析，然后处理该包消息
	if (IsNeedParseBuff())
	{
		CSMessage csMessage;
		csMessage.HandleMsgData(m_recvBuff);
	}

	return true;
}

bool CSMsgBuff::IsNeedParseBuff()
{
	return false;
}
