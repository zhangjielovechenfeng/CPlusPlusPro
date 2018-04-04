#include "Message.h"
#include "../Util/Util.h"
#include "MessageHandle/MessageHandleManager.h"
#include "../Util/LogPrint.h"

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
	m_data = NULL;
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
		LOG_ERR("MessageHandle[MsgID: %d] Create Failes!!!", m_csMsgPkg.csmsgpkghead().msgid());
		return false;
	}

	if (!msgHandle->InitMessgeHandle(this))
	{
		LOG_ERR("MessageHandle[MsgID: %d] Init Failes!!!", m_csMsgPkg.csmsgpkghead().msgid());
		return false;
	}

	if (!msgHandle->Handle())
	{
		LOG_ERR("Message[MsgID: %d] Handle Failed!!!", m_csMsgPkg.csmsgpkghead().msgid());
		return false;
	}
	return true;
}

void CSMessage::HandleMsgData()
{
	m_csMsgPkg.Clear();

	m_csMsgPkg.ParseFromString(m_data);
	//memcpy(&m_csMsgPkg, data, sizeof(m_csMsgPkg));

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
	if (m_csMsgPkg.SerializeToString(&m_serializeData)) // 序列化数据包
	{
		LOG_ERR("Serialize Msg Data Failed!!!");
		return;
	}
}

string & SCMessage::GetSerializeData()
{
	return m_serializeData;
}
