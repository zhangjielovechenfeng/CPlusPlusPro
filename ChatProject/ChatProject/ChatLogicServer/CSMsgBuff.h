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
	uint32_t GetCurrBuffLen();

public:
	// 保存数据到自定义缓存区
	bool InsertDataToBuff(char* data, uint32_t dataLen);

	// 是否需要解析buff
	bool IsNeedParseBuff();

private:
	char m_recvBuff[MAX_DATA_LEN];
	uint32_t  m_currBuffLen;
};

