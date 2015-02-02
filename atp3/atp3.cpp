/* --------------------------------------------------------------
Project:	ATP(A Tentative Plan) 3
Purpose:	
Author:		Ho-Jung Kim (godmode2k@hotmail.com)
Date:		Since Dec 2, 2014
Filename:	atp3.cpp

Last modified: Jan 30, 2015
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
#include "atp3.h"
// ---------------------------------------------------------------



//! Definition
// ---------------------------------------------------------------
// Global variable
CAtp3* g_pCAtp3 = NULL;
CProgress_dlg g_progressDlg;		// Progress Dialog
CToastMsg_dlg g_toastMsgDlg;		// Toast Message Dialog
// ---------------------------------------------------------------



//! Prototype
// ---------------------------------------------------------------
// ---------------------------------------------------------------



//! Class
// ---------------------------------------------------------------
// Ctor
CAtp3::CAtp3(void) : TAG("CAtp3") {
	__LOGT__( TAG, "CAtp3()" );

	__init();
}

// Dtor
CAtp3::~CAtp3() {
	__LOGT__( TAG, "~CAtp3()" );

	__release();
}

// Initialize
void CAtp3::__init(void) {
	__LOGT__( TAG, "__init()" );

	m_pGladeXml = NULL;
	m_pError = NULL;
	m_pWidgets = NULL;

	// Main View
	m_pCViewMain = new CViewMain;

	// Popup Menu
	m_show_popup_menu = false;
}

// Release
void CAtp3::__release(void) {
	__LOGT__( TAG, "__release()" );

	if ( m_pWidgets ) {
		if ( m_pWidgets->pCanvas ) {
			cairo_destroy( m_pWidgets->pCanvas );
			m_pWidgets->pCanvas = NULL;
		}
	}

	g_slice_free( Widgets_st, m_pWidgets );

	if ( m_pCViewMain ) {
		delete m_pCViewMain;
		 m_pCViewMain = NULL;
	}
}

// Initialize Widget structure
bool CAtp3::init_widget_all(Widgets_st** pWidgets) {
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

	//! Load an UI
	// ---------------------------------------------------------------
	//pGladeXml = glade_xml_new( "ui.glade", NULL, NULL );
	//pWindow = glade_xml_get_widget( pGladeXml, "window1" );
	m_pGladeXml = gtk_builder_new();
	//if ( !gtk_builder_add_from_file(m_pGladeXml, DEFAULT_STR_UI_GLADE_FILE, &m_pError) ) {
	//if ( !gtk_builder_add_from_string(m_pGladeXml, (const gchar*)&ui_glade, ui_glade_len, &m_pError) ) {
	if ( !gtk_builder_add_from_string(m_pGladeXml,
				(const gchar*)&DEFAULT_STR_UI_GLADE_STR_VAR, DEFAULT_STR_UI_GLADE_STR_LEN, &m_pError) ) {
		g_warning( "init_ui_with_callback(): %s", m_pError->message );
		if ( m_pError ) {
			g_free( m_pError );
			m_pError = NULL;
		}
		return false;
	}

	if ( !init_widget_all(&m_pWidgets) ) {
		__LOGT__( TAG, "init_ui_with_callback(): INIT [FAIL]" );
		return false;
	}

	__LOGT__( TAG, "init_ui_with_callback(): INIT [OK]" );



	//! Get Objects from UI and Set a signals
	// ---------------------------------------------------------------
	/*
//	gtk_container_add( GTK_CONTAINER(pImgWgtEvent), pImg_edit_window );
	g_signal_connect( G_OBJECT(pImgWgtEvent), "button-press-event",
					G_CALLBACK(on_mouse_press), pImgWgtEvent );
	g_signal_connect( G_OBJECT(pImgWgtEvent), "button-release-event",
					G_CALLBACK(on_mouse_release), pImgWgtEvent );
	g_signal_connect( G_OBJECT(pImgWgtEvent), "motion-notify-event",
					G_CALLBACK(on_mouse_move), pImgWgtEvent );

	//! Tool Menu Button event
	pWindow = glade_xml_get_widget( pGladeXml, "button_open" );
	g_signal_connect( G_OBJECT(pWindow), "button-release-event",
					G_CALLBACK(on_button_open_clicked), NULL );
	*/


	// Main Toplevel Window
	//pWidgets->pWindow = glade_xml_get_widget( pGladeXml, "window1" );
	m_pWidgets->pWindow = GTK_WIDGET( gtk_builder_get_object(m_pGladeXml, "window1") );
	g_signal_connect( G_OBJECT(m_pWidgets->pWindow), "destroy",
					G_CALLBACK(g_Func::main_window_quit), NULL );
	//g_signal_connect( G_OBJECT(m_pWidgets->pWindow), "expose-event",
	//				G_CALLBACK(g_FuncSignalHandler::on_event_draw_top_window), m_pWidgets );

	//gtk_widget_set_app_paintable( m_pWidgets->pWindow, true );



	// Container
	m_pWidgets->pTopContainer = GTK_WIDGET( gtk_builder_get_object(m_pGladeXml, "vbox1") );
	m_pWidgets->pContainerScrolledWindow =
				GTK_WIDGET( gtk_builder_get_object(m_pGladeXml, "scrolledwindow1") );


	// Menu: File->Quit
	m_pWidgets->pMenuItem = GTK_WIDGET( gtk_builder_get_object(m_pGladeXml, "imagemenuitem5") );
	g_signal_connect( G_OBJECT(m_pWidgets->pMenuItem), "activate",
					G_CALLBACK(g_Func::main_window_quit), NULL );


	// Menu: File->Open
	m_pWidgets->pMenuItem = GTK_WIDGET( gtk_builder_get_object(m_pGladeXml, "imagemenuitem2") );
	g_signal_connect( G_OBJECT(m_pWidgets->pMenuItem), "activate",
					G_CALLBACK(g_FuncSignalHandler::on_menu_button_open_clicked), m_pWidgets );


	// Toolbar Button: Update
	m_pWidgets->pToolbarButton =
					GTK_WIDGET( gtk_builder_get_object(m_pGladeXml, "Toolbutton_Update") );
	g_signal_connect( G_OBJECT(m_pWidgets->pToolbarButton), "clicked",
					G_CALLBACK(g_FuncSignalHandler::on_toolbar_button_update_clicked), m_pWidgets );


	// Toolbar Button: ToastMsg
	m_pWidgets->pToolbarButton =
					GTK_WIDGET( gtk_builder_get_object(m_pGladeXml, "Toolbutton_ToastMsg") );
	g_signal_connect( G_OBJECT(m_pWidgets->pToolbarButton), "clicked",
					G_CALLBACK(g_FuncSignalHandler::on_toolbar_button_toastmsg_clicked), m_pWidgets );

	// Toolbar Button: +Image
	m_pWidgets->pToolbarButton =
					GTK_WIDGET( gtk_builder_get_object(m_pGladeXml, "Toolbutton_AddImage") );
	g_signal_connect( G_OBJECT(m_pWidgets->pToolbarButton), "clicked",
					G_CALLBACK(g_FuncSignalHandler::on_toolbar_button_add_img_clicked), m_pWidgets );

	// Toolbar Button: +Text(BG)
	m_pWidgets->pToolbarButton =
					GTK_WIDGET( gtk_builder_get_object(m_pGladeXml, "Toolbutton_AddTextWithBgImage") );
	g_signal_connect( G_OBJECT(m_pWidgets->pToolbarButton), "clicked",
					G_CALLBACK(g_FuncSignalHandler::on_toolbar_button_add_text_with_bg_img_clicked), m_pWidgets );

	// Toolbar Button: -Image/Text
	m_pWidgets->pToolbarButton =
					GTK_WIDGET( gtk_builder_get_object(m_pGladeXml, "Toolbutton_DeleteAttachedObject") );
	g_signal_connect( G_OBJECT(m_pWidgets->pToolbarButton), "clicked",
					G_CALLBACK(g_FuncSignalHandler::on_toolbar_button_delete_attached_obj_clicked), m_pWidgets );

	// Toolbar Button: ClearAll
	m_pWidgets->pToolbarButton =
					GTK_WIDGET( gtk_builder_get_object(m_pGladeXml, "Toolbutton_ClearAll") );
	g_signal_connect( G_OBJECT(m_pWidgets->pToolbarButton), "clicked",
					G_CALLBACK(g_FuncSignalHandler::on_toolbar_button_clear_all), m_pWidgets );



	// TreeView
	//m_pWidgets->pTreeViewCtrl = (GtkTreeView*)gtk_builder_get_object( m_pGladeXml, "treeview1" );
	//g_signal_connect( G_OBJECT(m_pWidgets->pTreeViewCtrl), "clicked",
	//				G_CALLBACK(g_FuncSignalHandler::on_button_open_clicked), m_pWidgets );
	//gtk_tree_view_columns_autosize( m_pWidgets->pTreeViewCtrl );
	

	// DrawingArea
	//m_pWidgets->pDrawingArea = (GtkDrawingArea*)gtk_builder_get_object( m_pGladeXml, "DrawingArea" );
	m_pWidgets->pDrawingArea = GTK_WIDGET( gtk_builder_get_object(m_pGladeXml, "DrawingArea") );
