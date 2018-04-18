#pragma once
#include "../../Util/Singleton.h"
#include "../../Util/CommonFactory.h"
#include "../ProtoDest/Chat.pb.h"

using namespace google::protobuf;

/*
	CS消息包体工厂
*/
class CSMsgPkgBodyFactory : public Singleton<CSMsgPkgBodyFactory>, public CommonFactory<uint32_t, Message, CSMsgPkgBodyFactory>
{
public:
	CSMsgPkgBodyFactory()
	{
		REGISTER_PRODUCT(CSID_HEART_BEAT_REQ, CS_HEART_BEAT_REQ, CSMsgPkgBodyFactory)
		REGISTER_PRODUCT(SCID_HEART_BEAT_RSP, SC_HEART_BEAT_RSP, CSMsgPkgBodyFactory)
		REGISTER_PRODUCT(CSID_LOGIN_REQ, CS_LOGIN_REQ, CSMsgPkgBodyFactory)
		REGISTER_PRODUCT(SCID_LOGIN_RSP, SC_LOGIN_RSP, CSMsgPkgBodyFactory)
		REGISTER_PRODUCT(CSID_REGISTER_REQ, CS_REGISTER_REQ, CSMsgPkgBodyFactory)
		REGISTER_PRODUCT(SCID_REGISTER_RSP, SC_REGISTER_RSP, CSMsgPkgBodyFactory)
	}
	virtual ~CSMsgPkgBodyFactory() {}

public:
	Message * CreateMsgPkgBody(uint32_t type);
	void ReleaseMsgPkgBody();

private:
	INSTANCE_PRODUCT(CS_HEART_BEAT_REQ, Message)
	INSTANCE_PRODUCT(SC_HEART_BEAT_RSP, Message)
	INSTANCE_PRODUCT(CS_LOGIN_REQ, Message)
	INSTANCE_PRODUCT(SC_LOGIN_RSP, Message)
	INSTANCE_PRODUCT(CS_REGISTER_REQ, Message)
	INSTANCE_PRODUCT(SC_REGISTER_RSP, Message)
};

