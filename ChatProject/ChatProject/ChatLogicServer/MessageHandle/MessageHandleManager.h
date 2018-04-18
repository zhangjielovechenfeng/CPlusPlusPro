#pragma once

#include "../../Util/Singleton.h"
#include "MessageHandleSubClass.h"
#include "../../Util/CommonFactory.h"
#include "../../Protocol/ProtoDest/Chat.pb.h"

class MessageHandleManager : public Singleton<MessageHandleManager>, public CommonFactory<uint32_t, MessageHandle, MessageHandleManager>
{
public:
	MessageHandleManager()
	{
		REGISTER_PRODUCT(CSID_HEART_BEAT_REQ, MessageHandleHeartBeatReq, MessageHandleManager)
	}
	virtual ~MessageHandleManager() {}

public:
	MessageHandle * CreateMessageHandle(uint32_t type);
	void ReleaseMessageHandle();

private:
	INSTANCE_PRODUCT(MessageHandleHeartBeatReq, MessageHandle)
};