#ifdef __GTKv2__
	// v2.x: "expose-event", v3.x: "draw"
	g_signal_connect( G_OBJECT(m_pWidgets->pDrawingArea), "expose-event",
#elif __GTKv3__
	g_signal_connect( G_OBJECT(m_pWidgets->pDrawingArea), "draw",
#else
#endif
					G_CALLBACK(g_FuncSignalHandler::on_event_draw_main), m_pWidgets );
	g_signal_connect( G_OBJECT(m_pWidgets->pDrawingArea), "size-allocate",
					G_CALLBACK(g_FuncSignalHandler::on_event_gtkfixed_size_allocate), NULL );





	// Keyboard & Mouse Event
	// ---------------------------------------------------------------
	//  - Keyboard
	g_signal_connect( G_OBJECT(m_pWidgets->pWindow), "key-press-event",
					G_CALLBACK(g_FuncSignalHandler::on_event_key_press), NULL );
	g_signal_connect( G_OBJECT(m_pWidgets->pWindow), "key-release-event",
					G_CALLBACK(g_FuncSignalHandler::on_event_key_release), NULL );
	//  - Mouse
	g_signal_connect( G_OBJECT(m_pWidgets->pDrawingArea), "motion-notify-event",
					G_CALLBACK(g_FuncSignalHandler::on_event_mouse), m_pWidgets );
	g_signal_connect( G_OBJECT(m_pWidgets->pDrawingArea), "button-press-event",
					G_CALLBACK(g_FuncSignalHandler::on_event_mouse), m_pWidgets );
	g_signal_connect( G_OBJECT(m_pWidgets->pDrawingArea), "button-release-event",
					G_CALLBACK(g_FuncSignalHandler::on_event_mouse), m_pWidgets );
	// Set events
	//gtk_widget_set_events( m_pWidgets->pViewport, GDK_POINTER_MOTION_HINT_MASK
	gtk_widget_set_events( m_pWidgets->pDrawingArea, GDK_POINTER_MOTION_HINT_MASK
			| GDK_POINTER_MOTION_MASK
			| GDK_LEAVE_NOTIFY_MASK
			| GDK_KEY_PRESS_MASK
			| GDK_KEY_RELEASE_MASK
			| GDK_BUTTON1_MOTION_MASK
			| GDK_BUTTON2_MOTION_MASK
			| GDK_BUTTON3_MOTION_MASK
			| GDK_SCROLL_MASK
			| GDK_BUTTON_PRESS_MASK
			| GDK_BUTTON_RELEASE_MASK
	);





	//! Load/Set data
	// ---------------------------------------------------------------
	// ...



	//! if callback function defined in glade
	// ---------------------------------------------------------------
	//gtk_builder_connect_signals( m_pGladeXml, NULL );



	return true;
}

