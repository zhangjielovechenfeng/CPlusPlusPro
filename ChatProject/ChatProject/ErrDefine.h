#pragma once

enum ERROR_CODE
{
	ERROR_CODE_NONE								= 0,		// �ɹ�
	ERROR_CODE_SYS								= -1,		// ϵͳ����	
	ERROR_CODE_EPOLL_WAIT_TIMEOUT				= -2,		// epoll_wait��ʱ	
	ERROR_CODE_CREATE_SOCKET_FAILED				= -3,		// ����socketʧ��
	ERROR_CODE_SET_NONBLOCK_FAILED				= -4,		// ���÷�����ģʽʧ��
	ERROR_CODE_SET_REUSEADDR_FAILED				= -5,		// ���ö˿ڸ���ʧ��
	ERROR_CODE_SOCKET_BIND_FAILED				= -6,		// socket��ʧ��
	ERROR_CODE_SOCKET_LISTEN_FAILED				= -7,		// socket����ʧ��
	ERROR_CODE_SOCKET_ACCEPT_FAILED				= -8,		// �ȴ�client����ʧ��
	ERROR_CODE_RECV_MSG_ERROR					= -9,		// �������ݴ���
	ERROR_CODE_EPOLL_MOD_FAILED					= -10,		// epoll�¼��޸�ʧ��
	ERROR_CODE_EPOLL_ADD_FAILED					= -11,		// epoll�¼����ʧ��
	ERROR_CODE_SEND_MSG_ERROR					= -12,		// ��������ʧ��
};