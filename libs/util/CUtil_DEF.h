#ifndef __CUTIL_DEF_H__
#define __CUTIL_DEF_H__

/* --------------------------------------------------------------
Project:	
Purpose:	
Author:		Ho-Jung Kim (godmode2k@hotmail.com)
Date:		Since June 26, 2014
Filename: 	CUtil_DEF.h

Last modified: July 22, 2014
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
#ifdef __ANDROID_NDK__
	#include <string.h>
	#include <jni.h>

	#include "../log/android_log.h"
#endif
// ---------------------------------------------------------------



//! Definition
// ---------------------------------------------------------------
// LOG
#ifdef __DEBUG_MODE__
	#define __REQ_DEBUG_MESSAGE__
#endif

/*
// SEE: CUtil.h
#ifdef __ANDROID_NDK__
	#undef __LOGT__(TAG, ...)
	#define __LOGT__(TAG, ...) LOGD(__VA_ARGS__)
#endif
*/
// ---------------------------------------------------------------



#endif	// __CUTIL_DEF_H__
