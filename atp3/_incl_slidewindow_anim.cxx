/* --------------------------------------------------------------
Project:	ATP(A Tentative Plan) 3
Purpose:	
Author:		Ho-Jung Kim (godmode2k@hotmail.com)
Date:		Since Dec 2, 2014
Filename:	incl_slidewindow_anim.cxx

Last modified: Dec 2, 2015
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
 - DO NOT INCLUDE "incl_slidewindow_anim.hxx" here
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


// class Cxxx
CSlideWindow_Anim::CSlideWindow_Anim(void) : TAG("CSlideWindow_Anim") {
	__LOGT__( TAG, "CSlideWindow_Anim()" );
}

CSlideWindow_Anim::~CSlideWindow_Anim(void) {
	__LOGT__( TAG, "~CSlideWindow_Anim()" );
}

void CSlideWindow_Anim::task_in_thread(void* user_data) {
	//__LOGT__( TAG, "task_in_thread()" );

	if ( get_done() ) {
		__LOGT__( TAG, "task_in_thread(): DONE ALREADY\n" );
		goto _RELEASE;
	}


	{
		// Source object
		CViewSlideWindow* src_obj = (CViewSlideWindow*)get_src_obj();

		if ( !src_obj ) {
			__LOGT__( TAG, "task_in_thread(): CViewSlideWindow == NULL\n" );
			goto _RELEASE;
		}

		if ( !src_obj->get_top_view() ) {
			__LOGT__( TAG, "task_in_thread(): Top View == NULL\n" );
			goto _RELEASE;
		}

		// Position
		if ( !src_obj->get_opened() ) {
			// - fade-in
			src_obj->set_obj_rect( -(float)src_obj->get_width(), 0.f,
									(float)src_obj->get_width(), (float)src_obj->get_height() );
		}


		for ( int i = 0; i < SLIDE_ANIM_DURATION; i++ ) {
			//__LOGT__( TAG, "task_in_thread(): anim... [%2d/%2d]", i, SLIDE_ANIM_DURATION );


			// Interrupt
			if ( get_cancel_state() ) break;

			
			GdkRectangle rect = src_obj->get_rect();
			int latency = SLIDE_ANIM_LATENCY + (i * SLIDE_ANIM_LATENCY_INC);

			// Position
			//if ( !src_obj->get_opened() ) { rect.x += i * 2; }
			//else { rect.x -= i * 2; if ( i == (SLIDE_ANIM_DURATION - 1) ) rect.x -= 2; }
			{
				const int inc = ( rect.width / SLIDE_ANIM_DURATION );
				if ( !src_obj->get_opened() ) { rect.x += inc; }
				else { rect.x -= inc; if ( i == (SLIDE_ANIM_DURATION - 1) ) rect.x -= inc; }
			}
			src_obj->set_obj_rect( rect.x, rect.y, rect.width, rect.height );


			// Latency
			//__usleep( 40 + (i * 0.5f) );
			__usleep( latency );


			//! DO NOT USE 'src_obj->invalidate()' here AND DOESN'T NEED IT.
			// - SEE: atp3.cpp: atp3::init_ui_with_callback(): g_timeout_add()...
			//
			// Update
			//src_obj->invalidate();
		}

		set_done( true );
		src_obj->set_opened( !src_obj->get_opened() );
		//__LOGT__( TAG, "task_in_thread(): DONE [%s]\n", get_done()? "TRUE" : "FALSE" );
	}



_RELEASE:
	__usleep( SLIDE_ANIM_LATENCY_OUTER );
	stop();
}
// ---------------------------------------------------------------

