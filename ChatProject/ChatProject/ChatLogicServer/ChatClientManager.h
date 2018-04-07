#pragma once

#include "ChatClient.h"
#include <map>
#include "../Util/Singleton.h"
#include "../Util/Time/Timer.h"

#define ONE_SECOND_TO_MSECOND 1000000 // һ���΢��Ļ����ϵ
#define MAX_INTERVAL_TIME 5

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

	bool InitTickTimer();

	// �������ͻ��ˣ����ߣ�
	bool AddChatClient(int sessionID, SockAddr_In clientAddr);

	// ɾ������ͻ��ˣ����ߣ�
	bool DelChatClient(int sessionID);

	// ����Ƿ�����
	bool CheckOnline(int sessionID);

	ChatClient* GetChatClient(int sessionID);

private:
	void _CheckClientTick();

private:
	ChatClientMap	m_chatClientMap; // �ͻ���map
	Timer			m_tickTimer;	 // ����timer
};

