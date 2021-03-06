/* --------------------------------------------------------------
Project:	CToastMsgDlg
Purpose:	Progress Dialog
Author:		Ho-Jung Kim (godmode2k@hotmail.com)
Date:		Since Nov 28, 2014
Filename:	CToastMsgDlg.cpp

Last modified: May 28, 2015
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



//! Header
// ---------------------------------------------------------------
#include "CToastMsgDlg.h"
// ---------------------------------------------------------------



//! Definition
// ---------------------------------------------------------------
// ---------------------------------------------------------------



//! Prototype
// ---------------------------------------------------------------
// ---------------------------------------------------------------



//! Class
// ---------------------------------------------------------------
/*
// Ctor
CEventData::CEventData(void) {
}
// Dtor
CEventData::~CEventData() {
}
*/

// Ctor
CToastMsgDlg::CToastMsgDlg(void) : TAG("CToastMsgDlg") {
	__LOGT__( TAG, "CToastMsgDlg()" );

	__init();
}

// Dtor
CToastMsgDlg::~CToastMsgDlg() {
	__LOGT__( TAG, "~CToastMsgDlg()" );

	__release();
}

// Initialize
void CToastMsgDlg::__init(void) {
	__LOGT__( TAG, "__init()" );

	//m_pError = NULL;

	m_msg = NULL;
	m_delay = e_toastMsgShowDelay_NONE;

	m_pMessageDlg = NULL;
	m_working = false;
}

// Release
void CToastMsgDlg::__release(void) {
	__LOGT__( TAG, "__release()" );


	{
		if ( m_pLabel ) {
			gtk_widget_hide( m_pLabel );
			gtk_widget_destroy( m_pLabel );
			m_pLabel = NULL;
		}

		if ( m_pMessageDlg ) {
			gtk_widget_hide( m_pMessageDlg );
			gtk_widget_destroy( m_pMessageDlg );
			m_pMessageDlg = NULL;
		}
	}
}
// ---------------------------------------------------------------



//! Implementation
// ---------------------------------------------------------------
#if 0
namespace g_Func {
	// Callback
	/*
	void main_window_quit(GtkWidget* widget, gpointer user_data) {
		__LOGT__( TAG__g_Func, "main_window_quit()" );
	}
	*/
} // namespace g_Func

namespace g_FuncSignalHandler {
	// MenuItem: Button
	/*
	void on_button_xxxx_clicked(GtkWidget* widget, gpointer user_data) {
		__LOGT__( TAG__g_FuncSignalHandler, "on_button_open_clicked()" );

		GtkWidget* dlg;
		dlg = gtk_file_chooser_dialog_new( "Open File",
				//(GtkWindow*)widget,

				// Parent Window
				// Gtk-CRITICAL: IA__gtk_window_set_transient_for
				(GtkWindow*)((Widgets_st*)user_data)->pWindow,

				GTK_FILE_CHOOSER_ACTION_OPEN,
				GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
				GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
				NULL );

		if ( gtk_dialog_run(GTK_DIALOG(dlg) ) == GTK_RESPONSE_ACCEPT ) {
			char* filename;

			filename = gtk_file_chooser_get_filename( GTK_FILE_CHOOSER(dlg) );
			//open_file( filename );

			// Display Image
			//__LOGT__( TAG__g_FuncSignalHandler, "on_button_open_clicked(): filename = %s\n", filename );
			//g_pEvtData->setImgSrcFilename( filename );
			//default_display( widget, NULL );

			g_free( filename );
		}

		gtk_widget_destroy( dlg );
	}
	*/
} // namespace g_FuncSignalHandler
#endif


// ---------------------------------------------------------------

void CToastMsgDlg::updates(void) {
	//__LOGT__( TAG, "updates()" );
}

void CToastMsgDlg::wait_for_thread_finished(void) {
	__LOGT__( TAG, "wait_for_thread_finished()" );

	while ( get_working() ) {
		__usleep( 100 );
	}

	__LOGT__( TAG, "wait_for_thread_finished(): finished" );
}

