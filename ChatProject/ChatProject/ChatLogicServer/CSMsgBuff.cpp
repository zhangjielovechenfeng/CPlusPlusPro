#include "CSMsgBuff.h"
#include <string.h>
#include <cstdio>
#include "Message.h"
#include "../Util/OutOfOrderTool.h"
#include "../Util/CommonDef.h"

CSMsgBuff::CSMsgBuff()
{
	memset(m_recvBuff, 0, MAX_DATA_LEN);
	m_currBuffLen = 0;
}


CSMsgBuff::~CSMsgBuff()
{
}

char * CSMsgBuff::GetRecvBuff()
{
	return m_recvBuff;
}

uint32_t CSMsgBuff::GetCurrBuffLen()
{
	return m_currBuffLen;
}

bool CSMsgBuff::InsertDataToBuff(char * data, uint32_t dataLen)
{
	if (dataLen > MAX_DATA_LEN || dataLen + m_currBuffLen > MAX_DATA_LEN)
	{
		return false;
	}
	memcpy(m_recvBuff + m_currBuffLen, data, dataLen);
	m_currBuffLen += dataLen;
	m_recvBuff[m_currBuffLen] = 0;

	return true;
}

void CSMsgBuff::ClearBuff(uint32_t len)
{
	memset(m_recvBuff, 0, len);
	m_currBuffLen = 0;
}
