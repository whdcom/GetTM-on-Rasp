/*************************************************
Copyright (C), 2019-2029, GuideIR Tech. Co., Ltd.
File name	: porting_file.h
Author		: Guide Sensmart RD
Version		: 1.0
Date		: 2019/7/6
Description	:
*************************************************/

#ifndef PORTING_FILE_H
#define PORTING_FILE_H

#include "common_type.h"

#ifdef __cplusplus
extern "C" {
#endif

#define NAME_LENGTH 256

typedef struct
{
	UINT32_T flag; 				//0 表示文件节点，1表示目录节点
	CHAR_T 	 name[NAME_LENGTH]; //节点名字符串，不包含上级路径
	ULONG_T  size;				//文件大小，是文件时有意义。
	UINT64_T time;				//修改日期，从1970年1月1日0时0分0秒开始计时的秒数。
}DIR_T;

/**
* @brief	打开文件
* @param
* filename	文件路径+名称
* mode		使用文件方式，类型和操作要求，参考fopen。
* @return	文件句柄，NULL表示打开文件失败。
*/
HANDLE_T porting_file_open(CONST CHAR_T *filename, CONST CHAR_T *mode);

/**
* @brief	关闭文件
* @param
* fd		文件句柄
* @return	文件句柄，NULL表示打开文件失败。
*/
INT32_T porting_file_close(HANDLE_T fd);

/**
* @brief	读文件
* @param
* fd		文件句柄
* buffer	读数据缓存
* nbytes	读取大小
* @return	实际读取大小
*/
INT32_T porting_file_read(HANDLE_T fd, BYTE_T *buffer, INT32_T nbytes);

/**
* @brief	写文件
* @param
* fd		文件句柄
* buffer	写数据缓存
* nbytes	写入大小
* @return	实际写入大小
*/
INT32_T porting_file_write(HANDLE_T fd, CONST BYTE_T *buffer, INT32_T nbytes);

/**
* @brief	将缓冲区内容写入文件
* @param
* fd		文件句柄
*/
INT32_T porting_file_flush(HANDLE_T fd);

/**
* @brief	得到文件当前读写位置
* @param
* fd		文件句柄
* @return	当前读写位置偏离文件头部的字节数
*/
INT32_T porting_file_tell(HANDLE_T fd);

/**
* @brief	把文件读写指针移到指定位置
* @param
* fd		文件句柄
* offset	偏移位置
* seek		起始点，取值有0 1 2，代表文件不同的位置。
* SEEK_SET 0   文件开头
* SEEK_CUR 1   文件当前位置
* SEEK_END 2   文件末尾
* @return
* 0			成功
* -1		失败
*/
INT32_T porting_file_seek(HANDLE_T fd, INT32_T offset, INT32_T seek);

/**
* @brief	把文件读写指针重新指向文件开头
* @param
* fd		文件句柄
* @return
* 0			成功
* -1		失败
*/
INT32_T porting_file_rewind(HANDLE_T fd);

/**
* @brief	删除文件
* @param
* name		文件路径+名称
* @return
* 0			成功
* -1		失败
*/
INT32_T porting_file_delete(CONST CHAR_T *name);

/**
* @brief	新建目录
* @param
* dirname	目录名
* @return
* 0			成功
* -1		失败
*/
INT32_T porting_dir_create(CONST CHAR_T *dirname);

/**
* @brief	删除目录，不支持删非空目录。
* @param
* dirname	目录名
* @return
* 0			成功
* -1		失败
*/
INT32_T porting_dir_remove(CONST CHAR_T *dirname);

/**
* @brief	打开目录，read目录之前先open。
* @param
* dirname	目录名
* @return	返回目录句柄
*/
HANDLE_T porting_dir_open(CONST CHAR_T *dirname);

/**
* @brief	关闭目录
* @param
* dd		目录句柄
* @return
* 0			成功
* -1		失败
*/
INT32_T porting_dir_close(HANDLE_T dd);

/**
* @brief	读取目录下的一个子节点，多次read直到读取的子节点为空表明遍历目录完成。
* @param
* dd		目录句柄
* pdir		读到的子节点信息，输出参数。
* @return
* 0			成功
* -1		失败
*/
INT32_T porting_dir_read(HANDLE_T dd, DIR_T *pdir);

/**
* @brief	输出目录下的所有文件信息，记录到日志文件。
* @param
* dirname	目录名，要以通配符"*.*"结束。
* @return
* 0			成功
* -1		失败
*/
INT32_T porting_dir_list(CONST CHAR_T *dirname);

#ifdef __cplusplus
}
#endif

#endif // !PORTING_FILE_H

