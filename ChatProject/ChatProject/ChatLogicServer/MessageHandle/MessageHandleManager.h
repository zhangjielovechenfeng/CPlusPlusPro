#pragma once

#include "../../Util/Singleton.h"
#include "MessageHandle.h"
#include "../../Util/Util.h"
#include "../../Util/CommonFactory.h"

class MessageHandleManager : public Singleton<MessageHandleManager>, CommonFactory<UINT, MessageHandle, MessageHandleManager>
{
public:
	MessageHandleManager();
	~MessageHandleManager();

public:
	MessageHandle * CreateMessageHandle(UINT type);
	void ReleaseMessageHandle();

};

