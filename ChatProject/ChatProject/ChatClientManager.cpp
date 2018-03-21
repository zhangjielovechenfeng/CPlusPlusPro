#include "ChatClientManager.h"
#include "LogPrint.h"
#include "Util.h"

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

bool ChatClientManager::AddChatClient(int socketFd)
{
	ChatClient* chatClient = new ChatClient();
	if (!chatClient->InitChatClient())
	{
		LOG_ERR("Init ChatClient Failed!!!,socketFd:[%d]", socketFd);
		return false;
	}
	m_chatClientMap.insert(ChatClientMap::value_type(socketFd, chatClient));
	return true;
}

bool ChatClientManager::DelChatClient(int socketFd)
{
	ChatClientMap::iterator it = m_chatClientMap.find(socketFd);
	if (it == m_chatClientMap.end())
	{
		LOG_ERR("This Client Non-existent!!!"); // 不存在这个client对删除操作无影响
		return true;
	}
	SAFE_DELETE(it->second);
	m_chatClientMap.erase(socketFd);
	return true;
}

bool ChatClientManager::CheckOnline(int socketFd)
{
	ChatClientMap::iterator it = m_chatClientMap.find(socketFd);
	if (it == m_chatClientMap.end())
	{
		return false;
	}
	return true;
}

ChatClient * ChatClientManager::GetChatClient(int socketFd)
{
	ChatClientMap::iterator it = m_chatClientMap.find(socketFd);
	if (it == m_chatClientMap.end())
	{
		return NULL;
	}
	return it->second;
}
