/* --------------------------------------------------------------
Project:	ATP(A Tentative Plan) 3
Purpose:	
Author:		Ho-Jung Kim (godmode2k@hotmail.com)
Date:		Since Dec 2, 2014
Filename:	CKeyEvent.cpp

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
	$ g++ ... -D__LINUX__ -D_REENTRANT -lpthread -lrt \
		`pkg-config --cflags --libs gtk+-2.0 libglade-2.0`
		or
		`pkg-config --cflags --libs gtk+-3.0 libglade-2.0`

TODO:
 -
-------------------------------------------------------------- */



//! Header
// ---------------------------------------------------------------
#include "CKeyEvent.h"
// ---------------------------------------------------------------



//! Definition
// ---------------------------------------------------------------
// ---------------------------------------------------------------



//! Prototype
// ---------------------------------------------------------------
// ---------------------------------------------------------------



//! Class
// ---------------------------------------------------------------
// Ctor
CKeyEvent::CKeyEvent(void) : TAG("CKeyEvent") {
	__LOGT__( TAG, "CKeyEvent()" );

	__init();
}

// Dtor
CKeyEvent::~CKeyEvent() {
	__LOGT__( TAG, "~CKeyEvent()" );

	__release();
}

// Initialize
void CKeyEvent::__init(void) {
	__LOGT__( TAG, "__init()" );

	m_event_type = e_eventType_UNKNOWN;

	m_key_int = -1;
	m_key_char = -1;
	m_key_pressed_down = false;
	m_key_ctrl = false;
	m_key_shift = false;

	m_mouse_lbtn_pressed = false;
	m_mouse_rbtn_pressed = false;
	m_mouse_released = false;
	m_mouse_wbtn_pressed = false;
	m_mouse_wheel_up = false;
	m_mouse_wheel_down = false;
	m_mouse_x = 0.f;
	m_mouse_y = 0.f;
}

// Release
void CKeyEvent::__release(void) {
	__LOGT__( TAG, "__release()" );

	//g_slice_free( Widgets_st, m_pWidgets );
}

#if 0
// Initialize Widget structure
bool CKeyEvent::init_widget_all(Widgets_st** pWidgets) {
	__LOGT__( TAG, "init_widget_all()" );

	(*pWidgets) = g_slice_new( Widgets_st );

	if ( pWidgets == NULL )
		return false;
	/*
	// ...
	if ( (*pWidgets)->pWindow == NULL )
		return false;

	(*pWidgets)->pWindow = NULL;
	*/

	return true;
}

// Initialize
bool CAtp3::init_ui_with_callback(void) {
	__LOGT__( TAG, "init_ui_with_callback()" );

	return true;
}
#endif

//void CKeyEvent::key(GdkEvent* event, bool pressed_down, GtkWidget* widget, CViewMain* view) {
void CKeyEvent::key(GdkEvent* event, bool pressed_down, GtkWidget* widget) {
	//__LOGT__( TAG, "key()" );

	if ( !event ) {
		__LOGT__( TAG, "key(): GdkEvent == NULL" );
		return;
	}

	m_event_type = e_eventType_KEY_INPUT;

	m_mouse_lbtn_pressed = false;
	m_mouse_rbtn_pressed = false;
	m_mouse_released = false;
	m_mouse_wbtn_pressed = false;
	m_mouse_wheel_up = false;
	m_mouse_wheel_down = false;
	m_mouse_x = 0.f;
	m_mouse_y = 0.f;

	m_key_int = (int)event->key.keyval;
	m_key_char = (char)event->key.keyval;
	m_key_pressed_down = pressed_down;
	m_key_ctrl = ( (event->key.state & GDK_CONTROL_MASK) != 0 );
	m_key_shift = ( (event->key.state & GDK_SHIFT_MASK) != 0 );

	/*
	if ( pressed_down )
		__LOGT__( TAG, "key(): pressed key = %s", event->key.string );
	else
		__LOGT__( TAG, "key(): released key = %s", event->key.string );
	*/
}

