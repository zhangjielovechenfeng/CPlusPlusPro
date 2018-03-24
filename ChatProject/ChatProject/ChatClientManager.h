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
	bool AddChatClient(int connFd, SockAddr_In clientAddr);

	// 删除聊天客户端（下线）
	bool DelChatClient(int connFd);

	// 检查是否在线
	bool CheckOnline(int connFd);

	ChatClient* GetChatClient(int connFd);

private:
	ChatClientMap m_chatClientMap; //客户端map
};

