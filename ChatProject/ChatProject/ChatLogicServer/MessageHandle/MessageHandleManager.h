#pragma once

#include "../../Util/Singleton.h"
#include "MessageHandleSubClass.h"
#include "../../Util/Util.h"
#include "../../Util/CommonFactory.h"
#include "../../Protocol/ProtoDest/CSProtoPkgBodyDef.pb.h"

class MessageHandleManager : public Singleton<MessageHandleManager>, public CommonFactory<UINT, MessageHandle, MessageHandleManager>
{
public:
	MessageHandleManager()
	{
		REGISTER_PRODUCT(CSMsg_None, MessageHandleNone, MessageHandleManager);
		REGISTER_PRODUCT(CSMsg_HeartBeatReq, MessageHandleHeartBeatReq, MessageHandleManager);
	}
	virtual ~MessageHandleManager() {}

public:
	MessageHandle * CreateMessageHandle(UINT type);
	void ReleaseMessageHandle();

private:
	INSTANCE_PRODUCT(MessageHandleNone, MessageHandle)
	INSTANCE_PRODUCT(MessageHandleHeartBeatReq, MessageHandle)
};

