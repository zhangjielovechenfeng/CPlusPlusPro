#pragma once

#define MAX_DATA_LEN 64	// ����������󳤶�
#include "../Util/Util.h"

class RecvMsgBuff
{
public:
	RecvMsgBuff();
	~RecvMsgBuff();

	bool InsertDataToBuff(char* data, UINT dataLen);

	// �Ƿ���Ҫ����buff
	bool IsNeedParseBuff();

private:
	char m_recvBuff[MAX_DATA_LEN];
	UINT  m_currBuffLen;
};

