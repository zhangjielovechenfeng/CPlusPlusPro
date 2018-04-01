#pragma once

#define MAX_DATA_LEN 2*1024*1024	// 接收数据最大长度
#include "../Util/Util.h"

class CSMsgBuff
{
public:
	CSMsgBuff();
	~CSMsgBuff();

public:
	char* GetRecvBuff();
	UINT GetCurrBuffLen();

public:
	// 保存数据到自定义缓存区
	bool InsertDataToBuff(char* data, UINT dataLen);

	// 是否需要解析buff
	bool IsNeedParseBuff();

private:
	char m_recvBuff[MAX_DATA_LEN];
	UINT  m_currBuffLen;
};

