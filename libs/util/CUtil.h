#ifndef __CUTIL_H__
#define __CUTIL_H__
/*---------------------------------------------------------------------
Project:	CUtil
Purpose:
Author:		Ho-Jung Kim (godmode2k@hotmail.com)
Date:		February 17, 2012
Filename:	CUtil.h

Last modified: Feb 3, 2014
License:

*
* Copyright (C) 2014 Ho-Jung Kim (godmode2k@hotmail.com)
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
-----------------------------------------------------------------
Note:
-----------------------------------------------------------------
-----------------------------------------------------------------------*/



// --------------------------------------------------------------------------
//! Headers / Library
// --------------------------------------------------------------------------
#include <iostream>
#include <cstdio>		// <stdio.h>
#include <cstdlib>		// <stdlib.h>, labs()
#include <cstring>		// <string.h>
#include <cstdarg>		// <stdarg.h>
#include <ctime>		// <time.h>

// libxml2
// build opt: `xml2-config --cflags --libs`
//#include <libxml/xmlmemory.h>
//#include <libxml/parser.h>


#include "CUtil_DEF.h"
// --------------------------------------------------------------------------



// --------------------------------------------------------------------------
//! Definitions / Namespace
// --------------------------------------------------------------------------
//#define __REQ_DEBUG_MESSAGE__

#ifdef __REQ_DEBUG_MESSAGE__
	#define g_INT_MAX_LENGTH_LOG	2048
	#define g_STR_LOG_FILENAME		"__log_debug.txt"
	#define g_STR_MAKE_LOG_FILENAME(str)	str##g_STR_LOG_FILENAME
#endif	// __REQ_DEBUG_SHOW__

#define g_INT_MAX_UUID_SIZE		24

// Make a TAG string (String concatenation)
//#define INIT_TAG(TAG, SUBTAG)	#TAG":"#SUBTAG	// shit... variable to string (no #SUBTAG, SUBTAG)
#define INIT_TAG(TAG, SUBTAG)	TAG":"SUBTAG
//#define INIT_TAG(TAG)			TAG


#ifdef __LINUX__
	#define __PACK__	__attribute__((packed))
#else
	#define __PACK__
#endif


#ifdef __cplusplus
	extern "C" {
#endif
inline int RandInt(int x, int y) { return rand()%(y-x+1)+x; }
inline double RandFloat() { return (rand())/(RAND_MAX+1.0); }
inline bool RandBool() {
	if ( RandInt(0, 1) ) return true;
	else return false;
}

#ifdef __LINUX__
//
// DateTime
//
double get_time_usec(void);
double get_time_nsec(void);
void __usleep(int millisecond);
void get_date_d(int* pYear, int* pMonth, int* pDay,
		int* pHour, int* pMin, int* pSec, int* pMillisecond);
void get_date(char* pBuf_Year, char* pBuf_Month, char* pBuf_Day,
		char* pBuf_Hour, char* pBuf_Min, char* pBuf_Sec, char* pBuf_Millisecond);
bool get_uuid(char* buf, const int size);
void print_datetime_d(void);
void print_datetime(void);

//
// Alarm
//
//void sig_alarm(int);
#endif	// #ifdef __LINUX__

//
// Log
//
#ifdef __REQ_DEBUG_MESSAGE__
#define __LOG__(FUNC, LINE) fprintf( stdout, "%s : %d\n", /*__func__*/"##FUNC##", LINE );
void __LOG_FMT__(const char* pFmt, ...);
void __LOG_FMT_TAG__(const char* TAG, const char* pFmt, ...);
void _DBG_log_file(const char* pStrLog, const char* pNewFilename = NULL, const bool isInit = false);
void _DBG_log(const char* pFmt, ...);
void _DBG_log_init(void);
#define __LOGT__(TAG, ...) __LOG_FMT_TAG__(TAG, __VA_ARGS__)
	#ifdef __ANDROID_NDK__
		#undef __LOGT__(TAG, ...)
		#define __LOGT__(TAG, ...) LOGD(__VA_ARGS__)
	#endif
#else
#define __LOG__(FUNC, LINE)
/*
#define __LOG_FMT__(...)
#define __LOG_FMT_TAG__(TAG, ...)
#define _DBG_log_file(pStrLog, pNewFilename, isInit)
#define _DBG_log(...)
#define _DBG_log_init()
#define __LOG_FMT_TAG__(TAG, ...)
*/
void __LOG_FMT__(const char* pFmt, ...);
void __LOG_FMT_TAG__(const char* TAG, const char* pFmt, ...);
void _DBG_log_file(const char* pStrLog, const char* pNewFilename = NULL, const bool isInit = false);
void _DBG_log(const char* pFmt, ...);
void _DBG_log_init(void);
#define __LOGT__(TAG, ...)
	#ifdef __ANDROID_NDK__
		#define LOGD(...)
	#endif

#endif	// __REQ_DEBUG_MESSAGE__
#ifdef __cplusplus
	}	// extern "C"
#endif	// __cplusplus
// --------------------------------------------------------------------------



// --------------------------------------------------------------------------
//! Global Variables
// --------------------------------------------------------------------------
// --------------------------------------------------------------------------



// --------------------------------------------------------------------------
//! Prototypes / Inline Function
// --------------------------------------------------------------------------
// --------------------------------------------------------------------------



// --------------------------------------------------------------------------
//! Class / Structure / Namespace
// --------------------------------------------------------------------------
// --------------------------------------------------------------------------



#endif	// __CUTIL_H__
