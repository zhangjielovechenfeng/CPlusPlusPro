#pragma once

/*
	Message : ������Ϣ��
*/
class Message
{
public:
	Message();
	~Message();
private:
	char* m_data;
};

/*
	C->S������S�յ�����Ϣ
*/
class CSRecvMessage : public Message
{

};