void CToastMsgDlg::set_delay(e_ToastMsgShowDelay_t delay) {
	//__LOGT__( TAG, "set_delay()" );

	m_delay = (delay > e_toastMsgShowDelay_LONG)? e_toastMsgShowDelay_SHORT : delay;
}

bool CToastMsgDlg::show(GtkWindow* pWindow, gpointer user_data,
		const char* msg, e_ToastMsgShowDelay_t delay,
		e_ToastMsgShowPos_t pos) {
	//__LOGT__( TAG, "show()" );

	if ( get_working() ) {
		//__LOGT__( TAG, "show(): working... SKIP" );
		return false;
	}

	if ( !pWindow ) {
		__LOGT__( TAG, "show(): GtkWindow == NULL" );
		return false;
	}

	//if ( !user_data ) {
	//	__LOGT__( TAG, "show(): user_data == NULL" );
	//	return false;
	//}


	if ( !msg && !get_message() ) {
		__LOGT__( TAG, "show(): Message == NULL" );
		return false;
	}
	else if ( msg ) {
		set_message( msg );
	}
	else if ( !get_message() ) {
		__LOGT__( TAG, "show(): Message == NULL" );
		return false;
	}

	if ( get_delay() == e_toastMsgShowDelay_NONE )
		set_delay( delay );


	__LOGT__( TAG, "show(): Message = %s, delay = %d(ms)", get_message(), get_delay() );


	destroy_ui();

	/*
	//! Do not use gtk_message_dialog_new() due to word-wrap.
	m_pMessageDlg = gtk_message_dialog_new(
						//(GtkWindow*)((Widgets_st*)user_data)->pWindow,
						pWindow,
						GTK_DIALOG_DESTROY_WITH_PARENT,		// GtkDialogFlags
						GTK_MESSAGE_OTHER,
						GTK_BUTTONS_NONE,
						"%s", get_message() );				// NULL if used markup
	*/
	m_pMessageDlg = gtk_dialog_new();


	if ( m_pMessageDlg ) {
		// Markup
		/*
		//const char* markup = "<b> bold test... </b>";
		//gtk_message_dialog_set_markup( (GtkMessageDialog*)m_pMessageDlg, markup );
		//
		// or
		//
		char* markup = NULL;
		markup = g_markup_printf_escaped( "<b> %s </b>", get_message() );
		if ( markup ) {
			gtk_message_dialog_set_markup( (GtkMessageDialog*)m_pMessageDlg, markup );
			g_free( markup );
			markup = NULL;
		}
		*/

		// Secondary Text
		//gtk_message_dialog_format_secondary_text( (GtkMessageDialog*)m_pMessageDlg, "test..." );



		// Label
		{
			m_pLabel = gtk_label_new( get_message() );

			if ( !m_pLabel ) {
				__LOGT__( TAG, "show(): Label == NULL" );
				destroy_ui();

				return false;
			}

			gtk_label_set_justify( GTK_LABEL(m_pLabel), GTK_JUSTIFY_CENTER );
			gtk_container_add( GTK_CONTAINER(gtk_dialog_get_content_area(
								GTK_DIALOG(m_pMessageDlg))),
								m_pLabel );
		}


		// Show
		{

			gtk_window_set_resizable( GTK_WINDOW(m_pMessageDlg), false );
			gtk_window_set_transient_for( GTK_WINDOW(m_pMessageDlg), pWindow );
			gtk_window_set_position( GTK_WINDOW(m_pMessageDlg), GTK_WIN_POS_CENTER_ON_PARENT );
			gtk_window_set_decorated( GTK_WINDOW(m_pMessageDlg), false );

			// partially transparent: [0 ~ 1] opaque 0 being fully transparent, 1 fully opaque.
			// - NOTE: {
			//     on Ubuntu 11.10 Unity 2D, doesn't show anything in first time when have used
			//     gtk_window_set_opacity(), but since then it will works good.
			//     If haven't used it, doesn't matter.
			// }
			gtk_window_set_opacity( GTK_WINDOW(m_pMessageDlg), DEFAULT_TOAST_WINDOW_OPAQUE );

			gtk_widget_show_all( m_pMessageDlg );


			// Position
			{
				GtkAllocation windowSize;
				GtkAllocation msgWindowSize;
				int msgWindow_x = 0, msgWindow_y = 0;

				gtk_widget_get_allocation( GTK_WIDGET(pWindow), &windowSize );
				gtk_widget_get_allocation( GTK_WIDGET(m_pMessageDlg), &msgWindowSize );

				//__LOGT__( TAG, "show(): main window x = %d, y = %d, w = %d, h = %d",
				//			windowSize.x, windowSize.y, windowSize.width, windowSize.height );
				//__LOGT__( TAG, "show(): toast window x = %d, y = %d, w = %d, h = %d, w = %d, h = %d",
				//			msgWindowSize.x, msgWindowSize.y, msgWindowSize.width, msgWindowSize.height,
				//			m_pMessageDlg->allocation.width, m_pMessageDlg->allocation.height );

				//! NOTE: Top Window on the Application (NOT toplevel Window)
#ifdef __GTKv2__
				gdk_window_get_origin( GTK_WIDGET(pWindow)->window, &msgWindow_x, &msgWindow_y );
#elif __GTKv3__
				gdk_window_get_origin( gtk_widget_get_window(GTK_WIDGET(pWindow)), &msgWindow_x, &msgWindow_y );
#else
#endif
				//__LOGT__( TAG, "show(): new toast window x = %d, y = %d", msgWindow_x, msgWindow_y );

				msgWindow_x += DEFAULT_TOAST_WINDOW_WIDTH_CENTER_POS( windowSize.width, msgWindowSize.width );
				if ( pos == e_toastMsgShowPos_TOP ) {
					// None
				}
				else if ( pos == e_toastMsgShowPos_BOTTOM ) {
					msgWindow_y += DEFAULT_TOAST_WINDOW_BOTTOM_POS( windowSize.height, msgWindowSize.height );
				}
				else {
					// e_toastMsgShowPos_CENTER
					// ...

					msgWindow_y += DEFAULT_TOAST_WINDOW_HEIGHT_CENTER_POS( windowSize.height, msgWindowSize.height );
				}

				//__LOGT__( TAG, "show(): new toast window x = %d, y = %d, w = %d, h = %d",
				//			msgWindow_x, msgWindow_y, msgWindowSize.width, msgWindowSize.height );

				//! DO NOT USE THOSE
				//gtk_widget_set_allocation( GTK_WIDGET(m_pMessageDlg), (GtkAllocation*)&msgWindowSize );
				//gtk_widget_size_allocate( GTK_WIDGET(m_pMessageDlg), (GtkAllocation*)&msgWindowSize );

				// Move to new position
				gtk_window_move( GTK_WINDOW(m_pMessageDlg), msgWindow_x, msgWindow_y );
			}
		}


		/*
		// Show
		{
			//! for gtk_message_dialog_new()

			gtk_window_set_resizable( GTK_WINDOW(m_pMessageDlg), false );
			//gtk_widget_set_size_request( m_pMessageDlg, 220, 20 );
			//gtk_widget_set_allocation( GTK_WIDGET(m_pMessageDlg), &msgWindowSize );

			gtk_window_set_decorated( GTK_WINDOW(m_pMessageDlg), false );
			gtk_widget_show( m_pMessageDlg );
		}
		*/



		//
		// run the Thread
		//
		
		set_working( true );

		if ( !this->execute(m_pMessageDlg) ) {
			__LOGT__( TAG, "show(): execute [FAIL]" );

			destroy_ui();

			set_working( false );
			set_delay( e_toastMsgShowDelay_NONE );

			return false;
		}


		/*
		{
			gint ret_dlg_run = GTK_RESPONSE_NONE;


			__LOGT__( TAG, "show(): Run a Dialog" );

			// Will blocking the following
			// for catch an interrupts
			//  - send a signal: gtk_dialog_response( GTK_DIALOG(__DIALOG__), GTK_RESPONSE_NONE );
			ret_dlg_run = gtk_dialog_run( GTK_DIALOG(m_pMessageDlg) );
			switch ( ret_dlg_run ) {
				case GTK_RESPONSE_CANCEL:
					{
						__LOGT__( TAG, "show(): Cancel Button clicked" );
						this->cancel();
					} break;
				case GTK_RESPONSE_NONE:
					{
						__LOGT__( TAG, "show(): Destroyed" );
					} break;
				default:
					{
						__LOGT__( TAG, "show(): Destroyed: Response code = %d", ret_dlg_run );
						this->cancel();
					} break;
			}
		}
		*/


		// wait for cancelled thread task
		// SEE: CThreadTask::cancel(void);
	}

/*
	set_working( false );



	// Destroy
	{
		if ( m_pMessageDlg ) {
			gtk_widget_hide( m_pMessageDlg );
			gtk_widget_destroy( m_pMessageDlg );
			m_pMessageDlg = NULL;
		}

		set_message( NULL );
	}
*/



	return true;
}

