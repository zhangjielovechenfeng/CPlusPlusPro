#include <cstdio>
#include "./ChatLogicServer/ChatServer.h"
#include "./Util/ErrDefine.h"

int main()
{
	CSMsgPkgHead csMsgPkgHead;
	csMsgPkgHead.set_msgid(12121);
	csMsgPkgHead.set_pkgbodylen(1121);
	csMsgPkgHead.set_checksum("");
	printf("%d", csMsgPkgHead.ByteSize());
	//ChatServer::Instance().Init();
	//ChatServer::Instance().Run();
	//ChatServer::Instance().Stop();
	return 0;
}