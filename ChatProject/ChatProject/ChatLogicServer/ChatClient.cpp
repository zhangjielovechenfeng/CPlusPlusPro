#include "ChatClient.h"
#include <arpa/inet.h>
#include "../Util/LogPrint.h"
#include "WebSocketHandle.h"
#include "ChatClientManager.h"
#include "../Util/CommonDef.h"
#include "../Util/OutOfOrderTool.h"

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

int ChatClient::GetSessionID()
{
	return m_sessionID;
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

time_t & ChatClient::GetRecvTickTime()
{
	return m_recvTickTime;
}

void ChatClient::SetRecvTickTime(time_t recvTickTime)
{
	m_recvTickTime = recvTickTime;
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
	return true;
}

bool ChatClient::IsDisconnect(char * data, int dataLen)
{
	return WebSocketHandle::Instance().IsDisconnect(data, dataLen);
}

bool ChatClient::HandleMsg()
{
	// 判断是否已经和服务器建立了websoket连接
	if (!m_isBuildLongConn)
	{
		if (!_WebSocketShakeHandsHandle())
		{
			LOG_ERR("Server Shake Hands Msg Generate Failed!!!");
			return false;
		}
		return true;
	}
	// 服务器和client交互数据，提取真实数据内容
	string factData = WebSocketHandle::Instance().ParseClientData(m_recvMsgData.GetRecvBuff(), m_recvMsgData.GetCurrBuffLen());
	if (factData.empty())
	{
		LOG_ERR("Parse Client Data Failed!!!");
		return false;
	}

	// 是否需要解析
	if (_IsNeedParseData(factData))
	{
		CSMessage csMsg(m_sessionID);
		csMsg.SetMsgData(factData);
		// 重置buff
		m_recvMsgData.ClearBuff(m_recvMsgData.GetCurrBuffLen());
		// 处理消息
		csMsg.HandleMsgData();
	}
	return true;
}

bool ChatClient::_WebSocketShakeHandsHandle()
{
	// 还没有建立长连接，处理握手包
	if (!WebSocketHandle::Instance().IsWebSocketConn(m_recvMsgData.GetRecvBuff()))
	{
		// 不接受普通socket连接
		SetIsBuildLongConn(false);
	}
	else
	{
		string msg = WebSocketHandle::Instance().GenerateServerShakeHandsMsg(m_recvMsgData.GetRecvBuff(), m_recvMsgData.GetCurrBuffLen());
		if (msg.empty())
		{
			LOG_ERR("Generate Server ShakeHands Msg Is Empty!!!");
			return false;
		}
		if (!ChatServer::Instance().SendMessage(GetSessionID(), msg)) // 发送握手包信息， 直接send
		{
			LOG_ERR("Send Shake Hands Msg Failed");
			return false;
		}
		LOG_RUN("The Handshake With Websocket[ip: %s, port: %d, session: %d] Has Been Established!!!\n", GetIP().c_str(), GetPort(), GetSessionID());
		printf("The Handshake With Websocket[ip: %s, port: %d, session: %d] Has Been Established!!!\n", GetIP().c_str(), GetPort(), GetSessionID());

		// 回包发送后，清理buff
		m_recvMsgData.ClearBuff(m_recvMsgData.GetCurrBuffLen());

		SetIsBuildLongConn(true);
	}
	return true;
}

bool ChatClient::_IsNeedParseData(string& data)
{
	int pkgbodylen = 0;

	// 正序所有数据
	//OutOfOrderTool::PositiveOrder(const_cast<char*>(data.c_str()), data.size());
	 
	// 提出包体长度
	memcpy(&pkgbodylen, data.c_str(), CS_MSG_PKG_CONSTANT_HEAD_SIZE / 2);

 	if (data.size() < (uint32_t)(pkgbodylen + CS_MSG_PKG_CONSTANT_HEAD_SIZE))
	{
		return false;
	}
	return true;
}
