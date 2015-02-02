/* --------------------------------------------------------------
Project:	ThreadTask
Purpose:	Thread processing
Author:		Ho-Jung Kim (godmode2k@hotmail.com)
Date:		Since June 28, 2012
Filename:	CThreadTask.cpp

Last modified: Jan 5, 2015
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



//! Header
// ---------------------------------------------------------------
#include "CThreadTask.h"

//#include "util/CUtil_DEF.h"
// ---------------------------------------------------------------



//! Definition
// ---------------------------------------------------------------
// ---------------------------------------------------------------



//! Prototype
// ---------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif

/*
@Method: thread_cleanup_task
@Description: cleanup the extra value
@Params:
	[IN] void*
@Return:
	[OUT] None
@Reference
	Source:
@NOTE
*/
/*
void thread_cleanup_task(void* args) {
	const char* TAG = "CThreadTask";
	__LOGT__( TAG, "thread_cleanup_task()" );

	if ( args ) {
		threadtask_pthread_args_st* task = reinterpret_cast<threadtask_pthread_args_st*>(args);

		if ( task ) {
			CThreadTask* th = reinterpret_cast<CThreadTask*>(task->th);
			void* pRet = NULL;

			if ( th ) {
				th->set_cleanup_task( true );

				if ( task->pExtraVal_postExecute != NULL )
					th->postExecute( pRet, task->pExtraVal_postExecute );
				else
					th->postExecute( pRet, NULL );

				th->set_cleanup_task( false );
			}
		}

		delete (threadtask_pthread_args_st*)args;
		args = NULL;

		__LOGT__( TAG, "thread_cleanup_task(): released" );
	}
}
*/

#ifdef __cplusplus
}
#endif
// ---------------------------------------------------------------



//! Class
// ---------------------------------------------------------------
/*
@Method: CThreadTask
@Description: Ctor
@Params:
	[IN] None
@Return:
	[OUT] None
@Reference
	Source:
@NOTE
*/
CThreadTask::CThreadTask(void) : TAG("CThreadTask") {
	//__LOGT__( TAG, "CThreadTask()" );

	// Init
	if ( !__init() )
		return;
}
//CThreadTask::CThreadTask(const char* tag) : TAG(INIT_TAG(CThreadTask, tag)) {
CThreadTask::CThreadTask(const char* tag) {
	//__LOGT__( TAG, "CThreadTask()" );

	__set_class_tag( tag );

	// Init
	if ( !__init() )
		return;
}


/*
@Method: CThreadTask
@Description: Dtor
@Params:
	[IN] None
@Return:
	[OUT] None
@Reference
	Source:
@NOTE
*/
CThreadTask::~CThreadTask() {
	//__LOGT__( TAG, "~CThreadTask()" );

#ifdef __ANDROID_NDK__
	//! NOTE: Don't use named "__release", which is reserved in NDK
	___release();
#else
	__release();
#endif
}

/*
@Method: __set_class_tag
@Description: Set a tag for the Class
@Params:
	[IN] const char*
@Return:
	[OUT] None
@Reference
	Source:
@NOTE
*/
void CThreadTask::__set_class_tag(const char* tag) {
#ifdef __ANDROID_NDK__
	//! NOTE: Don't use named "_TAG" for variable here, in NDK, it may occured error the following:
	//		"error: expected unqualified-id before string constant"
	//
	//	wtf? are you kiddin me? i will have to rename the variable "_TAG" to avoid this shit.
	const char* default_TAG = "CThreadTask";
#else
	const char* _TAG = "CThreadTask";
#endif
	std::stringstream ss;
	std::string str;

#ifdef __ANDROID_NDK__
	ss << default_TAG << ":[" << tag << "]";
#else
	ss << _TAG << ":[" << tag << "]";
#endif
	str = ss.str();
	snprintf( _TAG_BUF, sizeof(_TAG_BUF), "%s", str.c_str() );
	TAG = _TAG_BUF;
}

/*
@Method: __init
@Description: Initialize
@Params:
	[IN] None
@Return:
	[OUT] bool
@Reference
	Source:
@NOTE
*/
bool CThreadTask::__init(void) {
	__LOGT__( TAG, "__init()" );

	THREAD_TAG = NULL;
	THREAD_TAG_ID = -1;

	// Thread Id
#ifdef __LINUX__
	m_threadId = 0;
#elif _WINDOWS
	memset( (void*)&m_threadId, 0x00, sizeof(pthread_t) );
#else
	memset( (void*)&m_threadId, 0x00, sizeof(pthread_t) );
#endif

	// Variable Argument Lists
	m_pvecVALists = new std::vector<void*>;

	// Cancel state
	m_cancel = false;

	// Cleanup state
	m_cleanup_task = false;



	//! TEST
	x = 100;
	y = 200;


	return true;
}

