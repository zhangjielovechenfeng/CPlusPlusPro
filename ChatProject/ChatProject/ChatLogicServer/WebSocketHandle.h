#pragma once
#include <string>
#include <websocketpp/sha1/sha1.hpp>
#include <websocketpp/base64/base64.hpp>

#define STORE_SHA1_RESULT_BUFF_SIZE 64	// �洢sha1�����buff��С 

using namespace websocketpp::sha1;
using namespace websocketpp;
using namespace std;

/*
	WebSocket������
*/
class WebSocketHandle
{
public:
	WebSocketHandle();
	~WebSocketHandle();

	// ����
	bool Handle();

	// ��ȡ������������Ϣ
	string& GetServerShakeHandsMsg();

	// �Ƿ���websocket����
	bool IsWebSocketConn();

	// ���� client������Ϣ
	void SetClientShakeHandsMsg(char* clientShakeHandsMsg, int len);

public:
	// ץȡ�ͻ�������key
	bool _FetchClientKey();

	// ���ɷ���������key
	bool _GenerateServerKey();

	// ���ɷ�����������Ϣ
	bool _GenerateServerShakeHandsMsg();
private:
	char*		m_clientShakeHandsMsg; // client��������
	string		m_serverShakeHandsMsg;
	int			m_len;
	string		m_clientKeyBody;
	string		m_serverKeyBody;
};

