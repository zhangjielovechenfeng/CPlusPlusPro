#include "MessageHandleSubClass.h"
#include "../ChatClientManager.h"
#include "../../Util/Time/Time.h"

bool MessageHandleHeartBeatReq::Handle()
{
	ChatClient* chatClient = ChatClientManager::Instance().GetChatClient(m_msg->GetSessionID());
	ASSERT_RETURN(chatClient != NULL, false);

	chatClient->SetRecvTickTime(Time::GetCurrMTime());

	return true;
}
