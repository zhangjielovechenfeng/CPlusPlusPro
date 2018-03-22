#pragma once

#define MAX_DATA_LEN 64	// 接收数据最大长度
#include "Util.h"

class RecvMsgBuff
{
public:
	RecvMsgBuff();
	~RecvMsgBuff();

	bool InsertDataToBuff(char* data, UINT dataLen);

	// 检查在数据buff中是否有一个完整的包
	bool HaveCompletePkgInData();

private:
	char m_recvBuff[MAX_DATA_LEN];
	UINT  m_currBuffLen;
};

