#include "MessageHandleManager.h"

MessageHandle * MessageHandleManager::CreateMessageHandle(UINT type)
{
	return CreateProduct(type);
}

void MessageHandleManager::ReleaseMessageHandle()
{
	ReleaseProduct();
}
