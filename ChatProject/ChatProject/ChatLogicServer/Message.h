#pragma once

#include "../Protocol/CSMsgPkg/CSMsgPkgDef.h"
#include <string>

using namespace std;

#define MAX_ONE_PKG_SIZE 1024 // һ��������С
/*
	Message : ������Ϣ�� 
*/
class MessageBase
{
public:
	MessageBase();
	virtual ~MessageBase();

public:
	CSMsgPkg & GetMsgPkg();

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
class CSMessage : public MessageBase
{
public:
	CSMessage(int session);
	virtual ~CSMessage();

public:
	// ֪ͨ��Ϣ����handle
	bool HandleMsg();

	// ����
	virtual void HandleMsgData();

	void SetMsgData(string& data);
private:
	string				m_data;

};


/*
S->C������S���͵���Ϣ
*/
class SCMessage : public MessageBase
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

