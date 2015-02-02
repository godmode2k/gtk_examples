/* --------------------------------------------------------------
Project:	ATP(A Tentative Plan) 3
Purpose:	
Author:		Ho-Jung Kim (godmode2k@hotmail.com)
Date:		Since Dec 2, 2014
Filename:	incl_progress_dlg.cxx

Last modified: Jan 20, 2015
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
 - DO NOT INCLUDE "incl_progress_dlg.hxx" here
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


// class CProgress_dlg
CProgress_dlg::CProgress_dlg(void) : TAG("CProgress_dlg") {
	__LOGT__( TAG, "CProgress_dlg()" );
}

CProgress_dlg::~CProgress_dlg(void) {
	__LOGT__( TAG, "~CProgress_dlg()" );
}

void CProgress_dlg::task_in_thread(void) {
	__LOGT__( TAG, "task_in_thread()" );

	char buf_progress[255] = {0,};
	const int total = 50;
	int pos = 0;
	int percent = 0;

	for ( pos = 0; pos <= total; pos++ ) {
		percent = set_progress( pos, total );

		//snprintf( buf_progress, sizeof(buf_progress), "%d%% (%d/%d), %f",
		//		percent, pos, total, get_progress_fraction() );
		snprintf( buf_progress, sizeof(buf_progress), "%d%% (%d/%d)", percent, pos, total );
		set_progress_msg( buf_progress );

		if ( get_cancel_state() ) break;

		__usleep( 100 );
	}



	__usleep( 1000 );
	stop();
}
// ---------------------------------------------------------------