//void CKeyEvent::mouse(GdkEvent* event, GtkWidget* widget, CViewMain* view) {
void CKeyEvent::mouse(GdkEvent* event, GtkWidget* widget) {
	//__LOGT__( TAG, "mouse()" );
	
	if ( !event ) {
		__LOGT__( TAG, "mouse(): GdkEvent == NULL" );
		return;
	}

	m_event_type = e_eventType_MOUSE_INPUT;

	m_key_int = -1;
	m_key_char = -1;
	m_key_pressed_down = false;
	m_key_ctrl = false;
	m_key_shift = false;

	m_mouse_lbtn_pressed = false;
	m_mouse_rbtn_pressed = false;
	m_mouse_released = false;
	m_mouse_wbtn_pressed = false;
	m_mouse_wheel_up = false;
	m_mouse_wheel_down = false;
	m_mouse_x = 0.f;
	m_mouse_y = 0.f;

	{
		bool flag_LButton = false;
		bool flag_RButton = false;
		bool flag_WButton = false;
		bool flag_Release = false;
		bool flag_WheelUp = false;
		bool flag_WheelDown = false;
		//GdkEvent* event; event->motion.state
		//GdkModifierType state = event->motion.state;
		GdkModifierType state;
		int x, y;


		// To reduce the number of GDK_MOTION_NOTIFY events received.
		if ( event->motion.is_hint ) {
			gdk_window_get_pointer( event->motion.window, &x, &y, &state );
		}
		else {
			return;
			//x = event->motion.x;
			//y = event->motion.y;
			//state = (GdkModifierType)event->motion.state;
		}

		flag_LButton = ( (state & GDK_BUTTON1_MASK) != 0 );
		flag_RButton = ( (state & GDK_BUTTON3_MASK) != 0 );
		flag_WButton = ( (state & GDK_BUTTON2_MASK) != 0 );
		// GDK_RELEASE_MASK is reserved by XKB
		// so we use the "event->type" here
		//flag_Release = ( (state & GDK_RELEASE_MASK) != 0 );
		if ( event->type == GDK_BUTTON_RELEASE )
			flag_Release = true;

		if ( flag_LButton ) {
			// mouse moving while pressed left button
			/*
			{
				// irr::EMIE_MOUSE_MOVED, irr::EMIE_LMOUSE_PRESSED_DOWN
				//irrEvent.MouseInput.Event = ( irr::EMIE_MOUSE_MOVED | irr::EMIE_LMOUSE_PRESSED_DOWN );	// ???
			}
			*/

			//irrEvent.MouseInput.Event = irr::EMIE_LMOUSE_PRESSED_DOWN;

			//__LOGT__( TAG, "mouse(): LBUTTON_PRESSED: x = %d, y = %d", x, y );
		}
		else if ( flag_RButton ) {
			//irrEvent.MouseInput.Event = irr::EMIE_RMOUSE_PRESSED_DOWN;

			//__LOGT__( TAG, "mouse(): RBUTTON_PRESSED: x = %d, y = %d", x, y );
		}
		else if ( flag_WButton ) {
			//irrEvent.MouseInput.Event = irr::EMIE_MOUSE_WHEEL;

			//__LOGT__( TAG, "mouse(): WBUTTON_PRESSED: x = %d, y = %d", x, y );
		}
		else if ( flag_Release ) {
			// LEFT, RIGHT UP
			//irrEvent.MouseInput.Event = irr::EMIE_LMOUSE_LEFT_UP;
			//irrEvent.MouseInput.Event = irr::EMIE_RMOUSE_LEFT_UP;

			//__LOGT__( TAG, "mouse(): BUTTON_RELEASED: x = %d, y = %d", x, y );
		}
		else {
			//irrEvent.MouseInput.Event = irr::EMIE_MOUSE_MOVED;

			//__LOGT__( TAG, "mouse(): MOVED: x = %d, y = %d", x, y );
		}

		//__LOGT__( TAG__g_FuncSignalHandler, "mouse(): x = %d, y = %d",
		//		event->motion.x, event->motion.y );


		m_mouse_lbtn_pressed = flag_LButton;
		m_mouse_rbtn_pressed = flag_RButton;
		m_mouse_released = flag_Release;
		m_mouse_wbtn_pressed = flag_WButton;
		m_mouse_wheel_up = flag_WheelUp;
		m_mouse_wheel_down = flag_WheelDown;
		m_mouse_x = x;
		m_mouse_y = y;
	}


	//if ( view ) {
	{
		if ( m_mouse_lbtn_pressed || m_mouse_rbtn_pressed || m_mouse_wbtn_pressed ) {
			if ( event->type == GDK_MOTION_NOTIFY ) {
				m_event_action = e_eventAction_mouse_move;
				//view->onTouchEventMove( m_mouse_x, m_mouse_y );
			}
			else {
				m_event_action = e_eventAction_mouse_pressed;
				//view->onTouchEventDown( m_mouse_x, m_mouse_y );
			}
		}
		else if ( m_mouse_released ) {
			m_event_action = e_eventAction_mouse_released;
			//view->onTouchEventUp( m_mouse_x, m_mouse_y );
		}
		else {
			m_event_action = e_eventAction_mouse_move;
			//view->onTouchEventMove( m_mouse_x, m_mouse_y );
		}

		//view->onTouchEvent( this );
	}
}
// ---------------------------------------------------------------