void CToastMsgDlg::task_in_thread(void) {
	__LOGT__( TAG, "task_in_thread()" );

	__usleep( get_delay() );
	set_working( false );
}

void CToastMsgDlg::destroy_ui(void) {
	__LOGT__( TAG, "destroy_ui()" );

	if ( m_pLabel ) {
		gtk_widget_hide( m_pLabel );
		gtk_widget_destroy( m_pLabel );
		m_pLabel = NULL;
	}

	if ( m_pMessageDlg ) {
		gtk_widget_hide( m_pMessageDlg );
		gtk_widget_destroy( m_pMessageDlg );
		m_pMessageDlg = NULL;
	}
}

void CToastMsgDlg::stop(void) {
	__LOGT__( TAG, "stop()" );

	set_working( false );
}

void CToastMsgDlg::__stop(void) {
	__LOGT__( TAG, "__stop()" );

	set_working( false );


	if ( m_pLabel ) {
		gtk_widget_hide( m_pLabel );
		gtk_widget_destroy( m_pLabel );
		m_pLabel = NULL;
	}

	if ( m_pMessageDlg ) {
		__LOGT__( TAG, "__stop(): Send a signal to Destroy Dialog" );
		__LOGT__( TAG, "__stop(): SKIP this..." );

		// for gtk_dialog_run()
		//gtk_dialog_response( GTK_DIALOG(m_pMessageDlg), GTK_RESPONSE_NONE );


		{
			gtk_widget_hide( m_pMessageDlg );
			gtk_widget_destroy( m_pMessageDlg );
			m_pMessageDlg = NULL;
		}
	}


	set_message( NULL );
	set_delay( e_toastMsgShowDelay_NONE );


	// wait for the thread finished
	wait_for_thread_finished();
}

