#pragma once

#include "ChatClient.h"
#include <map>
#include "../Util/Singleton.h"

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
	bool AddChatClient(int sessionID, SockAddr_In clientAddr);

	// ɾ������ͻ��ˣ����ߣ�
	bool DelChatClient(int sessionID);

	// ����Ƿ�����
	bool CheckOnline(int sessionID);

	ChatClient* GetChatClient(int sessionID);

private:
	ChatClientMap m_chatClientMap; //�ͻ���map
};

