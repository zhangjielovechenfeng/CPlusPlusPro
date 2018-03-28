#include "ChatClientManager.h"
#include "../Util/LogPrint.h"
#include "../Util/Util.h"
#include <iostream>

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

bool ChatClientManager::AddChatClient(int connFd, SockAddr_In clientAddr)
{
	ChatClient* chatClient = new ChatClient(connFd);
	if (!chatClient->InitChatClient(clientAddr))
	{
		LOG_ERR("Init ChatClient Failed!!!,connFd:[%d]", connFd);
		return false;
	}
	m_chatClientMap.insert(ChatClientMap::value_type(connFd, chatClient));
	LOG_RUN("New Client[ip: %s][port: %d] Had Connected!!!", chatClient->GetIP().c_str(), chatClient->GetPort());
	cout << "New Client[ip: "<<chatClient->GetIP().c_str()<<"] Had Connected!!!" << endl;
	return true;
}

bool ChatClientManager::DelChatClient(int connFd)
{
	ChatClientMap::iterator it = m_chatClientMap.find(connFd);
	if (it == m_chatClientMap.end())
	{
		LOG_ERR("This Client Non-existent!!!"); // 不存在这个client对删除操作无影响
		return true;
	}

	LOG_RUN("The Client[ip: %s][port: %d] Disconnect!!!", it->second->GetIP().c_str(), it->second->GetPort());
	cout << "The Client[ip: " << it->second->GetIP().c_str() << "] Disconnect!!!" << endl;
	SAFE_DELETE(it->second);
	m_chatClientMap.erase(connFd);
	return true;
}

bool ChatClientManager::CheckOnline(int connFd)
{
	ChatClientMap::iterator it = m_chatClientMap.find(connFd);
	if (it == m_chatClientMap.end())
	{
		return false;
	}
	return true;
}

ChatClient * ChatClientManager::GetChatClient(int connFd)
{
	ChatClientMap::iterator it = m_chatClientMap.find(connFd);
	if (it == m_chatClientMap.end())
	{
		return NULL;
	}
	return it->second;
}
