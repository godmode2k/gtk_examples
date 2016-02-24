#ifndef __INCL_TIMER_HANDLER_HXX__
#define __INCL_TIMER_HANDLER_HXX__

/* --------------------------------------------------------------
Project:	ATP(A Tentative Plan) 3
Purpose:	
Author:		Ho-Jung Kim (godmode2k@hotmail.com)
Date:		Since Dec 2, 2014
Filename:	incl_timer_handler.h

Last modified: Jun 2, 2015
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
	$ g++ ... -D__LINUX__ -D_REENTRANT -lpthread -lrt \
		`pkg-config --cflags --libs gtk+-2.0 libglade-2.0`
		or
		`pkg-config --cflags --libs gtk+-3.0 libglade-2.0`
-------------------------------------------------------------- */



//! Header and Definition
// ---------------------------------------------------------------
#include "util/CTimerHandlerTask.h"
// ---------------------------------------------------------------



//! Definition
// ---------------------------------------------------------------
// Latency
#define TIMER_HANDLER_LATENCY_OUTER			10	// millisecond

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif
// ---------------------------------------------------------------



//! Prototype
// ---------------------------------------------------------------
// ---------------------------------------------------------------



//! Class
// ---------------------------------------------------------------
// "incl_timer_handler.cxx"
class CTimerHandler : virtual public CTimerHandlerTask {
private:
	const char* TAG;

	//bool m_update;
protected:
public:
	explicit CTimerHandler(void);
	~CTimerHandler();

	void __init(void);
	void __release(void);


	// ---------------------------------------------------------------
	
	//void set_update(bool val) { m_update = val; }
	//bool get_update(void) { return m_update; }

	// thread
	void task_in_thread(void* user_data = NULL);
};
// ---------------------------------------------------------------



#endif	// __INCL_TIMER_HANDLER_HXX__
