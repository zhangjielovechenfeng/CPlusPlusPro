#pragma once

#define MAX_DATA_LEN 1024*1024	// ����������󳤶�
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
	// �������ݵ��Զ��建����
	bool InsertDataToBuff(char* data, uint32_t dataLen);

	void ClearBuff(uint32_t len);

private:
	char		m_recvBuff[MAX_DATA_LEN];
	uint32_t	m_currBuffLen;
};

