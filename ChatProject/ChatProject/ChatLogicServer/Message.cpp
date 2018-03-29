#include "Message.h"
#include "../Util/Util.h"
#include "MessageHandle/MessageHandleManager.h"
#include "../Util/LogPrint.h"

Message::Message()
{
}


Message::~Message()
{
}

CSMessage::CSMessage()
{
	m_csMsgPkg.Clear();
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

void CSMessage::HandleMsgData(char * data)
{
	ASSERT_RETURN_VOID(data != NULL)

	m_csMsgPkg.Clear();

	memcpy(&m_csMsgPkg, data, sizeof(m_csMsgPkg));

	HandleMsg();
}
