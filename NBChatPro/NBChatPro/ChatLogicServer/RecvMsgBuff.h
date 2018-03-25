#pragma once

#define MAX_DATA_LEN 64	// 接收数据最大长度
#include "../Util/Util.h"

class RecvMsgBuff
{
public:
	RecvMsgBuff();
	~RecvMsgBuff();

	bool InsertDataToBuff(char* data, UINT dataLen);

	// 是否需要解析buff
	bool IsNeedParseBuff();

private:
	char m_recvBuff[MAX_DATA_LEN];
	UINT  m_currBuffLen;
};

