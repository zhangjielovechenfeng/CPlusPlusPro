#include "MessageHandleManager.h"

MessageHandle * MessageHandleManager::CreateMessageHandle(uint32_t type)
{
	return CreateProduct(type);
}

void MessageHandleManager::ReleaseMessageHandle()
{
	ReleaseProduct();
}
