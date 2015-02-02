/* --------------------------------------------------------------
Project:	CDialog
Purpose:	GTK+ Dialog classes
Author:		Ho-Jung Kim (godmode2k@hotmail.com)
Date:		Since June 28, 2012
Filename:	CDialog.cpp

Last modified: July 17, 2012
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
#include "CDialog.h"
#include "util/CUtil.h"
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
CDialog::CDialog(void) : TAG("CDialog") {
	__LOGT__( TAG, "CDialog()" );

	__init();
}

// Dtor
CDialog::~CDialog() {
	__LOGT__( TAG, "~CDialog()" );

	__release();
}

// Initialize
void CDialog::__init(void) {
	__LOGT__( TAG, "__init()" );
}

// Release
void CDialog::__release(void) {
	__LOGT__( TAG, "__release()" );

	g_slice_free( DialogWidgets_st, m_pWidgets );
}

// Initialize Widget structure
bool CDialog::init_widget_all(DialogWidgets_st** pWidgets) {
	__LOGT__( TAG, "init_widget_all()" );

	(*pWidgets) = g_slice_new( DialogWidgets_st );

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
bool CDialog::init_ui_with_callback(void) {
	__LOGT__( TAG, "init_ui_with_callback()" );

/*
	//! Load an UI
	// ---------------------------------------------------------------
	//pGladeXml = glade_xml_new( "ui.glade", NULL, NULL );
	//pWindow = glade_xml_get_widget( pGladeXml, "window1" );
	m_pGladeXml = gtk_builder_new();
	if ( !gtk_builder_add_from_file(m_pGladeXml, DEFAULT_STR_UI_GLADE_FILE, &m_pError) ) {
		g_warning( "init_ui_with_callback(): %s", m_pError->message );
		if ( m_pError ) {
			g_free( m_pError );
			m_pError = NULL;
		}
		return false;
	}
*/

	if ( !init_widget_all(&m_pWidgets) ) {
		__LOGT__( TAG, "init_ui_with_callback(): INIT [FAIL]" );
		return false;
	}
	else {
		__LOGT__( TAG, "init_ui_with_callback(): INIT [OK]" );
	}

/*
	//! Get Objects from UI and Set a signals
	// ---------------------------------------------------------------
	// Main Top Window
	//pWidgets->pWindow = glade_xml_get_widget( pGladeXml, "window1" );
	m_pWidgets->pWindow = GTK_WIDGET( gtk_builder_get_object(m_pGladeXml, "window1") );
	g_signal_connect( G_OBJECT(m_pWidgets->pWindow), "destroy",
					G_CALLBACK(g_Dialog_Func::main_window_quit), NULL );
	// Container
	m_pWidgets->pTopContainer = GTK_WIDGET( gtk_builder_get_object(m_pGladeXml, "vbox1") );
	m_pWidgets->pContainerScrolledWindow = GTK_WIDGET( gtk_builder_get_object(m_pGladeXml, "scrolledwindow1") );

	// Menu: File->Open
	m_pWidgets->pMenuItem = GTK_WIDGET( gtk_builder_get_object(m_pGladeXml, "imagemenuitem2") );
	g_signal_connect( G_OBJECT(m_pWidgets->pMenuItem), "activate",
					G_CALLBACK(g_Dialog_FuncSignalHandler::on_button_open_clicked), m_pWidgets );

	// Toolbar Button: Update
	m_pWidgets->pToolbarButton = GTK_WIDGET( gtk_builder_get_object(m_pGladeXml, "Toolbutton_Update") );
	g_signal_connect( G_OBJECT(m_pWidgets->pToolbarButton), "clicked",
					G_CALLBACK(g_Dialog_FuncSignalHandler::on_button_update_clicked), m_pWidgets );

	// TreeView
	m_pWidgets->pTreeViewCtrl = (GtkTreeView*)gtk_builder_get_object(m_pGladeXml, "treeview1" );
	//g_signal_connect( G_OBJECT(m_pWidgets->pTreeViewCtrl), "clicked",
	//				G_CALLBACK(g_Dialog_FuncSignalHandler::on_button_open_clicked), m_pWidgets );
	gtk_tree_view_columns_autosize( m_pWidgets->pTreeViewCtrl );
*/



	//! Load/Set data
	// ---------------------------------------------------------------



	//! if callback function defined in glade
	// ---------------------------------------------------------------
	//gtk_builder_connect_signals( m_pGladeXml, NULL );



	return true;
}

