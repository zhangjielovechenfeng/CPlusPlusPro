#pragma once

#include "./ProtoDest/Chat.pb.h"
#include "../Util/Util.h"

/*
	CS 消息包头定义
*/
class CSMsgPkgHeadDef
{
public:
	CSMsgPkgHeadDef() : m_msgID(0), m_msgPkgLen(0) {}
	~CSMsgPkgHeadDef() {}

public:
	uint	m_msgID;		// 消息ID
	uint	m_msgPkgLen;	// 消息包体长度 
};

/*
	CS 消息包体定义
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
	CS 消息包定义
*/
class CSMsgPkgDef
{
public:
	CSMsgPkgDef() {}
	~CSMsgPkgDef() {}

public:
	CSMsgPkgHeadDef m_csMsgPkgHeadDef;	// 消息包头定义
	CSMsgPkgBodyDef m_csMsgPkgBodyDef;	// 消息包体定义
};

