#pragma once

#define MAX_DATA_LEN 2*1024*1024	// ����������󳤶�
#include "../Util/Util.h"

class CSMsgBuff
{
public:
	CSMsgBuff();
	~CSMsgBuff();

	bool InsertDataToBuff(char* data, UINT dataLen);

	// �Ƿ���Ҫ����buff
	bool IsNeedParseBuff();

private:
	char m_recvBuff[MAX_DATA_LEN];
	UINT  m_currBuffLen;
};

