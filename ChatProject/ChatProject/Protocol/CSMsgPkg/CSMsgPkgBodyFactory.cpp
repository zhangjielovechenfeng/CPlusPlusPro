#include "CSMsgPkgBodyFactory.h"

Message * CSMsgPkgBodyFactory::CreateMsgPkgBody(uint32_t type)
{
	return CreateProduct(type);
}

void CSMsgPkgBodyFactory::ReleaseMsgPkgBody()
{
	ReleaseProduct();
}
