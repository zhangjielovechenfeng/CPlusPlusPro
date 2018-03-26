#pragma once

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
	
	virtual void HandleMsgData(char* data) = 0;

	char* GetMsgData();

protected:
	char* m_data;		
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
	// ������Ϣ,��֪ͨ��Ϣ����handle
	bool ParseMsgToPkg();

	// ��buff�����ݹ�һ����ʱ���Ѱ��洢�ڴ�data��,��֪ͨ�������
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