// ---------------------------------------------------------------


void* CToastMsgDlg::inBackground(std::vector<void*>* pvecVal) {
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


	do {
		if ( get_updated() ) {
			// updates message
			updates();
			set_updated( false );
		}

		//__LOGT__( TAG, "inBackground(): get_working() = %s", (get_working()? "TRUE" : "FALSE") );
		//__LOGT__( TAG, "inBackground(): get_cancel_state() = %s", (this->get_cancel_state()? "TRUE" : "FALSE") );


		// perform the task
		task_in_thread();


		__usleep( 100 );
	} while ( get_working() && !this->get_cancel_state() );


	return ((void*)true);
}

void CToastMsgDlg::progressUpdate(void* pVal) {
	__LOGT__( TAG, "progressUpdate()" );

	char* pStr = reinterpret_cast<char*>(pVal);

	if ( pStr ) {
		set_updated();
		__LOGT__( TAG, "%s", pStr );
		m_msg = pStr;
	}
}

void CToastMsgDlg::postExecute(void* pResult, void* pExtraVal) {
	__LOGT__( TAG, "postExecute()" );

	//int result = reinterpret_cast<int>(pResult);
	int result = static_cast<int>(reinterpret_cast<long>(pResult));
	__LOGT__( TAG, "postExecute(): result = %s", (result? "TRUE" : "FALSE") );


	__stop();
}
// class CASyncTask ]
// ---------------------------------------------------------------


