/*************************************************
Copyright (C), 2019-2029, GuideIR Tech. Co., Ltd.
File name	: porting_util.h
Author		: Guide Sensmart RD
Version		: 1.0
Date		: 2019/12/16
Description	:
*************************************************/
#ifndef PORTING_UTIL_H
#define PORTING_UTIL_H

#include "common_type.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
* @brief:  porting_utf8_to_unicode
* @access: public 
* @param:  CHAR_T * unicode
* @param:  CONST CHAR_T * utf8
* @return: INT32_T
* @note:  
**/
INT32_T porting_utf8_to_unicode(CHAR_T *unicode, CONST CHAR_T *utf8);

/**
* @brief:  porting_unicode_to_utf8
* @access: public 
* @param:  CHAR_T * utf8
* @param:  CONST CHAR_T * unicode
* @return: INT32_T
* @note:  
**/
INT32_T porting_unicode_to_utf8(CHAR_T *utf8, CONST CHAR_T *unicode, INT32_T length);

#ifdef __cplusplus
}
#endif

#endif // !PORTING_UTIL_H
