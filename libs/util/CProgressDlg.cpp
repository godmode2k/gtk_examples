/* --------------------------------------------------------------
Project:	CProgressDlg
Purpose:	Progress Dialog
Author:		Ho-Jung Kim (godmode2k@hotmail.com)
Date:		Since Nov 28, 2014
Filename:	CProgressDlg.cpp

Last modified: Feb 3, 2015
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
#include "CProgressDlg.h"
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
CProgressDlg::CProgressDlg(void) : TAG("CProgressDlg") {
	__LOGT__( TAG, "CProgressDlg()" );

	__init();
}

// Dtor
CProgressDlg::~CProgressDlg() {
	__LOGT__( TAG, "~CProgressDlg()" );

	__release();
}

// Initialize
void CProgressDlg::__init(void) {
	__LOGT__( TAG, "__init()" );

	//m_pError = NULL;

	m_working = false;
	m_updated = false;

	m_progress = NULL;
	m_progress_total = 0;
	m_progress_pos = 0;

	m_pSpinner_dlg = NULL;
	m_pSpinner = NULL;
	m_pLabel = NULL;
	m_pLabel_progress = NULL;
	m_pProgressBar = NULL;

	m_pHBox_spinner_progress = NULL;
	m_pVBox = NULL;
}

// Release
void CProgressDlg::__release(void) {
	__LOGT__( TAG, "__release()" );


	{
		if ( m_pSpinner ) {
			gtk_spinner_stop( m_pSpinner );
			m_pSpinner = NULL;
		}

		if ( m_pLabel ) {
			gtk_widget_hide( m_pLabel );
			gtk_widget_destroy( m_pLabel );
			m_pLabel = NULL;
		}

		if ( m_pLabel_progress ) {
			gtk_widget_hide( m_pLabel_progress );
			gtk_widget_destroy( m_pLabel_progress );
			m_pLabel_progress = NULL;
		}

		if ( m_pProgressBar ) {
			gtk_widget_hide( m_pProgressBar );
			gtk_widget_destroy( m_pProgressBar );
			m_pProgressBar = NULL;
		}

		if ( m_pSpinner_dlg ) {
			gtk_widget_hide( m_pSpinner_dlg );
			gtk_widget_destroy( m_pSpinner_dlg );
			m_pSpinner_dlg = NULL;
		}


		/*
		if ( m_pHBox_spinner_progress ) {
			gtk_widget_hide( m_pHBox_spinner_progress );
			gtk_widget_destroy( m_pHBox_spinner_progress );
			m_pHBox_spinner_progress = NULL;
		}

		if ( m_pVBox ) {
			gtk_widget_hide( m_pVBox );
			gtk_widget_destroy( m_pVBox );
			m_pVBox = NULL;
		}
		*/
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

void CProgressDlg::updates(void) {
	//__LOGT__( TAG, "updates()" );

	if ( m_pLabel )
		gtk_label_set_text( (GtkLabel*)m_pLabel, get_message() );

	if ( m_pLabel_progress )
		gtk_label_set_text( (GtkLabel*)m_pLabel_progress, get_progress_msg() );

	if ( m_pProgressBar ) {
		gtk_progress_bar_set_text( (GtkProgressBar*)m_pProgressBar, get_progress_msg() );
		gtk_progress_bar_set_fraction( (GtkProgressBar*)m_pProgressBar, get_progress_fraction() );
	}
}

void CProgressDlg::wait_for_thread_finished(void) {
	__LOGT__( TAG, "wait_for_thread_finished()" );

	while ( get_working() ) {
		__usleep( 100 );
	}

	__LOGT__( TAG, "wait_for_thread_finished(): finished" );
}

void CProgressDlg::set_updated(bool val) { m_updated = val; }
bool CProgressDlg::get_updated(void) { return m_updated; }
bool CProgressDlg::get_working(void) { return m_working; }

void CProgressDlg::set_width_height(const int width, const int height) {
	m_width = width; m_height = height;
}
int CProgressDlg::get_width(void) { return m_width; }
int CProgressDlg::get_height(void) { return m_height; }
bool CProgressDlg::get_resizable(void) { return m_resizable; }

