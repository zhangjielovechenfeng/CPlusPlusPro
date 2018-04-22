#pragma once
#include <string>
#include <websocketpp/sha1/sha1.hpp>
#include <websocketpp/base64/base64.hpp>
#include "../Util/Singleton.h"

#define STORE_SHA1_RESULT_BUFF_SIZE 64	// �洢sha1�����buff��С 

using namespace websocketpp::sha1;
using namespace websocketpp;
using namespace std;

/*
	WebSocket������
*/
class WebSocketHandle : public Singleton<WebSocketHandle>
{
public:
	WebSocketHandle() {}
	virtual ~WebSocketHandle() {}

	// �Ƿ���websocket����
	bool IsWebSocketConn(char* clientData);

	// �����ͻ�������
	string ParseClientData(char* clientdata, int dataLen);

	// �������������
	string PackServerData(string& serverData);

	// ���ɷ�����������Ϣ
	string GenerateServerShakeHandsMsg(char* clientData, int dataLen);

private:
	// ץȡ�ͻ�������key
	string _FetchClientKey(char* clientData, int dataLen);

	// ���ɷ���������key
	string _GenerateServerKey(string& clientKeyBody);
};

