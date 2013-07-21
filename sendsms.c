#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libfetion/libfetion.h"

void sys_listener(int message, WPARAM id, LPARAM sender, void * args);
void log_callback(int message, WPARAM id, LPARAM from, void * args);

int main()
{
	// 初始化飞信库
	if (!fx_init()){
		fprintf(stderr, "初始化飞信库失败！\n");
		//exit(0);
		return 0;
	}

	// 登陆
	if (!fs_login("561729415", "fzsy2003")){
		fprintf(stderr, "飞信登陆失败！\n");
		//exit(0);
		return 0;
	}

	fx_set_longsms(1);
	fx_set_system_msg_cb(sys_listener, NULL);
	
	while(1)	;
	fx_loginout();
	fx_terminate();
	return 1;
}

/*
 * 系统消息监听分发函数
 *
 * 说明
 * 	负责监听所有来自飞信服务器的消息；
 * 	并分别调用对应的功能函数实现对应的功能；
 * */
void sys_listener(int message, WPARAM id, LPARAM sender, void * d){
	Fetion_MSG *fetion_msg;
	const Fetion_Account *fetion_account;
	char msg[500];
	
	if (message==FX_NEW_MESSAGE){	// 收取到新短信
		// 获取发送者昵称
		fetion_account = fx_get_account_by_id(sender);
		strcpy(msg, fetion_account->local_name);
		
		// 获取信息的内容
		fetion_msg = fx_get_msg(sender);
		strcat(msg,fx_msg_no_format(fetion_msg->message));
		
		// 信息转发给自己，同时输出日志到命令行
		fx_send_sms_to_self(msg, log_callback, NULL);
		fprintf(stdout,"%s", msg);

		// 释放转换字符串的堆栈
		fx_destroy_msg(fetion_msg);

	}
}

/*
 * 日志记录函数
 *
 * 说明
 * 	负责记录各次阻塞函数操作的结果到日志
 */
void log_callback(int type, WPARAM id, LPARAM from, void * d)
{
	fprintf(stderr, "LOG:%d\t%d\t%d\n", type, id, from);
}
