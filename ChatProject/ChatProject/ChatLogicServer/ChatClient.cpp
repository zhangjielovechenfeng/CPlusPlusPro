#include "ChatClient.h"
#include <arpa/inet.h>


ChatClient::ChatClient(int connFd)
{
	m_connFd = connFd;
	m_ip.clear();
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

bool ChatClient::InitChatClient(SockAddr_In clientAddr)
{
	m_port = clientAddr.sin_port;
	m_ip = inet_ntoa(clientAddr.sin_addr);
	return true;
}

bool ChatClient::SaveMsgData(char * data, UINT dataLen)
{
	return m_recvMsgData.InsertDataToBuff(data, dataLen);
}
