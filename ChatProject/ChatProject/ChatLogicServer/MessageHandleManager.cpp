#include "MessageHandleManager.h"
#include "../Util/CommonFactory.h"


MessageHandleManager::MessageHandleManager()
{
}


MessageHandleManager::~MessageHandleManager()
{
}

MessageHandle * MessageHandleManager::CreateMessageHandle(UINT type)
{
	return CreateProduct(type);
}

void MessageHandleManager::ReleaseMessageHandle()
{
	ReleaseProduct();
}
