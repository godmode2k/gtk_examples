/* --------------------------------------------------------------
Project:	ATP(A Tentative Plan) 3
Purpose:	
Author:		Ho-Jung Kim (godmode2k@hotmail.com)
Date:		Since Dec 2, 2014
Filename:	incl_progress_dlg.cxx

Last modified: Jun 8, 2017
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
/*
// Mutex Lock
static GMutex* g_mutex_progress_dlg = NULL;
{
	//g_assert( g_mutex_progress_dlg == NULL );
	//g_mutex_progress_dlg = g_mutex_new();
	
	if ( g_mutex_progress_dlg == NULL )
		g_mutex_progress_dlg = g_mutex_new();

	if ( g_mutex_trylock(g_mutex_progress_dlg) )
		__LOGT__( TAG, "...(): mutex lock [TRUE]" );
	else {
		__LOGT__( TAG, "...(): mutex lock: already locked by another thread" );
		return;
	}
}

// Mutex Unlock
{
	if ( g_mutex_progress_dlg != NULL )
		__LOGT__( TAG, "...(): mutex unlock" );
		g_mutex_unlock( g_mutex_progress_dlg );
}


// GTK thread
//
// https://developer.gnome.org/gdk2/2.24/gdk2-Threads.html
// Example: gtk-thread.c
// Author: Erik Mouw
//
//
// NOTE: g_thread_init (NULL);
//
// gdk_threads_init(); // no "g_thread_init(NULL)" here
// gdk_threads_enter(); // Get GTK thread lock
// {
//     ... gtk api ...
// }
// gdk_threads_leave(); // Release GTK thread lock
*/
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

	// GTK thread
	gdk_threads_init();

	for ( pos = 0; pos <= total; pos++ ) {
		percent = set_progress( pos, total );

		//snprintf( buf_progress, sizeof(buf_progress), "%d%% (%d/%d), %f",
		//		percent, pos, total, get_progress_fraction() );
		snprintf( buf_progress, sizeof(buf_progress), "%d%% (%d/%d)", percent, pos, total );

		// GTK thread
		gdk_threads_enter(); // Get GTK thread lock
		{
			set_progress_msg( buf_progress );
		}
		gdk_threads_leave(); // Release GTK thread lock

		if ( get_cancel_state() ) break;

		__usleep( 100 );
	}



	__usleep( 1000 );
	stop();
}
// ---------------------------------------------------------------

