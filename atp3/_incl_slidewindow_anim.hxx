#ifndef __INCL_SLIDEWINDOW_ANIM_HXX__
#define __INCL_SLIDEWINDOW_ANIM_HXX__

/* --------------------------------------------------------------
Project:	ATP(A Tentative Plan) 3
Purpose:	
Author:		Ho-Jung Kim (godmode2k@hotmail.com)
Date:		Since Dec 2, 2014
Filename:	incl_slidewindow_anim.h

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
#include "util/CAnimTask.h"
// ---------------------------------------------------------------



//! Definition
// ---------------------------------------------------------------
// Latency
//  - int latency = SLIDE_ANIM_LATENCY + (i * SLIDE_ANIM_LATENCY_INC);
#define SLIDE_ANIM_DURATION					15
#define SLIDE_ANIM_LATENCY					40	// millisecond
#define SLIDE_ANIM_LATENCY_OUTER			10	// millisecond
#define SLIDE_ANIM_LATENCY_INC				0.5f

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
// "incl_slidewindow_anim.cxx"
class CSlideWindow_Anim : virtual public CAnimTask {
private:
	const char* TAG;

	// Source object
	CViewSlideWindow* m_src;
protected:
public:
	explicit CSlideWindow_Anim(void);
	~CSlideWindow_Anim();

	// ---------------------------------------------------------------
	
	// thread
	void task_in_thread(void* user_data = NULL);

	// Source object
	void set_src_obj(CViewSlideWindow* src) { m_src = src; }
	CViewSlideWindow* get_src_obj(void) { return m_src; }
};
// ---------------------------------------------------------------



#endif	// __INCL_SLIDEWINDOW_ANIM_HXX__
