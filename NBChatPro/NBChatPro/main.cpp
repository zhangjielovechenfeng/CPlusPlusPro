#include <cstdio>
#include "./ChatLogicServer/ChatServer.h"

int main()
{
	ChatServer chatServer;
	chatServer.InitChatServer();
	chatServer.Run();
	chatServer.Stop();
	return 0;
}