#pragma once

#include "ChatClient.h"
#include <map>
#include "../Util/Singleton.h"
#include "../Util/Time/Timer.h"

#define ONE_SECOND_TO_MSECOND 1000000 // 一秒和微秒的换算关系
#define MAX_INTERVAL_TIME 5

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

	bool InitTickTimer();

	// 添加聊天客户端（上线）
	bool AddChatClient(int sessionID, SockAddr_In clientAddr);

	// 删除聊天客户端（下线）
	bool DelChatClient(int sessionID);

	// 检查是否在线
	bool CheckOnline(int sessionID);

	ChatClient* GetChatClient(int sessionID);

private:
	void _CheckClientTick();

private:
	ChatClientMap	m_chatClientMap; // 客户端map
	Timer			m_tickTimer;	 // 心跳timer
};

