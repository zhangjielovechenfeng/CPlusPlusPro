#include "WebSocketHandle.h"
#include "../Util/Util.h"
#include <string.h>

bool WebSocketHandle::IsWebSocketConn(char* clientData)
{
	string keyHead = "Sec-WebSocket-Key:";
	char* posPtr = strstr(clientData, keyHead.c_str());
	if (NULL == posPtr)
	{
		return false;
	}
	return true;
}

string WebSocketHandle::ParseClientData(char * clientData, int dataLen)
{
	ASSERT_RETURN(clientData != NULL, "");

	if (dataLen < 2)
	{
		return "";
	}
	bool fin = (clientData[0] & 0x80) == 0x80; // 1bit，1表示最后一帧
	if (!fin)
	{
		return "";// 超过一帧暂不处理
	}

	bool maskFlag = (clientData[1] & 0x80) == 0x80; // 是否包含掩码
	if (!maskFlag)
	{
		return "";// 不包含掩码的暂不处理
	}
	int payloadLen = clientData[1] & 0x7F; // 数据长度

	char masks[4] = { 0 };
	string payloadData = "";

	if (payloadLen == 126)
	{
		strncpy(masks, clientData + 4, 4);
		payloadLen = (uint16_t)(clientData[2] << 8 | clientData[3]);
		payloadData.copy(clientData + 8, payloadLen);
	}
	else if (payloadLen == 127)
	{
		strncpy(masks, clientData + 10, 4);
		char payloadFactLen[8];
		for (int i = 0; i < 8; i++)
		{
			payloadFactLen[i] = clientData[9 - i];
		}
		uint64_t len = *((uint64_t*)payloadFactLen);

		payloadData.copy(clientData + 14, len);
	}
	else
	{
		strncpy(masks, clientData + 2, 4);
		payloadData.copy(clientData + 6, payloadLen);
	}

	for (int i = 0; i < payloadLen; ++i)
	{
		payloadData[i] = payloadData[i] ^ masks[i % 4];
	}
	return payloadData;
}

string WebSocketHandle::PackServerData(string & serverData)
{
	string handleData = "";

	if (serverData.size() < 126)
	{
		handleData.push_back(0x81);
		handleData.push_back((char)serverData.size());
		handleData += serverData;
	}
	else if (serverData.size() < 0xFFFF)
	{
		handleData.push_back(0x81);
		handleData.push_back(126);
		handleData.push_back((char)(serverData.size() & 0xFF));
		handleData.push_back((char)(serverData.size() >> 8 & 0xFF));
		handleData += serverData;
	}
	else
	{
		// 暂不处理超长内容
	}

	return handleData;
}

bool WebSocketHandle::IsDisconnect(char * clientData, int dataLen)
{
	ASSERT_RETURN(clientData != NULL, false);
	if (clientData[0] & 0xF == 0x8)
	{
		return true;
	}
	return false;
}

string WebSocketHandle::_FetchClientKey(char* clientData, int dataLen)
{
	ASSERT_RETURN(clientData != NULL, "");
	string keyHead = "Sec-WebSocket-Key: "; // 此格式要求标准
	string clientKey = "";

	char* posPtr = strstr(clientData, keyHead.c_str());
	ASSERT_RETURN(posPtr != NULL, "");
	posPtr = posPtr + strlen(keyHead.c_str());
	for(int i = 0; i < dataLen; ++i)
	{
		if (posPtr[i] == '\r' || posPtr[i] == '\n')
		{
			//strncpy(const_cast<char*>(clientKey.c_str()), posPtr, i);
			break;
		}
		clientKey.push_back(posPtr[i]);
	}

	return clientKey;
}

string WebSocketHandle::_GenerateServerKey(string& clientKeyBody)
{
	clientKeyBody = clientKeyBody + "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

	unsigned char afterSha1Key[STORE_SHA1_RESULT_BUFF_SIZE] = "";
	calc(clientKeyBody.c_str(), clientKeyBody.size(), afterSha1Key); // sha1加密

	return base64_encode(afterSha1Key, strlen((const char*)afterSha1Key)); // base64转化
}

string WebSocketHandle::GenerateServerShakeHandsMsg(char* clientData, int dataLen)
{
	string clientKey = _FetchClientKey(clientData, dataLen);
	ASSERT_RETURN(!clientKey.empty(), "");

	string serverKey = _GenerateServerKey(clientKey);
	ASSERT_RETURN(!serverKey.empty(), "");

	//Sec-WebSocket-Protocol: chat\r\n
	return "HTTP/1.1 101 Switching Protocols\r\nUpgrade: websocket\r\nConnection: Upgrade\r\nSec-WebSocket-Accept: " + serverKey + "\r\n\r\n";
}
