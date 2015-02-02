#ifndef __CTHREADTASK_H__
#define __CTHREADTASK_H__

/* --------------------------------------------------------------
Project:	ThreadTask
Purpose:	Thread processing
Author:		Ho-Jung Kim (godmode2k@hotmail.com)
Date:		Since June 28, 2012
Filename:	CThreadTask.h

Last modified: Nov 28, 2014
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
1. Build:
	$ g++ ... -D__LINUX__ -D_REENTRANT -lpthread -lrt
-------------------------------------------------------------- */



//! Header and Definition
// ---------------------------------------------------------------
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <sstream>		// stringstream

#include <cstdio>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cstdarg>		// Variable Argument Lists
//#include <stdio.h>
//#include <string.h>
//#include <stdlib.h>
//#include <stdarg.h>
#include <errno.h>		// error code

#ifdef __LINUX__
#include <unistd.h>
#include <pthread.h>	// POSIX Thread
#include <signal.h>		// SIGKILL
#include <stdbool.h>	// bool
#else
#endif

#include "util/CUtil_DEF.h"
#include "util/CUtil.h"
// ---------------------------------------------------------------



//! Definition
// ---------------------------------------------------------------
#ifdef __LINUX__
	#define __PACK__	__attribute__((packed))
#else
	#define __PACK__
#endif

// LOG
//#define __REQ_DEBUG_MESSAGE__

#ifdef __cplusplus
extern "C" {
#endif

class CThreadTask;

#ifdef __LINUX__
#elif _WINDOWS
	#pragma pack( push, 1 )
#else
#endif
typedef struct {
	int a;	// TEST
	int b;	// TEST
	bool useVALists;	// Variable Argument Lists
	CThreadTask* th;	// this pointer(*this)
	void* pExtraVal_postExecute;	// Extra value
} __PACK__ threadtask_pthread_args_st;
#ifdef __LINUX__
#elif _WINDOWS
	#pragma pack( pop )
#else
#endif

#ifdef __cplusplus
}
#endif
// ---------------------------------------------------------------



//! Prototype
// ---------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif

// ...

//void thread_cleanup_task(void* args);

#ifdef __cplusplus
}
#endif
// ---------------------------------------------------------------



//! Class
// ---------------------------------------------------------------
#if 0
class CThreadTask {
private:
	static CThreadTask m_instance;
	CThreadTask() {
	}

	static CThreadTask& getInstance();
	/*
	// in .cpp
	static CThreadTask& getInstance() {
		if ( m_instance == NULL )
			m_instance = new CThreadTask();

		return m_instance;
	}
	*/
};
#endif

#define DEFAULT_MAX_TAG_BUF_SIZE		24
#define DEFAULT_MAX_THREAD_TAG_BUF_SIZE	12
class CThreadTask {
private:
	const char* TAG;
	char _TAG_BUF[DEFAULT_MAX_TAG_BUF_SIZE];
	const char* THREAD_TAG;
	char _THREAD_TAG_BUF[DEFAULT_MAX_THREAD_TAG_BUF_SIZE];
	long THREAD_TAG_ID;

	//! TODO: Thread Id List
	// Thread Id
	pthread_t m_threadId;
	// Variable Argument Lists
	std::vector<void*>* m_pvecVALists;
	// Cancel state
	bool m_cancel;
	bool m_cleanup_task;

	void __set_class_tag(const char* tag);
	//void _set_tag(const std::string str);

	bool __init(void);
#ifdef __ANDROID_NDK__
	//! NOTE: Don't use named "__release", which is reserved in NDK
	void ___release(void);
#else
	void __release(void);
#endif

	std::vector<void*>* getVALists(void);

	static void* __thread_run(void* pArgs);
	static void __thread_cleanup_task(void* args);
	void thread_cleanup_task(void* args);
	void set_cleanup_task(bool val) { m_cleanup_task = val; }
	void wait_for_cancelled_thread(void);
protected:
public:
	// Ctor/Dtor
	explicit CThreadTask(void);
	explicit CThreadTask(const char* tag);
	virtual ~CThreadTask();

	/*
	template<typename T>
	void set_tag(const T tag) {
		std::stringstream ss;
		std::string str;

		ss << tag;
		str = ss.str();

		_set_tag( str );
	}
	*/
	void set_tag(const char* tag);
	void set_tag(const int tag);
	const char* get_tag(void);
	void set_tag_id(const int tag);
	const int get_tag_id(void);

	void setClearVALists(void);

	bool execute(void* pExtraVal = NULL);
	bool execute(void* pExtraVal, char* pFmt, ...);
	void update(void* pVal);
	void cancel(void);
	bool get_cancel_state(void);
	bool get_cleanup_task(void) { return m_cleanup_task; }

/*
#ifdef __REQ_MAIN_FUNCTION__
	//virtual void* inBackground(void* pVal = NULL);
	virtual void* inBackground(std::vector<void*>* pvecVal = NULL);
	virtual void progressUpdate(void* pVal);
	virtual void postExecute(void* pResult, void* pExtraVal = NULL);
#else
	//virtual void* inBackground(void* pVal = NULL) = 0;
	virtual void* inBackground(std::vector<void*>* pvecVal = NULL) = 0;
	virtual void progressUpdate(void* pVal) = 0;
	virtual void postExecute(void* pResult, void* pExtraVal = NULL) = 0;
#endif
*/
	//virtual void* inBackground(void* pVal = NULL) = 0;
	virtual void* inBackground(std::vector<void*>* pvecVal = NULL) = 0;
	virtual void progressUpdate(void* pVal) = 0;
	virtual void postExecute(void* pResult, void* pExtraVal = NULL) = 0;
public:
	//! TEST
	int x;
	int y;
};
// ---------------------------------------------------------------



#endif	// __CTHREADTASK_H__
