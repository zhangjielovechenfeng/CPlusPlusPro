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

	// �Ƚ�����ͷ
	CSMsgPkgHead csMsgPkgHead;
	memcpy(&csMsgPkgHead, m_data.c_str(), sizeof(CSMsgPkgHead));
	m_csMsgPkg.set_allocated_csmsgpkghead(&csMsgPkgHead);

	// ������
	string tmpPkgBody = "";
	tmpPkgBody = OutOfOrderTool::NegativeOrder((char*)m_data.c_str() + sizeof(CSMsgPkgHead));

	// �����л�������
	CSMsgPkgBody* csMsgPkgBody = const_cast<CSMsgPkgBody*>(&m_csMsgPkg.csmsgpkgbody());
	csMsgPkgBody->ParseFromString(tmpPkgBody);

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
	// ���л����ݰ�����
	string serializeBodyData = "";
	if (m_csMsgPkg.csmsgpkgbody().SerializeToString(&serializeBodyData))
	{
		LOG_ERR("Serialize Msg Data Failed!!!");
		return;
	}
	char tmpData[sizeof(CSMsgPkgHead)] = { 0 };
	memcpy(tmpData, &m_csMsgPkg.csmsgpkghead(), sizeof(CSMsgPkgHead));
	string serializeHeadData = tmpData;
	m_serializeData = serializeHeadData + serializeBodyData;

	// ������
	m_serializeData = OutOfOrderTool::PositiveOrder(const_cast<char*>(m_serializeData.c_str()));
}

string & SCMessage::GetSerializeData()
{
	return m_serializeData;
}
