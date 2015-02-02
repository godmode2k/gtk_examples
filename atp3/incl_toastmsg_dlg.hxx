#ifndef __INCL_TOASTMSG_DLG_HXX__
#define __INCL_TOASTMSG_DLG_HXX__

/* --------------------------------------------------------------
Project:	ATP(A Tentative Plan) 3
Purpose:	
Author:		Ho-Jung Kim (godmode2k@hotmail.com)
Date:		Since Dec 2, 2014
Filename:	incl_toastmsg_dlg.h

Last modified: Jan 4, 2015
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
// GTK Dialog
#include "util/CToastMsgDlg.h"
// ---------------------------------------------------------------



//! Definition
// ---------------------------------------------------------------
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
// "incl_toastmsg_dlg.cxx"
class CToastMsg_dlg : public CToastMsgDlg {
private:
	const char* TAG;
protected:
public:
	explicit CToastMsg_dlg(void);
	~CToastMsg_dlg();

	// ---------------------------------------------------------------
	
	// thread
	//void task_in_thread(void);
};
// ---------------------------------------------------------------



#endif	// __INCL_TOASTMSG_DLG_HXX__
