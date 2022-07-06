/*************************************************
Copyright (C), 2019-2029, GuideIR Tech. Co., Ltd.
File name	: common_log.h
Author		: Guide Sensmart RD
Version		: 1.0
Date		: 2019/7/6
Description	:
*************************************************/

#ifndef COMMON_LOG_H
#define COMMON_LOG_H

#include "common_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
	LOG_DEBUG		= 1,	// 输出 Debug 级别信息
	LOG_INFO		= 2,	// 输出 Info 级别信息 
	LOG_WARN		= 3,	// 输出 Warning 级别信息 
	LOG_ERROR		= 4,	// 输出 Error 级别信息
	LOG_FATAL		= 5,	// 输出 Fatal Error 级别信息 
}LOG_LEVEL_T;

typedef enum
{
	MODULE_LOG		= 0,	// 日志模块
	MODULE_VIDEO	= 1,	// 视频处理模块
	MODULE_PORTING	= 2,	// 平台移植模块
	MODULE_RECORDE	= 3,	// 录像拍照模块
	MODULE_Y16		= 4,	// Y16处理
	MODULE_IMAGE	= 5,	// 图像处理
	MODULE_TEMPE	= 6,	// 测温模块
	MODULE_STATE	= 7,	// 机芯状态控制
	MODULE_DEVICE	= 8,	// 设备管理模块
	MODULE_CONTROL	= 9,	// 调度，机型兼容
	MODULE_UTIL		= 10,	// 工具集
}MODULE_T;

typedef enum
{
	DBG_OUTPUT_FILE		= 1,  //日志输出到文件，默认输出方式
	DBG_OUTPUT_CONSOLE	= 2,  //输出到控制台
	DBG_OUTPUT_ETHERNET = 3,  //使用网络输出，目前不支持
}DBG_MODE_T;

/**
* @brief 日志模块初始化，可重复多次调用。
* @param 无
* @return
* GUIDEIR_OK : 初始化成功
* GUIDEIR_ERR : 初始化失败
*/
INT32_T log_init();

/**
* @brief 日志模块去初始化
* @param 无
* @return
* GUIDEIR_OK : 去初始化成功
* GUIDEIR_ERR : 去初始化失败
*/
INT32_T log_uninit();

/**
* @brief 按模块调试等级输出日志，支持多线程调用。
* @param module 模块ID
* @param level 日志等级
* @param fmt 格式化字符串。参数说明参照标准的printf函数。
* @return
* > 0, 实际输出的字符数。
* = 0, GUIDEIR_OK，不满足等级条件不输出。
* < 0，GUIDEIR_ERR，参数非法或者日志输出文件写入失败。
* @note 默认日志输出等级是INFO，低于INFO不输出。在配置文件中可以分别设置各模块的日志等级。
* 例如：MODULE_VIDEO=1  VIDEO模块DEBUG等级以上日志将输出。MODULE_PORTING=3 PORTING模块WARN等级以上日志将输出。
* 每次打印一行日志上限是8KB。
*/
INT32_T log_output( MODULE_T module, LOG_LEVEL_T level, CONST CHAR_T *fmt, ...);

/**
* @brief 按调试等级输出日志，支持多线程调用。
* @param level 日志等级
* @param fmt 格式化字符串。参数说明参照标准的printf函数。
* @return
* > 0, 实际输出的字符数。
* = 0, GUIDEIR_OK，不满足等级条件不输出。
* < 0，GUIDEIR_ERR，参数非法或者日志输出文件写入失败。
* @note 每次打印一行日志上限是8KB。
*/
INT32_T log_output_ex(LOG_LEVEL_T level, CONST CHAR_T *fmt, ...);

/**
* @brief 设置日志文件保存路径。如果不设置，日志保存在程序同级目录下。
* @param 
*  path	文件路径
* @return
* GUIDEIR_OK : 设置成功
* GUIDEIR_ERR : 设置失败
*/
INT32_T log_set_path(CONST CHAR_T *path);

#ifdef __cplusplus
}
#endif

#endif // !COMMON_LOG_H

