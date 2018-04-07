#include <cstdio>
#include "./ChatLogicServer/ChatServer.h"
#include "./Util/ErrDefine.h"

int main()
{
	ChatServer chatServer;
	chatServer.InitChatServer();
	chatServer.StartTimeWheelThread();
	int ret = chatServer.Run();
	if (ret != ERROR_CODE_NONE)
	{
		chatServer.Stop();
		return -1;
	}
	chatServer.WaitTimeWheelThreadExit();
	chatServer.Stop();
	return 0;
}