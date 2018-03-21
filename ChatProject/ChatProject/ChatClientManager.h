#pragma once

#include "ChatClient.h"
#include <map>
#include "Singleton.h"

using namespace std;

/*
	����ͻ��˹�����
*/
typedef map<int /*fd*/, ChatClient*> ChatClientMap;
class ChatClientManager : public Singleton<ChatClientManager>
{
public:
	ChatClientManager();
	~ChatClientManager();

public:
	// �������ͻ��ˣ����ߣ�
	bool AddChatClient(int socketFd);

	// ɾ������ͻ��ˣ����ߣ�
	bool DelChatClient(int socketFd);

	// ����Ƿ�����
	bool CheckOnline(int socketFd);

	ChatClient* GetChatClient(int socketFd);

private:
	ChatClientMap m_chatClientMap; //�ͻ���map
};

