#pragma once

#define MAX_DATA_LEN 64	// ����������󳤶�
#include "Util.h"

class RecvMsgBuff
{
public:
	RecvMsgBuff();
	~RecvMsgBuff();

	bool InsertDataToBuff(char* data, UINT dataLen);

	// ���������buff���Ƿ���һ�������İ�
	bool HaveCompletePkgInData();

private:
	char m_recvBuff[MAX_DATA_LEN];
	UINT  m_currBuffLen;
};

