#pragma once

#include "ChatClient.h"
#include <map>
#include "Singleton.h"

using namespace std;

/*
	聊天客户端管理器
*/
typedef map<int /*fd*/, ChatClient*> ChatClientMap;
class ChatClientManager : public Singleton<ChatClientManager>
{
public:
	ChatClientManager();
	~ChatClientManager();

public:
	// 添加聊天客户端（上线）
	bool AddChatClient(int socketFd);

	// 删除聊天客户端（下线）
	bool DelChatClient(int socketFd);

	// 检查是否在线
	bool CheckOnline(int socketFd);

	ChatClient* GetChatClient(int socketFd);

private:
	ChatClientMap m_chatClientMap; //客户端map
};

