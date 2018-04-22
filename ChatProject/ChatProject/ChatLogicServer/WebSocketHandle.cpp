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

string WebSocketHandle::ParseClientData(char * clientdata, int dataLen)
{
	ASSERT_RETURN(clientdata != NULL, "");

	if (dataLen < 2)
	{
		return "";
	}
	bool fin = (clientdata[0] & 0x80) == 0x80; // 1bit，1表示最后一帧
	if (!fin)
	{
		return "";// 超过一帧暂不处理
	}

	bool maskFlag = (clientdata[1] & 0x80) == 0x80; // 是否包含掩码
	if (!maskFlag)
	{
		return "";// 不包含掩码的暂不处理
	}
	int payloadLen = clientdata[1] & 0x7F; // 数据长度

	char masks[4] = { 0 };
	string payloadData = "";

	if (payloadLen == 126)
	{
		strncpy(masks, clientdata + 4, 4);
		payloadLen = (uint16_t)(clientdata[2] << 8 | clientdata[3]);
		payloadData.copy(clientdata + 8, payloadLen);
	}
	else if (payloadLen == 127)
	{
		strncpy(masks, clientdata + 10, 4);
		char payloadFactLen[8];
		for (int i = 0; i < 8; i++)
		{
			payloadFactLen[i] = clientdata[9 - i];
		}
		uint64_t len = *((uint64_t*)payloadFactLen);

		payloadData.copy(clientdata + 14, len);
	}
	else
	{
		strncpy(masks, clientdata + 2, 4);
		payloadData.copy(clientdata + 6, payloadLen);
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
		handleData[0] = 0x81;
		handleData[1] = (char)serverData.size();
		strncpy(const_cast<char*>(handleData.c_str()) + 2, serverData.c_str(), serverData.size());
	}
	else if (serverData.size() < 0xFFFF)
	{
		handleData[0] = 0x81;
		handleData[1] = 126;
		handleData[2] = (char)(serverData.size() & 0xFF);
		handleData[3] = (char)(serverData.size() >> 8 & 0xFF);
		strncpy(const_cast<char*>(handleData.c_str()) + 4, serverData.c_str(), serverData.size());
	}
	else
	{
		// 暂不处理超长内容
	}

	return handleData;
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
			break;
		}
		clientKey[i] = posPtr[i];
	}
	clientKey[dataLen] = '\0';

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