void CProgressDlg::set_title(const char* title, const bool resizable) {
	m_title = (char*)title; m_resizable = resizable;
}
const char* CProgressDlg::get_title(void) { return m_title; }

void CProgressDlg::set_message(const char* msg) { m_msg = (char*)msg; }
const char* CProgressDlg::get_message(void) { return m_msg; }

void CProgressDlg::set_progress_msg(const char* progress) {
	m_progress = (char*)progress; updates();
}
const char* CProgressDlg::get_progress_msg(void) { return m_progress; }

int CProgressDlg::set_progress(int pos, int total) {
	//__LOGT__( TAG, "set_progress()" );

	m_progress_pos = pos;
	m_progress_total = total;

	return get_progress();
}

int CProgressDlg::get_progress(void) {
	//__LOGT__( TAG, "get_progress()" );

	const int rate = 100;
	int percent = (m_progress_pos*rate) / m_progress_total;

	return percent;
}

double CProgressDlg::get_progress_fraction(void) {
	//__LOGT__( TAG, "get_progress_fraction()" );

	const int rate = 100;
	double percent = (m_progress_pos*rate) / m_progress_total;

	percent = (percent >= 0)? (percent / 100) : 0.f;

	return percent;
}

gint CProgressDlg::show(GtkWindow* pWindow, gpointer user_data, const char* msg,
		const e_ShowProgress_t show_type) {
	__LOGT__( TAG, "show()" );

	gint ret_spinner_dlg = GTK_RESPONSE_NONE;


	if ( get_working() ) {
		//__LOGT__( TAG, "show(): working... SKIP" );
		return ret_spinner_dlg;
	}

	// Destroy
	destroy_ui();

	if ( !pWindow ) {
		__LOGT__( TAG, "show(): GtkWindow == NULL" );
		return ret_spinner_dlg;
	}

	//if ( !user_data ) {
	//	__LOGT__( TAG, "show(): user_data == NULL" );
	//	return ret_spinner_dlg;
	//}

	if ( msg ) {
		set_message( msg );
	}
	else if ( !get_message() ) {
		__LOGT__( TAG, "show(): Message == NULL" );
		return ret_spinner_dlg;
	}

	__LOGT__( TAG, "show(): Message = %s", get_message() );


	m_pSpinner = (GtkSpinner*)gtk_spinner_new();
	if ( m_pSpinner ) {
		//gtk_widget_show( ((Widgets_st*)user_data)->pSpinner );
		//gtk_spinner_start( (GtkSpinner*)((Widgets_st*)user_data)->pSpinner );

		m_pSpinner_dlg = gtk_dialog_new_with_buttons( "In progress...",
						// use a transient parent for set layout to center-align onto parent
						//(GtkWindow*)((Widgets_st*)user_data)->pWindow,
						pWindow,
						GTK_DIALOG_MODAL,
						GTK_STOCK_CANCEL,
						GTK_RESPONSE_CANCEL,
						NULL );
		//gtk_widget_set_size_request( m_pSpinner_dlg, 220, 100 );
		gtk_window_set_resizable( GTK_WINDOW(m_pSpinner_dlg), true );

		if ( m_pSpinner_dlg ) {
			// Spinner
			/*
			{
				gtk_container_add( GTK_CONTAINER(gtk_dialog_get_content_area(
								GTK_DIALOG(m_pSpinner_dlg))),
								(GtkWidget*)m_pSpinner );
				gtk_spinner_start( m_pSpinner );
			}
			*/

			// Label
			{
				m_pLabel = gtk_label_new( get_message() );
				m_pLabel_progress = gtk_label_new( DEFAULT_STR_PROGRESS );	// "0% (0/100)"

				if ( !m_pLabel || !m_pLabel_progress ) {
					if ( !m_pLabel ) {
						__LOGT__( TAG, "show(): Label Message == NULL" );
					}

					if ( !m_pLabel_progress ) {
						__LOGT__( TAG, "show(): Label Progress == NULL" );
					}

					destroy_ui();

					return false;
				}

				// Message
				gtk_label_set_justify( GTK_LABEL(m_pLabel), GTK_JUSTIFY_LEFT );
				//gtk_container_add( GTK_CONTAINER(gtk_dialog_get_content_area(
				//					GTK_DIALOG(m_pSpinner_dlg))),
				//					m_pLabel );

				// Progress
				gtk_label_set_justify( GTK_LABEL(m_pLabel_progress), GTK_JUSTIFY_CENTER );
			}

			// ProgressBar
			{
				double fraction = 0.0f;

				m_pProgressBar = gtk_progress_bar_new();

				if ( !m_pProgressBar ) {
					__LOGT__( TAG, "show(): Label Message == NULL" );

					destroy_ui();

					return false;
				}

				gtk_progress_bar_set_text( (GtkProgressBar*)m_pProgressBar, DEFAULT_STR_PROGRESS );

#ifdef __GTKv2__
				gtk_progress_bar_set_orientation( (GtkProgressBar*)m_pProgressBar, GTK_PROGRESS_LEFT_TO_RIGHT );
#elif __GTKv3__
				{
					GtkOrientable* orientable = NULL;
					orientable = GTK_ORIENTABLE( m_pProgressBar );
					if ( orientable ) {
						gtk_orientable_set_orientation( orientable, GTK_ORIENTATION_HORIZONTAL );
					}
				}
#else
#endif
				gtk_progress_bar_set_fraction( (GtkProgressBar*)m_pProgressBar, fraction );
				gtk_progress_bar_pulse( (GtkProgressBar*)m_pProgressBar );
			}

			// Layout
			{
				m_pHBox_spinner_progress = gtk_hbox_new( false, 0 );
				m_pVBox = gtk_vbox_new( false, 0 );

				if ( !m_pHBox_spinner_progress || !m_pVBox ) {
					if ( !m_pHBox_spinner_progress ) {
						__LOGT__( TAG, "show(): HBox Spinner_Progress == NULL" );
					}

					if ( !m_pVBox ) {
						__LOGT__( TAG, "show(): VBox == NULL" );
					}

					destroy_ui();

					return false;
				}

				{
					// Spinner, Label(Progress)

					if ( (show_type == e_showProgress_ALL) || (show_type == e_showProgress_NO_PROGRESSBAR) ) {
						gtk_box_pack_start( GTK_BOX(m_pHBox_spinner_progress), GTK_WIDGET(m_pSpinner), false, false, 1 );
						gtk_box_pack_start( GTK_BOX(m_pHBox_spinner_progress), m_pLabel_progress, false, false, 1 );
					}
					else if ( show_type == e_showProgress_NO_PROGRESSBAR_NO_PROGRESS_LABEL ) {
						gtk_box_pack_start( GTK_BOX(m_pHBox_spinner_progress), GTK_WIDGET(m_pSpinner), false, false, 1 );
					}
				}

				{
					// Spinner, Label(Progress), ProgressBar, Label(Message)

					if ( (show_type == e_showProgress_ALL) || (show_type == e_showProgress_NO_PROGRESSBAR) ||
							(show_type == e_showProgress_NO_PROGRESSBAR_NO_PROGRESS_LABEL) ) {
						gtk_box_pack_start( GTK_BOX(m_pVBox), m_pHBox_spinner_progress, false, false, 1 );
					}
					if ( (show_type == e_showProgress_ALL) || (show_type == e_showProgress_PROGRESSBAR) ) {
						gtk_box_pack_start( GTK_BOX(m_pVBox), m_pProgressBar, false, false, 1 );
					}
					gtk_box_pack_start( GTK_BOX(m_pVBox), m_pLabel, false, false, 1 );


					gtk_container_add( GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(m_pSpinner_dlg))),
										m_pVBox );
				}


				// Spinner
				//  - Start the Spinner
				gtk_spinner_start( m_pSpinner );
			}

			gtk_widget_show_all( m_pSpinner_dlg );



			//
			// run the Thread
			//
			
			set_working( true );

			if ( !this->execute(m_pSpinner_dlg) ) {
				__LOGT__( TAG, "show(): execute [FAIL]" );

				destroy_ui();

				set_working( false );
				return ret_spinner_dlg;
			}



			//
			//! Will blocking the following
			//
			
			__LOGT__( TAG, "show(): Run a Dialog" );

			// for catch an interrupts
			//  - send a signal: gtk_dialog_response( GTK_DIALOG(__DIALOG__), GTK_RESPONSE_NONE );
			ret_spinner_dlg = gtk_dialog_run( GTK_DIALOG(m_pSpinner_dlg) );
			switch ( ret_spinner_dlg ) {
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
						__LOGT__( TAG, "show(): Destroyed: Response code = %d", ret_spinner_dlg );
						this->cancel();
					} break;
			}


			// wait for cancelled thread task
			// SEE: CThreadTask::cancel(void);
		}
	}

	set_working( false );

	// Destroy
	destroy_ui();


	return ret_spinner_dlg;
}

