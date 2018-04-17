#pragma once

#include "./ProtoDest/Chat.pb.h"
#include "../Util/Util.h"

/*
	CS ��Ϣ��ͷ����
*/
class CSMsgPkgHeadDef
{
public:
	CSMsgPkgHeadDef() : m_msgID(0), m_msgPkgLen(0) {}
	~CSMsgPkgHeadDef() {}

public:
	uint	m_msgID;		// ��ϢID
	uint	m_msgPkgLen;	// ��Ϣ���峤�� 
};

/*
	CS ��Ϣ���嶨��
*/

class CSMsgPkgBodyDef
{
public:
	CSMsgPkgBodyDef() 
	{
		m_csHeartBeatReq = NULL;
		m_scHeartBeatRsp = NULL;
		m_csLoginReq = NULL;
		m_scLoginRsp = NULL;
		m_csRegisterReq = NULL;
		m_scRegisterRsp = NULL;
	}
	~CSMsgPkgBodyDef()
	{
		SAFE_DELETE(m_csHeartBeatReq);
		SAFE_DELETE(m_scHeartBeatRsp);
		SAFE_DELETE(m_csLoginReq);
		SAFE_DELETE(m_scLoginRsp);
		SAFE_DELETE(m_csRegisterReq);
		SAFE_DELETE(m_scRegisterRsp);
	}

public:
	CS_HEART_BEAT_REQ*	m_csHeartBeatReq;
	SC_HEART_BEAT_RSP*	m_scHeartBeatRsp;
	CS_LOGIN_REQ*		m_csLoginReq;
	SC_LOGIN_RSP*		m_scLoginRsp;
	CS_REGISTER_REQ*	m_csRegisterReq;
	SC_REGISTER_RSP*	m_scRegisterRsp;
};

/*
	CS ��Ϣ������
*/
class CSMsgPkgDef
{
public:
	CSMsgPkgDef() {}
	~CSMsgPkgDef() {}

public:
	CSMsgPkgHeadDef m_csMsgPkgHeadDef;	// ��Ϣ��ͷ����
	CSMsgPkgBodyDef m_csMsgPkgBodyDef;	// ��Ϣ���嶨��
};

