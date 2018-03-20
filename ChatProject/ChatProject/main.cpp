#include <cstdio>
#include "ChatServer.h"

int main()
{
	ChatServer chatServer;
	chatServer.InitChatServer();
	chatServer.Run();
	chatServer.Stop();
    return 0;
}