#include <cstdio>
#include "./ChatLogicServer/ChatServer.h"
#include "./Util/ErrDefine.h"

int main()
{
	ChatServer::Instance().Init();
	ChatServer::Instance().Run();
	ChatServer::Instance().Stop();
	return 0;
}