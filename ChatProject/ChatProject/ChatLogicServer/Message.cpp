#include "Message.h"
#include "../Util/Util.h"
#include "MessageHandle/MessageHandleManager.h"
#include "../Util/LogPrint.h"
#include "../Util/OutOfOrderTool.h"

Message::Message()
{
	m_csMsgPkg.Clear();
	m_sessionID = 0;
}


Message::~Message()
{
}

CSMsgPkg & Message::GetMsgPkg()
{
	return m_csMsgPkg;
}

int Message::GetSessionID()
{
	return m_sessionID;
}

CSMessage::CSMessage(int sessionID)
{
	m_sessionID = sessionID;
	m_data.clear();
}

CSMessage::~CSMessage()
{
}

CSMsgPkg & CSMessage::GetMsgPkg()
{
	return m_csMsgPkg;
}

bool CSMessage::HandleMsg()
{
	MessageHandle* msgHandle = MessageHandleManager::Instance().CreateProduct(m_csMsgPkg.csmsgpkghead().msgid());
	if (NULL == msgHandle)
	{
		LOG_ERR("MessageHandle[MsgID: %u] Create Failes!!!", m_csMsgPkg.csmsgpkghead().msgid());
		return false;
	}

	if (!msgHandle->InitMessgeHandle(this))
	{
		LOG_ERR("MessageHandle[MsgID: %u] Init Failes!!!", m_csMsgPkg.csmsgpkghead().msgid());
		return false;
	}

	if (!msgHandle->Handle())
	{
		LOG_ERR("Message[MsgID: %u] Handle Failed!!!", m_csMsgPkg.csmsgpkghead().msgid());
		return false;
	}
	return true;
}

void CSMessage::HandleMsgData()
{
	m_csMsgPkg.Clear();

	// 先解析出头
	memcpy(const_cast<CSMsgPkgHead*>(&m_csMsgPkg.csmsgpkghead()), m_data.c_str(), sizeof(CSMsgPkgHead));

	// 反序列化出包体
	CSMsgPkgBody* csMsgPkgBody = const_cast<CSMsgPkgBody*>(&m_csMsgPkg.csmsgpkgbody());
	string data(m_data.c_str() + sizeof(CSMsgPkgHead));
	csMsgPkgBody->ParseFromString(data);

	// 反乱序处理

	HandleMsg();
}

void CSMessage::SetMsgData(char * data)
{
	m_data = data;
}

SCMessage::SCMessage(int sessionID)
{
	m_sessionID = sessionID;
	m_serializeData.clear();
}

SCMessage::~SCMessage()
{
}

void SCMessage::HandleMsgData()
{
	// 序列化数据包包体
	string serializeBodyData = "";
	if (m_csMsgPkg.csmsgpkgbody().SerializeToString(&serializeBodyData))
	{
		LOG_ERR("Serialize Msg Data Failed!!!");
		return;
	}
	string serializeHeadData = (char*)(const_cast<CSMsgPkgHead*>(&m_csMsgPkg.csmsgpkghead()));
	m_serializeData = serializeHeadData + serializeBodyData;

	// 再乱序处理
	m_serializeData = OutOfOrderTool::PositiveOrder(const_cast<char*>(m_serializeData.c_str()));
}

string & SCMessage::GetSerializeData()
{
	return m_serializeData;
}