bool CAtp3::create_popup_menu(e_popupMenu_t type, bool show) {
	//__LOGT__( TAG, "create_popup_menu()" );

	bool ret = false;


	// Release
	release_popup_menu();

	m_pWidgets->pPopupMenu = gtk_menu_new();

	if ( !m_pWidgets->pPopupMenu ) {
		__LOGT__( TAG, "create_popup_menu(): Popup Menu = NULL" );
		return false;
	}

	ret = g_FuncSignalHandler::on_event_create_popup_menu(
			m_pWidgets->pPopupMenu, m_pWidgets, type );

	if ( ret && show ) {
		//__LOGT__( TAG, "create_popup_menu(): show" );
		show_popup_menu();
	}


	return ret;
}

void CAtp3::release_popup_menu(void) {
	//__LOGT__( TAG, "release_popup_menu()" );

	m_show_popup_menu = false;

	if ( m_pWidgets->pPopupMenu ) {
		gtk_widget_hide( m_pWidgets->pPopupMenu );
		gtk_widget_destroy( m_pWidgets->pPopupMenu );
		m_pWidgets->pPopupMenu = NULL;
	}
}

bool CAtp3::show_popup_menu(void) {
	//__LOGT__( TAG, "show_popup_menu()" );

	if ( !m_pWidgets->pPopupMenu ) {
		__LOGT__( TAG, "show_popup_menu(): Popup Menu = NULL" );

		m_show_popup_menu = false;

		return false;
	}

	m_show_popup_menu = true;
	gtk_widget_show_all( m_pWidgets->pPopupMenu );
	gtk_menu_popup( GTK_MENU(m_pWidgets->pPopupMenu),
					NULL, NULL, NULL, NULL,
					/*
					//! GdkEventButton
					event->button,
					event->time
					*/
					GDK_BUTTON_PRESS,
					gtk_get_current_event_time() );


	return true;
}

