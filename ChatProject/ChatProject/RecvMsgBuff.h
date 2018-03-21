#pragma once

#define MAX_DATA_LEN 2*1024*1024	// 接收数据最大长度
#include "Util.h"

class RecvMsgBuff
{
public:
	RecvMsgBuff();
	~RecvMsgBuff();

	void InsertDataToBuff(const char* data, UINT dataLen);

private:
	char* m_recvBuff[MAX_DATA_LEN];
	UINT  m_currBuffLen;
};

