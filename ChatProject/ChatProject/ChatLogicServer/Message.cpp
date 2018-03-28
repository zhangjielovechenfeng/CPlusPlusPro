#include "Message.h"
#include "../Util/Util.h"

Message::Message()
{
}


Message::~Message()
{
}

CSMsgPkg & CSMessage::GetMsgPkg()
{
	return m_csMsgPkg;
}

void CSMessage::HandleMsgData(char * data)
{
	ASSERT_RETURN_VOID(data != NULL)


}