//! Implementation
// ---------------------------------------------------------------
// Global variable

namespace g_Func {
} // namespace g_Func

namespace g_FuncSignalHandler {
} // namespace g_FuncSignalHandler


#if 0
// class CASyncTask [
CAsyncTask::CAsyncTask(void) : TAG("CAsyncTask") {
	__LOGT__( TAG, "CAsyncTask()" );

	m_pSpinner = NULL;
}

CAsyncTask::~CAsyncTask(void) {
	__LOGT__( TAG, "~CAsyncTask()" );
}

void* CAsyncTask::inBackground(std::vector<void*>* pvecVal) {
	__LOGT__( TAG, "inBackground()" );

	// TEST
	/*
	for ( int i = 0; i < 10; i++ ) {
		__LOGT__( TAG, "inBackground(): i = %d", i );
		update( (void*)"#---update---#" );
	}
	__LOGT__( TAG, "" );

	if ( pvecVal && (pvecVal->size() > 0) ) {
		std::vector<void*>::iterator iter;
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
	*/



	return ((void*)true);
}

void CAsyncTask::progressUpdate(void* pVal) {
	__LOGT__( TAG, "progressUpdate()" );

	char* pStr = reinterpret_cast<char*>(pVal);
	if ( pStr )
		__LOGT__( TAG, "%s", pStr );
}

void CAsyncTask::postExecute(void* pResult, void* pExtraVal) {
	__LOGT__( TAG, "postExecute()" );

	int result = reinterpret_cast<int>(pResult);
	__LOGT__( TAG, "postExecute(): result = %s", (result? "TRUE" : "FALSE") );

	if ( pExtraVal ) {
		GtkWidget* pDlg = NULL;
		pDlg = GTK_WIDGET( pExtraVal );
		
		//! DO NOT USE THE FOLLOWING
		/*
		//GtkSpinner* pSpinner = ((Widgets_st*)pExtraVal)->pSpinner;
		if ( m_pSpinner ) {
			__LOGT__( TAG, "postExecute(): Stop Spinner" );
			gtk_spinner_stop( m_pSpinner );
			m_pSpinner = NULL;
		}

		if ( pDlg ) {
			__LOGT__( TAG, "postExecute(): Destroy Dialog" );
			gtk_widget_hide( pDlg );
			gtk_widget_destroy( pDlg );
			pDlg = NULL;
		}
		*/

		if ( pDlg ) {
			__LOGT__( TAG, "postExecute(): Send a signal to Destroy Dialog" );
			gtk_dialog_response( GTK_DIALOG(pDlg), GTK_RESPONSE_NONE );
		}
	}
}
// class CASyncTask ]
#endif
// ---------------------------------------------------------------



//! Main
// ---------------------------------------------------------------
#if 0
int main(int argc, char* argv[]) {
	gtk_init( &argc, &argv );


	/*
	CAtp3 atp3;
	if ( !atp2.init_ui_with_callback() )
		return -1;



	//! Release the glade builder
	// ---------------------------------------------------------------
	g_object_unref( G_OBJECT(atp2.get_gtk_builder()) );



	//! Rendering
	// ---------------------------------------------------------------
	gtk_window_set_title( GTK_WINDOW(atp2.get_widget_st()->pWindow), "Atp2: Hello~" );
	//gtk_widget_show_all( atp2.get_widget_st()->pWindow );                
	gtk_widget_show( atp2.get_widget_st()->pWindow );                
	*/



	g_pCAtp3 = new CAtp3();
	if ( !g_pCAtp3 )
		return -1;
	if ( !g_pCAtp3->init_ui_with_callback() )
		return -1;

	//! Release the glade builder
	// ---------------------------------------------------------------
	g_object_unref( G_OBJECT(g_pCAtp3->get_gtk_builder()) );



	//! Rendering
	// ---------------------------------------------------------------
	gtk_window_set_title( GTK_WINDOW(g_pCAtp3->get_widget_st()->pWindow), "Atp3: Hello~" );
	gtk_widget_show( g_pCAtp3->get_widget_st()->pWindow );                



	//! Start Application
	// ---------------------------------------------------------------
	gtk_main();



	//! Release
	// ---------------------------------------------------------------
	//g_slice_free( Widgets_st, pWidgets );

	return 0;
}
#endif



