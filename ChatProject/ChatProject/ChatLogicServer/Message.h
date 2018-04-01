#pragma once

#include "../Protocol/ProtoDest/CSProtoPkgDef.pb.h"


using namespace CSProtocol;

#define MAX_ONE_PKG_SIZE 1024 // һ��������С
/*
	Message : ������Ϣ��
*/
class Message
{
public:
	Message();
	virtual ~Message();

public:
	CSMsgPkg& GetMsgPkg();

	void SetSessionID(int sessionID);
	int GetSessionID();

public:
	// ��Ϣ�������������Լ�ʵ��
	virtual void HandleMsgData(char* data) = 0;		

protected:
	CSMsgPkg		m_csMsgPkg;
	int				m_sessionID;
};

/*
	C->S������S�յ�����Ϣ
*/
class CSMessage : public Message
{
public:
	CSMessage();
	virtual ~CSMessage();
public:
	CSMsgPkg& GetMsgPkg();

public:
	// ֪ͨ��Ϣ����handle
	bool HandleMsg();

	// ����
	virtual void HandleMsgData(char* data);

};


/*
S->C������S���͵���Ϣ
*/
class SCMessage : public Message
{
public:
	SCMessage();
	virtual ~SCMessage();

public:
	// ������Ϣ,��֪ͨ��Ϣ����handle
	bool ParseMsgToPkg();

	// 
	virtual void HandleMsgData(char* data);

};

