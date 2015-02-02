/*---------------------------------------------------------------------
Project:
Purpose:	Utilities
Author:		Ho-Jung Kim (godmode2k@hotmail.com)
Date:		February 17, 2012
File:		CUtil.cpp

Last Modified Date: Jan 5, 2015
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
NOTE:
-----------------------------------------------------------------
-----------------------------------------------------------------------*/



// --------------------------------------------------------------------------
//! Headers / Library
// --------------------------------------------------------------------------
#include "CUtil.h"

#ifdef __LINUX__
	#include <sys/time.h>		// clock_gettime(): use "-lrt" at compile
	#include <sys/select.h>
#else
#endif
// --------------------------------------------------------------------------



// --------------------------------------------------------------------------
//! Definitions / Namespace
// --------------------------------------------------------------------------
//#define __REQ_DEBUG_MESSAGE__
// --------------------------------------------------------------------------



// --------------------------------------------------------------------------
//! Global Variables
// --------------------------------------------------------------------------
// --------------------------------------------------------------------------



// --------------------------------------------------------------------------
//! Prototypes / Inline Function
// --------------------------------------------------------------------------
#ifdef __cplusplus
	extern "C" {
#endif

#ifdef __LINUX__
//
// DateTime
//
//double get_time_usec(void);
double get_time_usec(void) {
	struct timeval tv = { 0, 0 };

	gettimeofday( &tv, 0 );	// timezone be set NULL
	//printf( "usec = %ld\n", ((double)tv.tv_sec + ((double)tv.tv_usec)/1E6) );

	return (double)tv.tv_sec + ((double)tv.tv_usec)/1E6;
}

//double get_time_nsec(void);
double get_time_nsec(void) {
	struct timespec tp = { 0, 0 };

	clock_gettime( CLOCK_REALTIME, &tp );
	//printf( "nsec = %ld\n", ((double)tp.tv_sec + ((double)tp.tv_nsec)/1E9) );

	return (double)tp.tv_sec + ((double)tp.tv_nsec)/1E9;
}

void __usleep(int millisecond) {
	struct timeval tv;

	tv.tv_sec = 0;
	tv.tv_usec = millisecond * 1000;	// microsecond

	select( 0, NULL, NULL, NULL, &tv );
}

//void get_date_d(int* pYear, int* pMonth, int* pDay,
//		int* pHour, int* pMin, int* pSec, int* pMillisecond);
void get_date_d(int* pYear, int* pMonth, int* pDay,
		int* pHour, int* pMin, int* pSec, int* pMillisecond) {
	time_t curTime = time( (time_t*)NULL );
	//struct tm getDateTime = *localtime( &curTime );	// Thread not safe
	struct tm getDateTime;
   	localtime_r( &curTime, &getDateTime );	// Thread safe

	*pYear = (getDateTime.tm_year + 1900);
	*pMonth = (getDateTime.tm_mon + 1);
	*pDay = (getDateTime.tm_mday);
	*pHour = (getDateTime.tm_hour);
	*pMin = (getDateTime.tm_min);
	*pSec = (getDateTime.tm_sec) - 1;
	*pMillisecond = (int)labs( get_time_usec() );

	//printf( "year = %d, month = %d, day = %d, hour = %d, min = %d, sec = %d, millisecond = %d\n",
	//		(getDateTime.tm_year + 1900),
	//		(getDateTime.tm_mon + 1),
	//		(getDateTime.tm_mday),
	//		(getDateTime.tm_hour),
	//		(getDateTime.tm_min),
	//		(getDateTime.tm_sec) - 1,
	//		labs(get_time_usec()) );
	//printf( "year = %d, month = %d, day = %d, hour = %d, min = %d, sec = %d, millisecond = %d\n",
	//		pYear, pMonth, pDay,
	//		pHour, pMin, pSec, pMillisecond );

	return;
}

//void get_date(char* pBuf_Year, char* pBuf_Month, char* pBuf_Day,
//		char* pBuf_Hour, char* pBuf_Min, char* pBuf_Sec, char* pBuf_Millisecond);
void get_date(char* pBuf_Year, char* pBuf_Month, char* pBuf_Day,
		char* pBuf_Hour, char* pBuf_Min, char* pBuf_Sec, char* pBuf_Millisecond) {
	time_t curTime = time( (time_t*)NULL );
	//struct tm getDateTime = *localtime( &curTime );	// Thread not safe
	struct tm getDateTime;
   	localtime_r( &curTime, &getDateTime );	// Thread safe

	sprintf( pBuf_Year, "%d", (getDateTime.tm_year + 1900) );
	sprintf( pBuf_Month, "%d", (getDateTime.tm_mon + 1) );
	sprintf( pBuf_Day, "%d", (getDateTime.tm_mday) );
	sprintf( pBuf_Hour, "%d", (getDateTime.tm_hour) );
	sprintf( pBuf_Min, "%d", (getDateTime.tm_min) );
	sprintf( pBuf_Sec, "%d", (getDateTime.tm_sec) - 1 );
	sprintf( pBuf_Millisecond, "%ld", labs(get_time_usec()) );

	//printf( "year = %d, month = %d, day = %d, hour = %d, min = %d, sec = %d, millisecond = %d\n",
	//		(getDateTime.tm_year + 1900),
	//		(getDateTime.tm_mon + 1),
	//		(getDateTime.tm_mday),
	//		(getDateTime.tm_hour),
	//		(getDateTime.tm_min),
	//		(getDateTime.tm_sec) - 1,
	//		labs(get_time_usec()) );
	//printf( "year = %s, month = %s, day = %s, hour = %s, min = %s, sec = %s, millisecond = %s\n",
	//		pBuf_Year, pBuf_Month, pBuf_Day,
	//		pBuf_Hour, pBuf_Min, pBuf_Sec, pBuf_Millisecond );

	return;
}

//void print_datetime_d(void);
void print_datetime_d(void) {
	int year;
	int month;
	int day;
	int hour;
	int min;
	int sec;
	int millisecond;

	//get_time_usec();
	get_date_d( &year, &month, &day, &hour, &min, &sec, &millisecond );
	fprintf( stdout, "[%d:%d:%d:%d:%d:%d:%d]\n",
		year, month, day, hour, min, sec, millisecond );

	return;
}

//void print_datetime(void);
void print_datetime(void) {
	char buf_Year[4] = {0,};
	char buf_Month[2] = {0,};
	char buf_Day[2] = {0,};
	char buf_Hour[2] = {0,};
	char buf_Min[2] = {0,};
	char buf_Sec[2] = {0,};
	char buf_Millisecond[10] = {0,};

	/*
	memset( (void*)buf_Year, 0x00, sizeof(buf_Year) );
	memset( (void*)buf_Month, 0x00, sizeof(buf_Month) );
	memset( (void*)buf_Day, 0x00, sizeof(buf_Day) );
	memset( (void*)buf_Hour, 0x00, sizeof(buf_Hour) );
	memset( (void*)buf_Min, 0x00, sizeof(buf_Min) );
	memset( (void*)buf_Sec, 0x00, sizeof(buf_Sec) );
	memset( (void*)buf_Millisecond, 0x00, sizeof(buf_Millisecond) );
	*/

	//get_time_usec();
	get_date( buf_Year, buf_Month, buf_Day, buf_Hour, buf_Min, buf_Sec, buf_Millisecond );
	fprintf( stdout, "[%s:%s:%s:%s:::%s]\n",
		buf_Year, buf_Month, buf_Day, buf_Hour, /*buf_Min, buf_Sec,*/ buf_Millisecond );

	return;
}

//
// Alarm
//
/*
void sig_alarm(int);
void sig_alarm(int signo) {
	fprintf( stderr, "Error, Socket Timeout\n" );
	return;
}
*/
#endif	// __LINUX__

//
// Log
//
#ifdef __REQ_DEBUG_MESSAGE__
void _DBG_log_file(const char* pStrLog, const char* pNewFilename, const bool isInit) {
	FILE* fp;

	if ( isInit )
		fp = fopen( (pNewFilename? pNewFilename : g_STR_LOG_FILENAME), "w" );
	else
		fp = fopen( (pNewFilename? pNewFilename : g_STR_LOG_FILENAME), "a+" );

	if ( fp < 0 ) return;

	fseek( fp, 0, SEEK_END );
	fwrite( pStrLog, 1, strlen(pStrLog), fp );
	//fwrite( "\r\n", 1, strlen("\r\n"), fp );
	fclose( fp );
}

void _DBG_log(const char* pFmt, ...) {
	char buf_Log[g_INT_MAX_LENGTH_LOG+1];
	va_list pArgList;

	memset( buf_Log, 0x00, g_INT_MAX_LENGTH_LOG+1 );

	va_start( pArgList, pFmt );
	vsnprintf( buf_Log, sizeof(buf_Log), pFmt, pArgList );
	va_end( pArgList );

	// Write to a file
	_DBG_log_file( buf_Log, NULL );

	// Write to Console
	fprintf( stdout, "%s", buf_Log );
	//fprintf( stdout, "\r\n" );
}

/*
	@Method: __LOG_FMT__
	@Description: print log
	@Params:
		[IN] const char*
	@Return:
		[OUT] None
	@Reference
		Source:
*/
void __LOG_FMT__(const char* pFmt, ...) {
	char buf_Log[g_INT_MAX_LENGTH_LOG+1];
	va_list pArgList;

	memset( buf_Log, 0x00, g_INT_MAX_LENGTH_LOG+1 );

	va_start( pArgList, pFmt );
	vsnprintf( buf_Log, sizeof(buf_Log), pFmt, pArgList );
	va_end( pArgList );

	// Write to a file
	//_DBG_log_file( buf_Log, NULL );

	// Write to Console
	fprintf( stdout, "%s\n", buf_Log );
	//fprintf( stdout, "\r\n" );
}

void __LOG_FMT_TAG__(const char* TAG, const char* pFmt, ...) {
	char buf_Log[g_INT_MAX_LENGTH_LOG+1];
	va_list pArgList;

	memset( buf_Log, 0x00, g_INT_MAX_LENGTH_LOG+1 );

	va_start( pArgList, pFmt );
	vsnprintf( buf_Log, sizeof(buf_Log), pFmt, pArgList );
	va_end( pArgList );

	// Write to a file
	//_DBG_log_file( buf_Log, NULL );

	// Write to Console
	fprintf( stdout, "[%s] %s\n", TAG, buf_Log );
	//fprintf( stdout, "\r\n" );
}

void _DBG_log_init(void) {
	// Write to a file
	_DBG_log_file( "", NULL, true );
}
#else
void _DBG_log_file(const char* pStrLog, const char* pNewFilename, const bool isInit) {}
void _DBG_log(const char* pFmt, ...) {}
void __LOG_FMT__(const char* pFmt, ...) {}
void __LOG_FMT_TAG__(const char* TAG, const char* pFmt, ...) {}
void _DBG_log_init(void) {}
#endif	// __REQ_DEBUG_MESSAGE__

#ifdef __cplusplus
	}	// extern "C"
#endif	// cplusplus



// --------------------------------------------------------------------------
//! Class / Structure / Namespace
// --------------------------------------------------------------------------
// --------------------------------------------------------------------------



// __EOF__
