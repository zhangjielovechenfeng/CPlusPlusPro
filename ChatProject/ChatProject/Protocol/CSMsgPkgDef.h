#pragma once

#include "../ProtoDest/Chat.pb.h"
#include <types.h>

/*
CS 消息包头定义
*/
class CSMsgPkgHeadDef
{
public:
	CSMsgPkgHeadDef();
	~CSMsgPkgHeadDef();

private:
	uint	m_msgID;		// 消息ID
	uint	m_msgPkgLen;	// 消息包体长度 

};


/*
CS 消息包体定义
*/
class CSMsgPkgBodyDef
{
public:
	CSMsgPkgBodyDef();
	~CSMsgPkgBodyDef();


};


/*
	CS 消息包定义
*/
class CSMsgPkgDef
{
public:
	CSMsgPkgDef();
	~CSMsgPkgDef();


};

