#pragma once

enum ERROR_CODE
{
	ERROR_CODE_NONE								= 0,		// 成功
	ERROR_CODE_SYS								= -1,		// 系统错误	
	ERROR_CODE_EPOLL_WAIT_TIMEOUT				= -2,		// epoll_wait超时	
	ERROR_CODE_CREATE_SOCKET_FAILED				= -3,		// 创建socket失败
	ERROR_CODE_SET_NONBLOCK_FAILED				= -4,		// 设置非阻塞模式失败
	ERROR_CODE_SET_REUSEADDR_FAILED				= -5,		// 设置端口复用失败
	ERROR_CODE_SOCKET_BIND_FAILED				= -6,		// socket绑定失败
	ERROR_CODE_SOCKET_LISTEN_FAILED				= -7,		// socket监听失败
	ERROR_CODE_SOCKET_ACCEPT_FAILED				= -8,		// 等待client请求失败
	ERROR_CODE_RECV_MSG_ERROR					= -9,		// 接受数据错误
	ERROR_CODE_EPOLL_MOD_FAILED					= -10,		// epoll事件修改失败
	ERROR_CODE_EPOLL_ADD_FAILED					= -11,		// epoll事件添加失败
	ERROR_CODE_SEND_MSG_ERROR					= -12,		// 发送数据失败
	ERROR_CODE_CLIENT_NOT_EXIST					= -13,		// client不存在
	ERROR_CODE_CLIENT_OFFLINE_FAILED			= -14,		// client离线失败
	ERROR_CODE_INSERT_MSG_TO_BUFF_FAILED		= -15,		// 接受数据保存失败
	ERROR_CODE_INSERT_ONLINE_FAILED				= -16,		// client上线失败
	ERROR_CODE_CLIENT_HAD_OFFLINE				= -17,		// client已离线
	ERROR_CODE_GET_CURR_TIME_FAILED				= -18		// 获取当前时间失败
};