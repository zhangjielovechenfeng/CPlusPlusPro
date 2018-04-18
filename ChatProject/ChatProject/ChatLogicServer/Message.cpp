#include "Message.h"
#include "../Util/Util.h"
#include "MessageHandle/MessageHandleManager.h"
#include "../Util/LogPrint.h"
#include "../Util/OutOfOrderTool.h"
#include "../Util/CommonDef.h"

MessageBase::MessageBase()
{
	m_sessionID = 0;
}


MessageBase::~MessageBase()
{
}

CSMsgPkg & MessageBase::GetMsgPkg()
{
	return m_csMsgPkg;
}

int MessageBase::GetSessionID()
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

bool CSMessage::HandleMsg()
{
	MessageHandle* msgHandle = MessageHandleManager::Instance().CreateProduct(m_csMsgPkg.GetMsgID());
	if (NULL == msgHandle)
	{
		LOG_ERR("MessageHandle[MsgID: %u] Create Failes!!!", m_csMsgPkg.GetMsgID());
		return false;
	}

	if (!msgHandle->InitMessgeHandle(this))
	{
		LOG_ERR("MessageHandle[MsgID: %u] Init Failes!!!", m_csMsgPkg.GetMsgID());
		return false;
	}

	if (!msgHandle->Handle())
	{
		LOG_ERR("Message[MsgID: %u] Handle Failed!!!", m_csMsgPkg.GetMsgID());
		return false;
	}
	return true;
}

void CSMessage::HandleMsgData()
{
	uint32_t msgID = 0;
	uint32_t pkgBodyLen = 0;
	char* tmpData = NULL;

	// 正序所有数据（前方已经判断此数据是否是一个包）
	tmpData = OutOfOrderTool::PositiveOrder(const_cast<char*>(m_data.c_str()), m_data.size());

	// 提出消息ID和包体长度
	memcpy(&msgID, tmpData, CS_MSG_PKG_CONSTANT_HEAD_SIZE / 2); 
	memcpy(&pkgBodyLen, tmpData + CS_MSG_PKG_CONSTANT_HEAD_SIZE / 2, CS_MSG_PKG_CONSTANT_HEAD_SIZE / 2); 

	// 设置消息ID和包体长度
	m_csMsgPkg.SetMsgID(msgID);
	m_csMsgPkg.SetMsgPkgLen(pkgBodyLen);

	// 反序列化出包体
	m_csMsgPkg.GetMsgPkgBody()->ParseFromString(tmpData + CS_MSG_PKG_CONSTANT_HEAD_SIZE);

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
	if (m_csMsgPkg.GetMsgPkgBody()->SerializeToString(&serializeBodyData))
	{
		LOG_ERR("Serialize Msg Data Failed!!!");
		return;
	}
	char tmpData[CS_MSG_PKG_CONSTANT_HEAD_SIZE] = { 0 };
	uint32_t msgID = m_csMsgPkg.GetMsgID();
	uint32_t pkgBodyLen = serializeBodyData.size();

	// 填充包体长度
	m_csMsgPkg.SetMsgPkgLen(serializeBodyData.size());

	// 包头转化成字节序
	memcpy(tmpData, &msgID, CS_MSG_PKG_CONSTANT_HEAD_SIZE / 2);
	memcpy(tmpData + CS_MSG_PKG_CONSTANT_HEAD_SIZE / 2, &pkgBodyLen, CS_MSG_PKG_CONSTANT_HEAD_SIZE / 2);

	string serializeHeadData = tmpData;
	m_serializeData = serializeHeadData + serializeBodyData;

	//反序处理
	m_serializeData = OutOfOrderTool::PositiveOrder(const_cast<char*>(m_serializeData.c_str()), m_serializeData.size());
}

string & SCMessage::GetSerializeData()
{
	return m_serializeData;
}
