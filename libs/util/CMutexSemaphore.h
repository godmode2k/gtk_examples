#ifndef __CMUTEXSEMAPHORE_H__
#define __CMUTEXSEMAPHORE_H__

/* --------------------------------------------------------------
Project:	Mutex & Semaphore
Purpose:	Thread processing
Author:		Ho-Jung Kim (godmode2k@hotmail.com)
Date:		Since June 28, 2012
Filename:	CMutexSemaphore.h

Last modified: July 28, 2014
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

#ifdef __LINUX__
#elif _WINDOWS
	#pragma pack( push, 1 )
#else
#endif
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

#ifdef __cplusplus
}
#endif
// ---------------------------------------------------------------



//! Class
// ---------------------------------------------------------------
class CThreadMutex {
private:
//	const char* TAG;

	// POSIX Thread Mutex Lock
	//pthread_mutex_t g_pthread_mutex_lock = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t m_mutex_lock;

/*
	void __set_class_tag(const char* tag);
	//void _set_tag(const std::string str);
*/

/*
	bool __init(void);
#ifdef __ANDROID_NDK__
	//! NOTE: Don't use named "__release", which is reserved in NDK
	void ___release(void);
#else
	void __release(void);
#endif
*/
protected:
public:
	// Ctor/Dtor
	explicit CThreadMutex(void);
	explicit CThreadMutex(pthread_mutex_t& mutex_lock) : m_mutex_lock(mutex_lock) { _lock(); }
	//explicit CThreadMutex(const char* tag);
	//explicit CThreadMutex(const char* tag, pthread_mutex_t& mutex_lock);
	virtual ~CThreadMutex() { _unlock(); }



	inline void _set_mutex(pthread_mutex_t& mutex_lock) { m_mutex_lock = mutex_lock; }
	inline void _init_mutex(void) { pthread_mutex_init( &m_mutex_lock, NULL ); }
	inline void _release_mutex(void) { pthread_mutex_destroy( &m_mutex_lock ); }

	inline void _lock(void) { pthread_mutex_lock( &m_mutex_lock ); }
	inline void _unlock(void) { pthread_mutex_unlock( &m_mutex_lock ); }


	inline void lock(pthread_mutex_t& mutex_lock) { pthread_mutex_lock( &mutex_lock ); }
	inline void unlock(pthread_mutex_t& mutex_lock) { pthread_mutex_unlock( &mutex_lock ); }
};



#if 0
class CThreadSemaphore {
private:
//	const char* TAG;


/*
	void __set_class_tag(const char* tag);
	//void _set_tag(const std::string str);
*/

/*
	bool __init(void);
#ifdef __ANDROID_NDK__
	//! NOTE: Don't use named "__release", which is reserved in NDK
	void ___release(void);
#else
	void __release(void);
#endif
*/
protected:
public:
	// Ctor/Dtor
	explicit CThreadSemaphore(void);
	//explicit CThreadSemaphore(const char* tag);
	virtual ~CThreadSemaphore() { unlock(); }



	inline void lock(void) { }
	inline void unlock(void) { }
};
#endif
// ---------------------------------------------------------------



#endif	// __CMUTEXSEMAPHORE_H__

