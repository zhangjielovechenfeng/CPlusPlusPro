#include <cstdio>
#include "./ChatLogicServer/ChatServer.h"
#include "./Util/ErrDefine.h"

int main()
{
	ChatServer chatServer;
	chatServer.InitChatServer();
	int ret = chatServer.Run();
	if (ret != ERROR_CODE_NONE)
	{
		chatServer.Stop();
		return -1;
	}
	chatServer.Stop();
	return 0;
}