#include "WebSocketHandle.h"
#include "string.h"
#include "../Util/Util.h"
#include <cstdio>


WebSocketHandle::WebSocketHandle()
{
	//m_clientKeyBody.clear();
	m_serverKeyBody.clear();
	m_serverShakeHandsMsg.clear();
	m_clientShakeHandsMsg = NULL;
	m_len = 0;
}


WebSocketHandle::~WebSocketHandle()
{
	memset(m_clientShakeHandsMsg, 0, m_len);
}

bool WebSocketHandle::Handle()
{
	if (!_FetchClientKey())
	{
		return false;
	}
	if (!_GenerateServerKey())
	{
		return false;
	}
	if (!_GenerateServerShakeHandsMsg())
	{
		return false;
	}

	return true;
}

string & WebSocketHandle::GetServerShakeHandsMsg()
{
	return m_serverShakeHandsMsg;
}

bool WebSocketHandle::IsWebSocketConn()
{
	string keyHead = "Sec-WebSocket-Key:";
	char* posPtr = strstr(m_clientShakeHandsMsg, keyHead.c_str());
	if (NULL == posPtr)
	{
		return false;
	}
	return true;
}

void WebSocketHandle::SetClientShakeHandsMsg(char* clientShakeHandsMsg, int len)
{
	m_clientShakeHandsMsg = clientShakeHandsMsg;
	m_len = len;
}

bool WebSocketHandle::_FetchClientKey()
{
	string keyHead = "Sec-WebSocket-Key: "; // 此格式要求标准

	char* posPtr = strstr(m_clientShakeHandsMsg, keyHead.c_str());
	ASSERT_RETURN(posPtr != NULL, false);
	posPtr = posPtr + strlen(keyHead.c_str());
	for(int i = 0; i < m_len; ++i)
	{
		if (posPtr[i] == '\r' || posPtr[i] == '\n')
		{
			break;
		}
		m_clientKeyBody[i] = posPtr[i];
	}

	return true;;
}

bool WebSocketHandle::_GenerateServerKey()
{
	char* tmpKey = strcat(m_clientKeyBody, "258EAFA5-E914-47DA-95CA-C5AB0DC85B11");
	ASSERT_RETURN(tmpKey != NULL, false);

	unsigned char afterSha1Key[STORE_SHA1_RESULT_BUFF_SIZE];
	calc(tmpKey, strlen(tmpKey), afterSha1Key); // sha1加密

	m_serverKeyBody = base64_encode(afterSha1Key, strlen((const char*)afterSha1Key)); // base64转化
	return true;
}

bool WebSocketHandle::_GenerateServerShakeHandsMsg()
{
	m_serverShakeHandsMsg = "HTTP/1.1 101 Switching Protocols\r\nUpgrade: websocket\r\nConnection: Upgrade\r\nSec-WebSocket-Accept: " + m_serverKeyBody + "\r\nSec-WebSocket-Protocol: chat\r\n\r\n";
	printf("%s", m_serverShakeHandsMsg.c_str());
	if (m_serverShakeHandsMsg.empty())
	{
		return false;
	}
	return true;
}