bool CAtp3::is_show_popup_menu(void) {
	//__LOGT__( TAG, "is_show_popup_menu()" );

	return m_show_popup_menu;
}
// ---------------------------------------------------------------



//! Implementation
// ---------------------------------------------------------------


// g_Func and g_FuncSignalHandler
#include "incl_g_func.cxx"
#include "incl_g_func_signal_handler.cxx"

// class CProgressdlg
#include "incl_progress_dlg.cxx"
// class CToastMsg_dlg
#include "incl_toastmsg_dlg.cxx"


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
int main(int argc, char* argv[]) {
	__LOGT__( "ATP3", "main()" );

#ifdef __LINUX__
	/*
		[xcb] Unknown request in queue while dequeuing
		[xcb] Most likely this is a multi-threaded client and XInitThreads has not been called
		[xcb] Aborting, sorry about that.
		...: ../../src/xcb_io.c:178: dequeue_pending_request: Assertion `!xcb_xlib_unknown_req_in_deq' failed.
		Aborted


		or


		The program '...' received an X Window System error.
		This probably reflects a bug in the program.
		The error was 'BadGC (invalid GC parameter)'.
			(Details: serial 4202 error_code 13 request_code 56 minor_code 0)
			(Note to programmers: normally, X errors are reported asynchronously;
			that is, you will receive the error a while after causing it.
			To debug your program, run it with the --sync command line
			option to change this behavior. You can then get a meaningful
			backtrace from your debugger if you break on the gdk_x_error() function.)
	*/

	//! X11 Thread
	// ---------------------------------------------------------------
	XInitThreads();
#elif _WINDOWS
#else
#endif



	//! GTK Init
	// ---------------------------------------------------------------
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



	{
		g_pCAtp3 = new CAtp3;
		if ( !g_pCAtp3 )
			return -1;

		if ( !g_pCAtp3->init_ui_with_callback() )
			return -1;
	}



	//! Release the glade builder
	// ---------------------------------------------------------------
	g_object_unref( G_OBJECT(g_pCAtp3->get_gtk_builder()) );



	//! Rendering
	// ---------------------------------------------------------------
	gtk_window_set_title( GTK_WINDOW(g_pCAtp3->get_widget_st()->pWindow), "Atp3: Hello~" );
	gtk_widget_show( g_pCAtp3->get_widget_st()->pWindow );                
	// unset any focus widget for the toplevel window
	gtk_window_set_focus( GTK_WINDOW(g_pCAtp3->get_widget_st()->pWindow), NULL );



	//! Start Application
	// ---------------------------------------------------------------
	gtk_main();



	//! Release
	// ---------------------------------------------------------------
	//g_slice_free( Widgets_st, pWidgets );

	return 0;
}
