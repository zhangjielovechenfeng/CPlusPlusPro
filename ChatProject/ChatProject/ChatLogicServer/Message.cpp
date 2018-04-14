#include "Message.h"
#include "../Util/Util.h"
#include "MessageHandle/MessageHandleManager.h"
#include "../Util/LogPrint.h"
#include "../Util/OutOfOrderTool.h"
#include "../Util/CommonDef.h"

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
	MessageHandle* msgHandle = MessageHandleManager::Instance().CreateProduct(m_csMsgPkg.msgid());
	if (NULL == msgHandle)
	{
		LOG_ERR("MessageHandle[MsgID: %u] Create Failes!!!", m_csMsgPkg.msgid());
		return false;
	}

	if (!msgHandle->InitMessgeHandle(this))
	{
		LOG_ERR("MessageHandle[MsgID: %u] Init Failes!!!", m_csMsgPkg.msgid());
		return false;
	}

	if (!msgHandle->Handle())
	{
		LOG_ERR("Message[MsgID: %u] Handle Failed!!!", m_csMsgPkg.msgid());
		return false;
	}
	return true;
}

void CSMessage::HandleMsgData()
{
	m_csMsgPkg.Clear();

	uint msgID = 0;
	uint pkgBodyLen = 0;
	char* tmpData = NULL;

	// �����������ݣ�ǰ���Ѿ��жϴ������Ƿ���һ������
	tmpData = OutOfOrderTool::PositiveOrder(const_cast<char*>(m_data.c_str()), m_data.size());

	// �����ϢID�Ͱ��峤��
	memcpy(&msgID, tmpData, CS_MSG_PKG_CONSTANT_HEAD_SIZE / 2); 
	memcpy(&pkgBodyLen, tmpData + CS_MSG_PKG_CONSTANT_HEAD_SIZE / 2, CS_MSG_PKG_CONSTANT_HEAD_SIZE / 2); 

	// ������ϢID�Ͱ��峤��
	m_csMsgPkg.set_msgid(msgID);
	m_csMsgPkg.set_pkgbodylen(pkgBodyLen);

	// �����л�������
	CSMsgPkgBody* csMsgPkgBody = const_cast<CSMsgPkgBody*>(&m_csMsgPkg.csmsgpkgbody());
	csMsgPkgBody->ParseFromString(tmpData + CS_MSG_PKG_CONSTANT_HEAD_SIZE);

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
	char tmpData[CS_MSG_PKG_CONSTANT_HEAD_SIZE] = { 0 };
	uint msgID = m_csMsgPkg.msgid();
	uint pkgBodyLen = serializeBodyData.size();

	// �����峤��
	m_csMsgPkg.set_pkgbodylen(serializeBodyData.size());

	// ��ͷת�����ֽ���
	memcpy(tmpData, &msgID, CS_MSG_PKG_CONSTANT_HEAD_SIZE / 2);
	memcpy(tmpData + CS_MSG_PKG_CONSTANT_HEAD_SIZE / 2, &pkgBodyLen, CS_MSG_PKG_CONSTANT_HEAD_SIZE / 2);

	string serializeHeadData = tmpData;
	m_serializeData = serializeHeadData + serializeBodyData;

	//������
	m_serializeData = OutOfOrderTool::PositiveOrder(const_cast<char*>(m_serializeData.c_str()), m_serializeData.size());
}

string & SCMessage::GetSerializeData()
{
	return m_serializeData;
}
