#pragma once

#include "../Protocol/ProtoDest/CSProtoPkgDef.pb.h"
#include <string>

using namespace std;

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

	int GetSessionID();

public:
	// ��Ϣ�������������Լ�ʵ��
	virtual void HandleMsgData() = 0;		

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
	CSMessage(int session);
	virtual ~CSMessage();
public:
	CSMsgPkg& GetMsgPkg();

public:
	// ֪ͨ��Ϣ����handle
	bool HandleMsg();

	// ����
	virtual void HandleMsgData();

	void SetMsgData(char* data);
private:
	string				m_data;

};


/*
S->C������S���͵���Ϣ
*/
class SCMessage : public Message
{
public:
	SCMessage(int sessionID);
	virtual ~SCMessage();

public:
	// ������msg
	virtual void HandleMsgData();

	string& GetSerializeData();
private:
	string			m_serializeData; // ���л��������

};

