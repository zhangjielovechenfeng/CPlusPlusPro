#include "WebSocketHandle.h"
#include "string.h"
#include "../Util/Util.h"
#include <cstdio>


WebSocketHandle::WebSocketHandle()
{
	m_clientKeyBody.clear();
	m_clientShakeHandsMsg = NULL;
	m_len = 0;
}


WebSocketHandle::~WebSocketHandle()
{
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
	string keyHead = "Sec - WebSocket - Accept:";
	char* posPtr = strstr(m_clientShakeHandsMsg, keyHead.c_str());
	if (NULL == posPtr)
	{
		return false;
	}
	return true;
}

void WebSocketHandle::SetShakeHandsMsg(char* shakeHandsMsg, int len)
{
	m_clientShakeHandsMsg = shakeHandsMsg;
	m_len = len;
}

bool WebSocketHandle::_FetchClientKey()
{
	string keyHead = "Sec - WebSocket - Accept:";

	char* posPtr = strstr(m_clientShakeHandsMsg, keyHead.c_str());
	ASSERT_RETURN(posPtr != NULL, false);
	posPtr = posPtr + strlen(keyHead.c_str());
	for(int i = 0; i < m_len; ++i)
	{
		if (posPtr[i] == '\r\n')
		{
			break;
		}
		m_clientKeyBody[i] = posPtr[i];
	}

	return false;
}

bool WebSocketHandle::_GenerateServerKey()
{
	char* tmpKey = strcat(const_cast<char *>(m_clientKeyBody.c_str()), "258EAFA5-E914-47DA-95CA-C5AB0DC85B11");
	ASSERT_RETURN(tmpKey != NULL, false);

	unsigned char afterSha1Key[STORE_SHA1_RESULT_BUFF_SIZE];
	calc(tmpKey, strlen(tmpKey), afterSha1Key); // sha1¼ÓÃÜ

	m_serverKeyBody = base64_encode(afterSha1Key, strlen((const char*)afterSha1Key)); // base64×ª»¯
	return true;
}

bool WebSocketHandle::_GenerateServerShakeHandsMsg()
{
	sprintf(const_cast<char*>(m_serverShakeHandsMsg.c_str()),	"HTTP / 1.1 101 Switching Protocols\r\n		\
																Upgrade : websocke Connection : Upgrade\r\n \
																Sec - WebSocket - Accept : %s\r\n			\
																Sec - WebSocket - Protocol : ChatServer", m_serverKeyBody.c_str());
	if (m_serverShakeHandsMsg.empty())
	{
		return false;
	}
	return true;
}