void CProgressDlg::destroy_ui(void) {
	__LOGT__( TAG, "destroy_ui()" );

	m_working = false;
	m_updated = false;

	m_progress = NULL;
	m_progress_total = 0;
	m_progress_pos = 0;


	if ( m_pSpinner ) {
		gtk_spinner_stop( m_pSpinner );
		m_pSpinner = NULL;
	}

	if ( m_pLabel ) {
		gtk_widget_hide( m_pLabel );
		gtk_widget_destroy( m_pLabel );
		m_pLabel = NULL;
	}

	if ( m_pLabel_progress ) {
		gtk_widget_hide( m_pLabel_progress );
		gtk_widget_destroy( m_pLabel_progress );
		m_pLabel_progress = NULL;
	}

	if ( m_pProgressBar ) {
		gtk_widget_hide( m_pProgressBar );
		gtk_widget_destroy( m_pProgressBar );
		m_pProgressBar = NULL;
	}

	if ( m_pSpinner_dlg ) {
		gtk_widget_hide( m_pSpinner_dlg );
		gtk_widget_destroy( m_pSpinner_dlg );
		m_pSpinner_dlg = NULL;
	}

	/*
	if ( m_pHBox_spinner_progress ) {
		gtk_widget_hide( m_pHBox_spinner_progress );
		gtk_widget_destroy( m_pHBox_spinner_progress );
		m_pHBox_spinner_progress = NULL;
	}

	if ( m_pVBox ) {
		gtk_widget_hide( m_pVBox );
		gtk_widget_destroy( m_pVBox );
		m_pVBox = NULL;
	}
	*/
}

