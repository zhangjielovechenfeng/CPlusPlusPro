#include "ChatClientManager.h"
#include "../Util/LogPrint.h"
#include "../Util/Util.h"
#include <iostream>
#include "../Util/Time/Time.h"
#include <unistd.h>

using namespace std;

ChatClientManager::ChatClientManager()
{
	m_chatClientMap.clear();
}


ChatClientManager::~ChatClientManager()
{
	ChatClientMap::iterator it = m_chatClientMap.begin();
	for (; it != m_chatClientMap.end(); ++it)
	{
		SAFE_DELETE(it->second);
	}
	m_chatClientMap.clear();
}

bool ChatClientManager::InitTickTimer()
{
	m_tickTimer.Start(5 * ONE_SECOND_TO_MSECOND, TIMER_IND_FUNC(&ChatClientManager::_CheckClientTick));
	return true;
}

bool ChatClientManager::AddChatClient(int sessionID, SockAddr_In clientAddr)
{
	ChatClient* chatClient = new ChatClient(sessionID);
	if (!chatClient->InitChatClient(clientAddr))
	{
		LOG_ERR("Init ChatClient Failed!!!,sessionID:[%d]", sessionID);
		return false;
	}
	m_chatClientMap.insert(ChatClientMap::value_type(sessionID, chatClient));

	LOG_RUN("New Client[ip: %s][port: %d] Had Connected!!!", chatClient->GetIP().c_str(), chatClient->GetPort());
	cout << "New Client[ip: "<<chatClient->GetIP().c_str()<<"] Had Connected!!!" << endl;
	return true;
}

bool ChatClientManager::DelChatClient(int sessionID)
{
	ChatClientMap::iterator it = m_chatClientMap.find(sessionID);
	if (it == m_chatClientMap.end())
	{
		LOG_ERR("This Client Non-existent!!!"); // 不存在这个client对删除操作无影响
		return true;
	}

	LOG_RUN("The Client[ip: %s][port: %d] Disconnect!!!", it->second->GetIP().c_str(), it->second->GetPort());
	cout << "The Client[ip: " << it->second->GetIP().c_str() << "] Disconnect!!!" << endl;
	SAFE_DELETE(it->second);
	m_chatClientMap.erase(sessionID);
	return true;
}

bool ChatClientManager::CheckOnline(int sessionID)
{
	ChatClientMap::iterator it = m_chatClientMap.find(sessionID);
	if (it == m_chatClientMap.end())
	{
		return false;
	}
	return true;
}

ChatClient * ChatClientManager::GetChatClient(int sessionID)
{
	ChatClientMap::iterator it = m_chatClientMap.find(sessionID);
	if (it == m_chatClientMap.end())
	{
		return NULL;
	}
	return it->second;
}

void ChatClientManager::_CheckClientTick()
{
	// 获取当前时间
	time_t currMtime = Time::GetCurrMTime();

	ChatClientMap::iterator it = m_chatClientMap.begin();
	for (; it != m_chatClientMap.end(); ++it)
	{
		ChatClient* chatClient = it->second;
		ASSERT_CONTINUE(chatClient != NULL);

		time_t tmpMtime = currMtime - chatClient->GetRecvTickTime();

		if (tmpMtime < 0)
		{
			LOG_ERR("Time Error!!!");
			return;
		}
		// 5s没有和client交流，client下线
		else if(tmpMtime > MAX_INTERVAL_TIME * ONE_SECOND_TO_MSECOND)
		{
			// 关闭连接，删除client
			close(it->first);
			DelChatClient(it->first);
		}
	}
}