/*
@Method: __release
@Description: Initialize
@Params:
	[IN] None
@Return:
	[OUT] None
@Reference
	Source:
@NOTE
*/
#ifdef __ANDROID_NDK__
	//! NOTE: Don't use named "__release", which is reserved in NDK
void CThreadTask::___release(void) {
#else
void CThreadTask::__release(void) {
#endif
	__LOGT__( TAG, "__release()" );

	// Send a no signal(zero is no signal) to thread
	// for error checking perform.
#ifdef __LINUX__
	// int status;
	//if ( pthread_tryjoin_np(m_threadId, (void**)&status == 0 ) // return 0, if success {}
	if ( !m_threadId || pthread_kill(m_threadId, 0) == 0 ) {
		__LOGT__( TAG, "__release(): It may be the Thread is joinable" );
		__LOGT__( TAG, "__release(): NOTE: each thread are detached already" );

		int ret = pthread_join( m_threadId, NULL );	// return 0, if success

		if ( ret != 0 ) {
			__LOGT__( TAG, "__release(): Thread joined [FAIL]" );

			if ( ret == ESRCH ) {
				__LOGT__( TAG, "__release(): ret = %d, "	\
						"ESRCH: no such process", ret );
			}
			else if ( ret == EINVAL ) {
				__LOGT__( TAG, "__release(): ret = %d, "	\
						"EINVAL: invalid argument", ret );
			}
			else {
				__LOGT__( TAG, "__release(): ret = %d, Send a KILL signal", ret );

				//pthread_cancel( m_threadId ); // pthread_cleanup_push(), pthread_cleanup_pop()
				pthread_kill( m_threadId, SIGKILL );
				//pthread_exit( PTHREAD_CANCELED );
//				pthread_exit( 0 );
			}
		}
	}
	else {
		__LOGT__( TAG, "__release(): Thread not found" );
	}
#elif _WINDOWS
	{
		__LOGT__( TAG, "__release(): It may be the Thread is joinable" );

		int ret = pthread_join( m_threadId, NULL );	// return 0, if success

		if ( ret != 0 ) {
			__LOGT__( TAG, "__release(): Thread joined [FAIL]" );

			if ( ret == ESRCH ) {
				__LOGT__( TAG, "__release(): ret = %d, "	\
						"ESRCH: no such process", ret );
			}
			else if ( ret == EINVAL ) {
				__LOGT__( TAG, "__release(): ret = %d, "	\
						"EINVAL: invalid argument", ret );
			}
			else {
				__LOGT__( TAG, "__release(): ret = %d, Send a KILL signal", ret );

				//pthread_cancel( m_threadId ); // pthread_cleanup_push(), pthread_cleanup_pop()
				//pthread_kill( m_threadId, SIGKILL );
				pthread_kill( m_threadId, 1 );	// SIGHUP(1): hangup
				//pthread_exit( PTHREAD_CANCELED );
//				pthread_exit( 0 );
			}
		}
	}
#else
#endif

	// Variable Argument Lists
	if ( m_pvecVALists ) {
		if ( m_pvecVALists->size() > 0 ) {
			std::vector<void*>::iterator iter;
			for ( iter = m_pvecVALists->begin(); iter != m_pvecVALists->end(); ++iter ) {
				// ...


				//iter = m_pvecVALists->erase( iter );
			}

			m_pvecVALists->erase( m_pvecVALists->begin(), m_pvecVALists->end() );
		}

		m_pvecVALists->clear();
		delete m_pvecVALists;
		m_pvecVALists = NULL;
	}
}

/*
@Method: setClearVALists
@Description:
@Params:
	[IN] None
@Return:
	[OUT] None
@Reference
	Source:
@NOTE
*/
void CThreadTask::setClearVALists(void) {
	// Variable Argument Lists
	if ( m_pvecVALists ) {
		if ( m_pvecVALists->size() > 0 ) {
			std::vector<void*>::iterator iter;
			for ( iter = m_pvecVALists->begin(); iter != m_pvecVALists->end(); ) {
				iter = m_pvecVALists->erase( iter );
			}
		}

		m_pvecVALists->clear();
		//delete m_pvecVALists;
		//m_pvecVALists = NULL;
	}
}

/*
@Method: getVALists
@Description:
@Params:
	[IN] None
@Return:
	[OUT] None
@Reference
	Source:
@NOTE
*/
std::vector<void*>* CThreadTask::getVALists(void) {
	if ( m_pvecVALists && m_pvecVALists->size() > 0 )
		return m_pvecVALists;
	return NULL;
}

/*
void CThreadTask::_set_tag(std::string str) {
	snprintf( _THREAD_TAG_BUF, sizeof(_THREAD_TAG_BUF), "%s", str.c_str() );
	THREAD_TAG = _THREAD_TAG_BUF;
}
template<typename T>
void CThreadTask::set_tag(const T tag) {
	std::stringstream ss;
	std::string str;

	ss << tag;
	str = ss.str();
	snprintf( _THREAD_TAG_BUF, sizeof(_THREAD_TAG_BUF), "%s", str.c_str() );
	THREAD_TAG = _THREAD_TAG_BUF;
}
*/
void CThreadTask::set_tag(const char* tag) {
	snprintf( _THREAD_TAG_BUF, sizeof(_THREAD_TAG_BUF), "%s", tag );
	THREAD_TAG = _THREAD_TAG_BUF;
}
void CThreadTask::set_tag(const int tag) {
	snprintf( _THREAD_TAG_BUF, sizeof(_THREAD_TAG_BUF), "%d", tag );
	THREAD_TAG = _THREAD_TAG_BUF;
}
const char* CThreadTask::get_tag(void) {
	//__LOGT__( TAG, "get_tag(): THREAD TAG = %s", (THREAD_TAG? THREAD_TAG : "NULL") );
	return THREAD_TAG;
}
void CThreadTask::set_tag_id(const int tag) {
	THREAD_TAG_ID = tag;
}
const int CThreadTask::get_tag_id(void) {
	//__LOGT__( TAG, "get_tag_id(): THREAD TAG ID = %ld", THREAD_TAG_ID );
	return THREAD_TAG_ID;
}


/*
@Method: execute
@Description: Execute thread task
@Params:
	[IN] void* pVal = NULL
@Return:
	[OUT] bool
@Reference
	Source:
@NOTE
	- execute() -> __thread_run() -> inBackground() -> (update()->progressUpdate()) -> postExecute()
*/
bool CThreadTask::execute(void* pExtraVal) {
	__LOGT__( TAG, "execute()" );

	{
		m_cancel = false;
		m_cleanup_task = false;
	}


	threadtask_pthread_args_st* pThreadArgs = NULL;
	//pThreadArgs = (threadtask_pthread_args_st*)malloc( sizeof(threadtask_pthread_args_st) );
	pThreadArgs = new threadtask_pthread_args_st;
	if ( pThreadArgs == NULL ) {
		__LOGT__( TAG, "execute(): Thread argument structure initialized [FAIL]" );
		return false;
	}

	pThreadArgs->a = 1;
	pThreadArgs->b = 2;
	pThreadArgs->useVALists = false;
	pThreadArgs->th = this;
	if ( pExtraVal != NULL )
		pThreadArgs->pExtraVal_postExecute = pExtraVal;
	else
		pThreadArgs->pExtraVal_postExecute = NULL;

	//! check a released resource
	/*
	pthread_attr_t tattr;
	pthread_t tid;
	void *start_routine;
	void arg
	int ret;
	...
	// initialized with default attributes
	ret = pthread_attr_init(&tattr);
	ret = pthread_attr_setdetachstate(&tattr,PTHREAD_CREATE_DETACHED);
	ret = pthread_create(&tid, &tattr, start_routine, arg);
	...
	*/


	// Thread
	if ( pthread_create(&m_threadId, NULL, __thread_run, (void*)pThreadArgs) != 0 ) {
		__LOGT__( TAG, "execute(): Thread created [FAIL]" );
		return false;
	}

	__LOGT__( TAG, "execute(): thread id = %lu", m_threadId );

	/*
	int ret = pthread_join( m_threadId, NULL );	// return 0, if success
	if ( ret != 0 ) {
		__LOGT__( TAG, "execute(): Thread joined [FAIL]" );

		//pthread_cancel( m_threadId ); // pthread_cleanup_push(), pthread_cleanup_pop()
		pthread_kill( m_threadId, SIGKILL );
		pthread_exit( PTHREAD_CANCELED );
		return false;
	}
	*/

	return true;
}

bool CThreadTask::execute(void* pExtraVal, char* pFmt, ...) {
	__LOGT__( TAG, "execute(...)" );

	{
		m_cancel = false;
		m_cleanup_task = false;
	}


	threadtask_pthread_args_st* pThreadArgs = NULL;
	//pThreadArgs = (threadtask_pthread_args_st*)malloc( sizeof(threadtask_pthread_args_st) );
	pThreadArgs = new threadtask_pthread_args_st;
	if ( pThreadArgs == NULL ) {
		__LOGT__( TAG, "execute(...): Thread argument structure initialized [FAIL]" );
		return false;
	}

	pThreadArgs->a = 1;
	pThreadArgs->b = 2;
	pThreadArgs->useVALists = true;
	if ( pExtraVal != NULL )
		pThreadArgs->pExtraVal_postExecute = pExtraVal;
	else
		pThreadArgs->pExtraVal_postExecute = NULL;

	// Variable Argument Lists
	{
		bool found = false;
		va_list ap;
		int VA_int;
		long VA_long;
		double VA_float;	// float/doule
		char VA_char;
		char* pVA_str;
		// ...

		va_start( ap, pFmt );
		while ( *pFmt ) {
			switch ( *pFmt++ ) {
				case 'd':	// int
				{
					found = true;

					VA_int = va_arg( ap, int );
					__LOGT__( TAG, "execute(...): INT: %d", VA_int );
					m_pvecVALists->push_back( (void*)VA_int );
				} break;

				case 'l':	// long
				{
					found = true;

					VA_long = va_arg( ap, long );
					__LOGT__( TAG, "execute(...): LONG: %ld", VA_long );
					m_pvecVALists->push_back( (void*)VA_long );
				} break;

				case 'f':	// float/double
				{
					found = true;

					VA_float = va_arg( ap, double );
					__LOGT__( TAG, "execute(...): FLOAT/DOUBLE: %f", VA_float );

					//! TODO:
					//m_pvecVALists->push_back( (void*)VA_float );
				} break;

				case 'c':	// char
				{
					found = true;

					//! TODO:  need a cast here since va_arg only takes fully promoted types
					VA_char = (char)va_arg( ap, int );
					__LOGT__( TAG, "execute(...): CHAR: %c", VA_char );
					m_pvecVALists->push_back( (void*)&VA_char );
				} break;

				case 's':	// String
				{
					//found = !found;
					found = true;

					pVA_str = va_arg( ap, char* );
					__LOGT__( TAG, "execute(...): STRING: %s 0x%x", pVA_str, pVA_str );
					m_pvecVALists->push_back( (void*)pVA_str );
				} break;
			} // switch()
		} // while()
		va_end( ap );

		if ( !found )
			pThreadArgs->useVALists = false;
	}
	pThreadArgs->th = this;



	// Thread
	if ( pthread_create(&m_threadId, NULL, __thread_run, (void*)pThreadArgs) != 0 ) {
		__LOGT__( TAG, "execute(...): Thread created [FAIL]" );
		return false;
	}

	__LOGT__( TAG, "execute(...): thread id = %lu", m_threadId );

	/*
	int ret = pthread_join( m_threadId, NULL );	// return 0, if success
	if ( ret != 0 ) {
		__LOGT__( TAG, "execute(...): Thread joined [FAIL]" );

		//pthread_cancel( m_threadId ); // pthread_cleanup_push(), pthread_cleanup_pop()
		pthread_kill( m_threadId, SIGKILL );
		pthread_exit( PTHREAD_CANCELED );
		return false;
	}
	*/

	return true;
}

/*
@Method: __thread_run
@Description: run a task
@Params:
	[IN] void* pArgs
@Return:
	[OUT] void*
@Reference
	Source:
@NOTE
	- This is a STATIC member function
*/
void* CThreadTask::__thread_run(void* pArgs) {
	const char* const TAG = "CThreadTask";

	//__LOGT__( TAG, "__thread_run()" );

	pthread_detach( pthread_self() );
	pthread_cleanup_push( CThreadTask::__thread_cleanup_task, pArgs );


	//usleep( 1000000 );	// microsecond: 1sec
	threadtask_pthread_args_st* args = reinterpret_cast<threadtask_pthread_args_st*>(pArgs);
	if ( args ) {
		CThreadTask* th = reinterpret_cast<CThreadTask*>(args->th);

		if ( th ) {
/*
#ifdef __REQ_MAIN_FUNCTION__
			__LOGT__( TAG, "__thread_run(): x = %d, y = %d", th->x, th->y );
#endif
*/

			void* pRet = NULL;
			/*
			pRet = th->inBackground();
			if ( args->pExtraVal_postExecute != NULL )
				th->postExecute( pRet, args->pExtraVal_postExecute );
			else
				th->postExecute( pRet, NULL );
			*/

			if ( !args->useVALists ) {
				__LOGT__( TAG, "__thread_run(): USE Variable Argument Lists [FALSE]" );
				pRet = th->inBackground();
			}
			else {
				__LOGT__( TAG, "__thread_run(): USE Variable Argument Lists [TRUE]" );
				pRet = th->inBackground( th->getVALists() );
			}

			if ( args->pExtraVal_postExecute != NULL )
				th->postExecute( pRet, args->pExtraVal_postExecute );
			else
				th->postExecute( pRet, NULL );
		}

		//delete pArgs;
		delete args;
		args = NULL;
	}
	else {
		if ( pArgs ) {
			delete (threadtask_pthread_args_st*)pArgs;
			pArgs = NULL;
		}

		//pthread_cleanup_pop( 0 );

		__LOGT__( TAG, "__thread_run(): finish without work" );
		//pthread_cancel();
		//pthread_exit( PTHREAD_CANCELED );
		//
		//pthread_exit( 0 );
		//return NULL;
	}
	
	//delete pArgs;
	//delete args;
	//args = NULL;

	pthread_cleanup_pop( 0 );

//#ifdef __REQ_MAIN_FUNCTION__
	__LOGT__( TAG, "__thread_run(): finish" );
//#endif

	//pthread_cancel();
	//pthread_exit( PTHREAD_CANCELED );
	//
	pthread_exit( 0 );


	return NULL;
}

/*
@Method: update
@Description: Update a progress states
@Params:
	[IN] void* pVal
@Return:
	[OUT] void*
@Reference
	Source:
@NOTE
	- Invoke "this->progressUpdate()"
*/
void CThreadTask::update(void* pVal) {
	//__LOGT__( TAG, "update()" );
	
	progressUpdate( pVal );
}

/*
@Method: cancel 
@Description: Cancel the progress states
@Params:
	[IN] None
@Return:
	[OUT] None
@Reference
	Source:
@NOTE
*/
void CThreadTask::cancel(void) {
	//__LOGT__( TAG, "cancel()" );
	
	m_cancel = true;

	__LOGT__( TAG, "cancel(): cancelled" );
	pthread_cancel( m_threadId );

	// wait for cancelled thread task
	wait_for_cancelled_thread();


/*
#ifdef __ANDROID_NDK__
	//! NOTE: Don't use named "__release", which is reserved in NDK
	___release();
#else
	__release();
#endif
*/
}

/*
@Method: get_cancel_state
@Description: Cancel state
@Params:
	[IN] None
@Return:
	[OUT] bool 
@Reference
	Source:
@NOTE
*/
bool CThreadTask::get_cancel_state(void) {
	//__LOGT__( TAG, "get_cancel_state()" );
	
	return m_cancel;
}

/*
@Method: thread_cleanup_task
@Description: cleanup the extra value
@Params:
	[IN] void*
@Return:
	[OUT] None
@Reference
	Source:
@NOTE
*/
void CThreadTask::__thread_cleanup_task(void* args) {
	const char* const TAG = "CThreadTask";

	__LOGT__( TAG, "__thread_cleanup_task()" );

	if ( args ) {
		threadtask_pthread_args_st* task = reinterpret_cast<threadtask_pthread_args_st*>(args);

		if ( task ) {
			CThreadTask* th = reinterpret_cast<CThreadTask*>(task->th);

			if ( th ) {
				th->set_cleanup_task( true );
				th->thread_cleanup_task( args );
				th->set_cleanup_task( false );
			}
		}
	}
}

void CThreadTask::thread_cleanup_task(void* args) {
	__LOGT__( TAG, "thread_cleanup_task()" );

	if ( args ) {
		threadtask_pthread_args_st* task = reinterpret_cast<threadtask_pthread_args_st*>(args);

		if ( task ) {
			CThreadTask* th = reinterpret_cast<CThreadTask*>(task->th);
			void* pRet = NULL;

			if ( th ) {
				if ( task->pExtraVal_postExecute != NULL )
					th->postExecute( pRet, task->pExtraVal_postExecute );
				else
					th->postExecute( pRet, NULL );
			}
		}

		delete (threadtask_pthread_args_st*)args;
		args = NULL;

		__LOGT__( TAG, "thread_cleanup_task(): released" );
	}
}

void CThreadTask::wait_for_cancelled_thread(void) {
	__LOGT__( TAG, "wait_for_cancelled_thread()" );

	while ( get_cleanup_task() ) {
		__usleep( 100 );
	}

	__LOGT__( TAG, "wait_for_cancelled_thread(): finished" );
}


/*
@Method: inBackground
@Description: do something in background
@Params:
	//[IN] void* pVal = NULL
	[IN] std::vector<void*>* pvecVal
@Return:
	[OUT] void*
@Reference
	Source:
@NOTE
*/
//void* CThreadTask::inBackground(void* pVal) {
void* CThreadTask::inBackground(std::vector<void*>* pvecVal) {
	//__LOGT__( TAG, "inBackground()" );

/*
#ifdef __REQ_MAIN_FUNCTION__
	for ( int i = 0; i < 10; i++ ) {
		__LOGT__( TAG, "inBackground(): i = %d", i );
		update( (void*)"#---update---#" );
	}

	__LOGT__( TAG, "" );

	if ( pvecVal && (pvecVal->size() > 0) ) {
		//std::vector<void*>::iterator iter;
		//for ( iter = pvecVal->begin(); iter != pvecVal->end(); iter++ ) {
		//	__LOGT__( TAG, "inBackground(): str -> %s", (char*)(*iter) );
		//}

		__LOGT__( TAG, "inBackground(): str[0] -> %s", (char*)(*pvecVal)[0] );
		__LOGT__( TAG, "inBackground(): str[1] -> %s", (char*)(*pvecVal)[1] );
		__LOGT__( TAG, "inBackground(): str[2] -> %d", (int)(*pvecVal)[2] );
		__LOGT__( TAG, "inBackground(): str[3] -> %s", (char*)(*pvecVal)[3] );
	}

	//return NULL;
	return ((void*)true);
#else
	return NULL;
#endif
*/
	return NULL;
}

/*
@Method: progressUpdate
@Description: Update a progress states
@Params:
	[IN] void* pVal
@Return:
	[OUT] None
@Reference
	Source:
@NOTE
	 - Will be invoked from this->update()
*/
void CThreadTask::progressUpdate(void* pVal) {
	//__LOGT__( TAG, "progressUpdate()" );

/*
#ifdef __REQ_MAIN_FUNCTION__
	char* pStr = reinterpret_cast<char*>(pVal);
	if ( pStr )
		__LOGT__( TAG, "%s", pStr );
#endif
*/
}

/*
@Method: postExecute
@Description: Post-Execution (this->execute())
@Params:
	[IN] void* pResult
	[IN] void* pExtraVal
@Return:
	[OUT] None
@Reference
	Source:
@NOTE
*/
void CThreadTask::postExecute(void* pResult, void* pExtraVal) {
	//__LOGT__( TAG, "postExecute()" );

/*
#ifdef __REQ_MAIN_FUNCTION__
	int result = reinterpret_cast<int>(pResult);
	__LOGT__( TAG, "postExecute(): result = %s", (result? "TRUE" : "FALSE") );
#endif
*/
}
// ---------------------------------------------------------------



//! Implementation
// ---------------------------------------------------------------
// ---------------------------------------------------------------



/*
#ifdef __REQ_MAIN_FUNCTION__
//! Main
// ---------------------------------------------------------------
int main(void) {
	const char* TAG = "MAIN";
	__LOG_FMT__( "main()" );

	__LOGT__( TAG, "-----------CThreadTaskTask Test------------ [");
	CThreadTask th;
	//th.execute();
	th.execute( NULL, "%s %s %d %s", "test0", "test1", 1004, "test2" );
	__LOGT__( TAG, "-----------CThreadTaskTask Test------------ ]");

	return 0;
}
// ---------------------------------------------------------------
#endif //#ifdef __REQ_MAIN_FUNCTION__
*/
