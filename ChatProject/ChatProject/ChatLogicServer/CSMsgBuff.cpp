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

bool CSMsgBuff::IsNeedParseBuff()
{
	if (m_currBuffLen < CS_MSG_PKG_CONSTANT_HEAD_SIZE)
	{
		return false;
	}
	char* tmpData = NULL;
	int pkgbodylen = 0;
	int id = 0;

	// 正序数据
	//tmpData = OutOfOrderTool::PositiveOrder(m_recvBuff, CS_MSG_PKG_CONSTANT_HEAD_SIZE);

	// 提出包体长度
	memcpy(&pkgbodylen, m_recvBuff + CS_MSG_PKG_CONSTANT_HEAD_SIZE / 2, CS_MSG_PKG_CONSTANT_HEAD_SIZE / 2);
	memcpy(&id, m_recvBuff, CS_MSG_PKG_CONSTANT_HEAD_SIZE / 2);

	if (m_currBuffLen < (uint32_t)(pkgbodylen + CS_MSG_PKG_CONSTANT_HEAD_SIZE))
	{
		return false;
	}
	return true;
}

void CSMsgBuff::ClearBuff(uint32_t len)
{
	memset(m_recvBuff, 0, len);
	m_currBuffLen = 0;
}
