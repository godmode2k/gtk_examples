/* --------------------------------------------------------------
Project:	ATP(A Tentative Plan) 3
Purpose:	
Author:		Ho-Jung Kim (godmode2k@hotmail.com)
Date:		Since Dec 2, 2014
Filename:	incl_timer_handler.cxx

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
 - DO NOT INCLUDE "incl_timer_handler.hxx" here
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
CTimerHandler::CTimerHandler(void) : TAG("CTimerHandler") {
	//__LOGT__( TAG, "CTimerHandler()" );
}

CTimerHandler::~CTimerHandler(void) {
	//__LOGT__( TAG, "~CTimerHandler()" );
}

void CTimerHandler::__init(void) {
	//__LOGT__( TAG, "__init()" );
	
	//m_update = false;
}

void CTimerHandler::__release(void) {
	//__LOGT__( TAG, "__release()" );
}

void CTimerHandler::task_in_thread(void* user_data) {
	//__LOGT__( TAG, "task_in_thread()" );


	goto _RELEASE;

/*
	{
		CAtp3* main = NULL;
		CViewMain* view = NULL;
		

		main = (CAtp3*)user_data;
		if ( !main ) {
			__LOGT__( TAG, "task_in_thread(): Main object == NULL" );
			goto _RELEASE;
		}
		//__LOGT__( TAG, "task_in_thread(): Main object = %p, TAG = %s", main, main->get_tag() );

		view = main->get_view_main();
		if ( !view ) {
			__LOGT__( TAG, "task_in_thread(): MainView object == NULL" );
			goto _RELEASE;
		}
		//__LOGT__( TAG, "task_in_thread(): MainView object = %p, TAG = %s", view, view->get_tag() );


		while ( 1 ) {
			//__LOGT__( TAG, "task_in_thread(): waiting..." );

			// Interrupt
			if ( get_cancel_state() ) break;


			// Update
			if ( view->get_attach_animstask_update() ) {
				//__LOGT__( TAG, "task_in_thread(): redraw..." );
				//view->invalidate();


				Widgets_st* widget = main->get_widget_st();

				if ( !widget ) {
					__LOGT__( TAG, "task_in_thread(): Widget object == NULL" );
					break;
				}
				if ( !widget->pDrawingArea ) {
					__LOGT__( TAG, "task_in_thread(): DrawingArea object == NULL" );
					break;
				}

				if ( !GTK_IS_WIDGET(widget->pDrawingArea) )
					__LOGT__( TAG, "task_in_thread(): DrawingArea == widget [FALSE]" );


				print_datetime_d();
				gtk_widget_queue_draw( widget->pDrawingArea );
			}


			// Latency
			__usleep( TIMER_HANDLER_LATENCY );
		}
	}
*/



_RELEASE:
	__usleep( TIMER_HANDLER_LATENCY_OUTER );
	stop();
}
// ---------------------------------------------------------------

