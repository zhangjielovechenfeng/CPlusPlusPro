#include "ChatClient.h"
#include <arpa/inet.h>
#include "../Util/LogPrint.h"
#include "WebSocketHandle.h"

ChatClient::ChatClient(int sessionID)
{
	m_sessionID = sessionID;
	m_ip.clear();
	m_isBuildLongConn = false;
}


ChatClient::~ChatClient()
{
}

string & ChatClient::GetIP()
{
	return m_ip;
}

int ChatClient::GetPort()
{
	return m_port;
}

bool ChatClient::IsBuildLongConn()
{
	return m_isBuildLongConn;
}

void ChatClient::SetIsBuildLongConn(bool isBuildLongConn)
{
	m_isBuildLongConn = isBuildLongConn;
}

CSMsgBuff & ChatClient::GetCSMsgBuff()
{
	return m_recvMsgData;
}

bool ChatClient::InitChatClient(SockAddr_In clientAddr)
{
	m_port = clientAddr.sin_port;
	m_ip = inet_ntoa(clientAddr.sin_addr);
	return true;
}

bool ChatClient::SaveMsgData(char * data, uint32_t dataLen)
{
	if (!m_recvMsgData.InsertDataToBuff(data, dataLen))
	{
		LOG_ERR("Save To Msg Buff Failed!!!");
		return false;
	}

	if (!m_isBuildLongConn)
	{
		return true;
	}

	if (m_recvMsgData.IsNeedParseBuff())
	{
		Message* message;
	}
}
