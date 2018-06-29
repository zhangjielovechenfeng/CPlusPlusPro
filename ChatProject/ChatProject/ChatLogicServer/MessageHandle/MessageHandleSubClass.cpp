#include "MessageHandleSubClass.h"
#include "../ChatClientManager.h"
#include "../../Util/Time/Time.h"
#include "../../Protocol/ProtoDest/Chat.pb.h"
#include "../ChatServer.h"
#include "../../Util/LogPrint.h"
#include <stdio.h>

bool MessageHandleHeartBeatReq::Handle()
{
	ChatClient* chatClient = ChatClientManager::Instance().GetChatClient(m_msg->GetSessionID());
	ASSERT_RETURN(chatClient != NULL, false);

	chatClient->SetRecvTickTime(Time::GetCurrMTime());

	SCMessage msg(m_msg->GetSessionID());
	CSMsgPkg& pkg = msg.GetMsgPkg();
	pkg.m_msgID = SCID_HEART_BEAT_RSP;
	pkg.m_msgPkgBody = CSMsgPkgBodyFactory::Instance().CreateMsgPkgBody(pkg.m_msgID);

	printf("recv heart!!!\n");

	if (!ChatServer::Instance().SendMessage(&msg))
	{
		LOG_ERR("Send Message Failed!!!");
	}
	return true;
}
