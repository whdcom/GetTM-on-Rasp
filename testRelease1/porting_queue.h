/*************************************************
Copyright (C), 2020-2030, GuideIR Tech. Co., Ltd.
File name	: porting_queue.h
Author		: Guide Sensmart RD
Version		: 1.0
Date		: 2020/1/6
Description	:
*************************************************/
#include <string.h>
#include "common_type.h"

#ifndef PORTING_QUEUE_H
#define PORTING_QUEUE_H

struct QUEUE_MESSAGE_s
{
	LONG_T msgType;			/* 消息类型，msgrcv根据此值接收消息。*/
	CHAR_T msgData[32];
public:
	QUEUE_MESSAGE_s()
	{
		memset(this, 0, sizeof(QUEUE_MESSAGE_s));
	}
};

/**
* @brief	创建队列。
* @param
   name		队列名称
   len		队列长度
* @return	队列句柄，a nonnegative integer，返回0也正常。
*/
INT32_T porting_queue_create(CONST CHAR_T *name, UINT32_T len);

/**
* @brief	销毁队列。
* @param
	handle	队列句柄
* @return
*	GUIDEIR_OK 成功
*	GUIDEIR_ERR 失败
*/
INT32_T porting_queue_destroy(INT32_T handle);

/**
* @brief	发送消息到队列。
* @param
	handle	队列句柄
	msg		消息
* @return
*	GUIDEIR_OK 成功
*	GUIDEIR_ERR 失败
*/
INT32_T porting_queue_send(INT32_T handle, QUEUE_MESSAGE_s msg);

/**
* @brief	从队列接收消息。
* @param
	handle	队列句柄
	msg		消息
	waitMode 为(0)时表示立即返回，为(-1)表示永久等待。
* @return
*	GUIDEIR_OK 成功
*	GUIDEIR_ERR 失败
*/
INT32_T porting_queue_recv(INT32_T handle, QUEUE_MESSAGE_s *msg, INT32_T waitMode);

#endif //PORTING_QUEUE_H
