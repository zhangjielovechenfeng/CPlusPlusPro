#pragma once

#include "../ProtoDest/Chat.pb.h"
#include "../../Util/Util.h"
#include "./CSMsgPkgBodyFactory.h"

/*
	CS 消息包定义
*/
class CSMsgPkg
{
public:
	CSMsgPkg() : m_msgID(0), m_msgPkgLen(0), m_msgPkgBody(NULL){}
	~CSMsgPkg() 
	{
		SAFE_DELETE(m_msgPkgBody);
	}

public:
	uint32_t GetMsgID();
	void SetMsgID(uint32_t msgID);
	uint32_t GetMsgPkgLen();
	void SetMsgPkgLen(uint32_t msgPkgLen);
	Message * GetMsgPkgBody();

public:
	uint32_t		m_msgID;		// 消息ID
	uint32_t		m_msgPkgLen;	// 消息包体长度 
	Message *		m_msgPkgBody;	// 消息包体
};

