#pragma once

#define MAX_DATA_LEN 2*1024*1024	// ����������󳤶�
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
	// �������ݵ��Զ��建����
	bool InsertDataToBuff(char* data, UINT dataLen);

	// �Ƿ���Ҫ����buff
	bool IsNeedParseBuff();

private:
	char m_recvBuff[MAX_DATA_LEN];
	UINT  m_currBuffLen;
};

