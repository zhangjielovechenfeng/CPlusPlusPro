#pragma once

#include "../ProtoDest/Chat.pb.h"
#include <types.h>

/*
CS ��Ϣ��ͷ����
*/
class CSMsgPkgHeadDef
{
public:
	CSMsgPkgHeadDef();
	~CSMsgPkgHeadDef();

private:
	uint	m_msgID;		// ��ϢID
	uint	m_msgPkgLen;	// ��Ϣ���峤�� 

};


/*
CS ��Ϣ���嶨��
*/
class CSMsgPkgBodyDef
{
public:
	CSMsgPkgBodyDef();
	~CSMsgPkgBodyDef();


};


/*
	CS ��Ϣ������
*/
class CSMsgPkgDef
{
public:
	CSMsgPkgDef();
	~CSMsgPkgDef();


};