void CDialog::fileChooser(GtkWidget* widget, gpointer user_data) {
	__LOGT__( TAG, "fileChooser()" );

	GtkWidget* dlg = NULL;
	dlg = gtk_file_chooser_dialog_new( "Open File",
			//(GtkWindow*)widget,

			// Parent Window
			// Gtk-CRITICAL: IA__gtk_window_set_transient_for
			(GtkWindow*)((DialogWidgets_st*)user_data)->pWindow,

			GTK_FILE_CHOOSER_ACTION_OPEN,
			GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
			GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
			NULL );

	if ( !dlg ) {
		__LOGT__( TAG, "fileChooser(): File Chooser Dialog == NULL" );
		return;
	}

	if ( gtk_dialog_run(GTK_DIALOG(dlg) ) == GTK_RESPONSE_ACCEPT ) {
		char* filename;

		filename = gtk_file_chooser_get_filename( GTK_FILE_CHOOSER(dlg) );

		if ( filename ) {
			//open_file( filename );

			/*
			// Display Image
			__LOGT__( TAG, "fileChooser(): filename = %s\n", filename );
			g_pEvtData->setImgSrcFilename( filename );
			default_display( widget, NULL );
			*/

			g_free( filename );
		}
	}

	gtk_widget_destroy( dlg );
}

void CDialog::progress(GtkWidget* widget, gpointer user_data) {
	__LOGT__( TAG, "progress()" );

	// Start progress animation
	GtkWidget* pSpinnerDlg = NULL;
	gint spinnerDlgResult = GTK_RESPONSE_NONE;


	((DialogWidgets_st*)user_data)->pSpinner = (GtkSpinner*)gtk_spinner_new();

	if ( ((DialogWidgets_st*)user_data)->pSpinner ) {
		//gtk_widget_show( ((DialogWidgets_st*)user_data)->pSpinner );
		//gtk_spinner_start( (GtkSpinner*)((DialogWidgets_st*)user_data)->pSpinner );

		pSpinnerDlg = gtk_dialog_new_with_buttons( "In progressing...",
						// use a transient parent for set layout to center-align onto parent
						(GtkWindow*)((DialogWidgets_st*)user_data)->pWindow,
						GTK_DIALOG_MODAL,
						GTK_STOCK_CANCEL,
						GTK_RESPONSE_CANCEL,
						NULL );
		gtk_widget_set_size_request( pSpinnerDlg, 220, 70 );
		gtk_window_set_resizable( GTK_WINDOW(pSpinnerDlg), false );

		if ( pSpinnerDlg ) {
			gtk_container_add( GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(pSpinnerDlg))),
							(GtkWidget*)((DialogWidgets_st*)user_data)->pSpinner );
			gtk_spinner_start( ((DialogWidgets_st*)user_data)->pSpinner );
			gtk_widget_show_all( pSpinnerDlg );

			__LOGT__( TAG, "progress(): Run a Dialog" );
			// for catch an interrupts
			spinnerDlgResult = gtk_dialog_run( GTK_DIALOG(pSpinnerDlg) );
			switch ( spinnerDlgResult ) {
				case GTK_RESPONSE_CANCEL:
					{
						__LOGT__( TAG, "progress(): Cancel Button clicked" );
					}
					break;
				case GTK_RESPONSE_NONE:
					{
						__LOGT__( TAG, "progress(): Destroyed" );
					}
					break;
				default:
					{
						__LOGT__( TAG, "progress(): Destroyed: Response code = %d", spinnerDlgResult );
					}
					break;
			}
		}
	}

	if ( pSpinnerDlg ) {
		if ( GTK_IS_WIDGET(pSpinnerDlg) ) {
			gtk_widget_destroy( pSpinnerDlg );
			//pSpinnerDlg = NULL;
		}

		pSpinnerDlg = NULL;
	}
}

// ---------------------------------------------------------------



//! Implementation
// ---------------------------------------------------------------
namespace g_Dialog_Func {
	// Callback
	void main_window_quit(GtkWidget* widget, gpointer user_data) {
		__LOGT__( TAG__g_Dialog_Func, "main_window_quit()" );

		// Finish Application
		gtk_main_quit();
	}
}	// namespace g_Dialog_Func



// class CDialogASyncTask [
CDialogAsyncTask::CDialogAsyncTask(void) : TAG("CDialogAsyncTask") {
	__LOGT__( TAG, "CDialogAsyncTask()" );
}

CDialogAsyncTask::~CDialogAsyncTask(void) {
	__LOGT__( TAG, "~CDialogAsyncTask()" );
}

void* CDialogAsyncTask::inBackground(void* pVal) {
	__LOGT__( TAG, "inBackground()" );

	return ((void*)true);
}

void CDialogAsyncTask::progressUpdate(void* pVal) {
	__LOGT__( TAG, "progressUpdate()" );

//	char* pStr = reinterpret_cast<char*>(pVal);
//	if ( pStr )
//		__LOGT__( TAG, "%s", pStr );
}

void CDialogAsyncTask::postExecute(void* pResult, void* pVal) {
	__LOGT__( TAG, "postExecute()" );

//	int result = reinterpret_cast<int>(pResult);
//	__LOGT__( TAG, "postExecute(): result = %s", (result? "TRUE" : "FALSE") );

	//gtk_widget_destroy( ... );
}
// class CDialogASyncTask ]
// ---------------------------------------------------------------

