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
	bool AddChatClient(int connFd, SockAddr_In clientAddr);

	// ɾ������ͻ��ˣ����ߣ�
	bool DelChatClient(int connFd);

	// ����Ƿ�����
	bool CheckOnline(int connFd);

	ChatClient* GetChatClient(int connFd);

private:
	ChatClientMap m_chatClientMap; //�ͻ���map
};

