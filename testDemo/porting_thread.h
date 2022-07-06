/*************************************************
Copyright (C), 2019-2029, GuideIR Tech. Co., Ltd.
File name	: porting_thread.h
Author		: Guide Sensmart RD
Version		: 1.0
Date		: 2019/7/6
Description	:
*************************************************/

#ifndef PORTING_THREAD_H
#define PORTING_THREAD_H

#include "common_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct 
{
	UINT16_T year;
	UINT16_T month;
	UINT16_T day;
	UINT16_T hour;
	UINT16_T minute;
	UINT16_T second;
	UINT16_T milliseconds;
} TIME_T;

typedef  VOID(*GUIDEIR_THREAD_PROC)(VOID *param);

/**
* @brief		创建线程
* @param
* func:			函数入口地址:typedef VOID(*IPANEL_THREAD_PROC)(VOID *);
* param:		参数列表
* stack_size:	栈大小，可以不设置
* priority:		优先级别，可以不设置
* @return		线程句柄
*/
HANDLE_T porting_thread_create(GUIDEIR_THREAD_PROC func, VOID *param, INT32_T priority, UINT32_T stack_size);

/**
* @brief		销毁线程。等待目标线程结束后回收目标线程对象资源。另一种方式将目标线程分离，由其自己结束，不用等待。
*				采用第一种方式实现销毁线程。无论哪种方式都要求线程函数保证能自动退出。
* @param
* handle:		线程句柄
* @return
* 0				成功
* -1			失败
*/
INT32_T porting_thread_destroy(HANDLE_T handle);

/**
* @brief		挂起一段时间，让出CPU
* @param
* ms			休眠时间，单位毫秒
* @return		无
*/
VOID porting_thread_sleep(INT32_T ms);

/**
* @brief		获取线程在操作系统中的ID
* @param
* handle:		线程句柄
* @return		ID
*/
UINT64_T porting_thread_identify(HANDLE_T handle);

/**
* @brief		创建互斥量，类型是ERRORCHECK_MUTEX，同一线程不能重复加锁，加上的锁只能由本线程解锁。
* @param		无
* @return		mutex句柄
*/
HANDLE_T porting_mutex_create();

/**
* @brief		销毁互斥量
* @param		mutex句柄
* @return
* 0				成功
* -1			失败
*/
INT32_T porting_mutex_destroy(HANDLE_T handle);

/**
* @brief		互斥量加锁
* @param		mutex句柄
* @return
* 0				成功
* -1			失败
*/
INT32_T porting_mutex_lock(HANDLE_T handle);

/**
* @brief		互斥量解锁
* @param		mutex句柄
* @return
* 0				成功
* -1			失败
*/
INT32_T porting_mutex_unlock(HANDLE_T handle);

/**
* @brief		获取当前日期时间，精确到毫秒。
* @param		time，输出当前日期时间。
* @return
* 0				成功
* -1			失败
*/
INT32_T porting_get_time(TIME_T *time);

/**
* @brief		设置系统日期时间，精确到毫秒。
* @param		time，输入当前日期时间。
* @return
* 0				成功
* -1			失败
*/
INT32_T porting_set_time(TIME_T time);

/**
* @brief		获取当前时间和程序启动时间之间经过的毫秒数。
* @param		无
* @return		返回当前时间和程序启动时间之间经过的毫秒数。
* @note			使用该接口可以计算某个过程的耗时。过程开始时调用一次函数，结束时调用一次函数，两者相减即过程消耗的毫秒数。
*/
UINT32_T porting_get_ms(VOID);

#ifdef __cplusplus
}
#endif

#endif // !PORTING_THREAD_H


