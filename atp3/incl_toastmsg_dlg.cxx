/* --------------------------------------------------------------
Project:	ATP(A Tentative Plan) 3
Purpose:	
Author:		Ho-Jung Kim (godmode2k@hotmail.com)
Date:		Since Dec 2, 2014
Filename:	incl_toastmsg_dlg.cxx

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
 - DO NOT INCLUDE "incl_toastmsg_dlg.hxx" here
-----------------------------------------------------------------
1. Build:
	$ g++ ... -D__LINUX__ -D_REENTRANT -lpthread -lrt \
		`pkg-config --cflags --libs gtk+-2.0 libglade-2.0`
		or
		`pkg-config --cflags --libs gtk+-3.0 libglade-2.0`

TODO:
 -
-------------------------------------------------------------- */



//! Header
// ---------------------------------------------------------------
// ---------------------------------------------------------------



//! Definition
// ---------------------------------------------------------------
// ---------------------------------------------------------------



//! Prototype
// ---------------------------------------------------------------
// ---------------------------------------------------------------



//! Class
// ---------------------------------------------------------------
// ---------------------------------------------------------------



//! Implementation
// ---------------------------------------------------------------


// class CToastMsg_dlg
CToastMsg_dlg::CToastMsg_dlg(void) : TAG("CToastMsg_dlg") {
	__LOGT__( TAG, "CToastMsg_dlg()" );
}

CToastMsg_dlg::~CToastMsg_dlg(void) {
	__LOGT__( TAG, "~CToastMsg_dlg()" );
}

/*
void CToastMsg_dlg::task_in_thread(void) {
	__LOGT__( TAG, "task_in_thread()" );

	__usleep( 2000 );
	stop();
}
*/
// ---------------------------------------------------------------