void CProgressDlg::stop(void) {
	__LOGT__( TAG, "stop()" );

	set_working( false );
	//destroy_ui();
	//^~~~~~~~~~~~~
	// gtkwidget.c:15927:gtk_widget_unregister_window: assertion failed: (user_data == widget)
}

void CProgressDlg::__stop(void) {
	__LOGT__( TAG, "__stop()" );

	set_working( false );

	if ( m_pSpinner_dlg ) {
		__LOGT__( TAG, "__stop(): Send a signal to Destroy Dialog" );
		gtk_dialog_response( GTK_DIALOG(m_pSpinner_dlg), GTK_RESPONSE_NONE );
	}

	// wait for the thread finished
	wait_for_thread_finished();
}

// ---------------------------------------------------------------


void* CProgressDlg::inBackground(std::vector<void*>* pvecVal) {
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
		/*
		if ( get_updated() ) {
			// updates message
			updates();
			set_updated( false );
		}
		*/

		//__LOGT__( TAG, "inBackground(): get_working() = %s", (get_working()? "TRUE" : "FALSE") );
		//__LOGT__( TAG, "inBackground(): get_cancel_state() = %s", (this->get_cancel_state()? "TRUE" : "FALSE") );


		// perform the task
		task_in_thread();


		__usleep( 100 );
	} while ( get_working() && !this->get_cancel_state() );


	return ((void*)true);
}

void CProgressDlg::progressUpdate(void* pVal) {
	__LOGT__( TAG, "progressUpdate()" );

	char* pStr = reinterpret_cast<char*>(pVal);

	if ( pStr ) {
		set_updated();
		__LOGT__( TAG, "%s", pStr );
		m_msg = pStr;
	}
}

void CProgressDlg::postExecute(void* pResult, void* pExtraVal) {
	__LOGT__( TAG, "postExecute()" );

	//int result = reinterpret_cast<int>(pResult);
	int result = static_cast<int>(reinterpret_cast<long>(pResult));
	__LOGT__( TAG, "postExecute(): result = %s", (result? "TRUE" : "FALSE") );


	__stop();
}
// class CASyncTask ]
// ---------------------------------------------------------------


