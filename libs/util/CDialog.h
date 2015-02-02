#ifndef __CDIALOG_H__
#define __CDIALOG_H__

/* --------------------------------------------------------------
Project:	CDialog
Purpose:	GTK+ Dialog classes
Author:		Ho-Jung Kim (godmode2k@hotmail.com)
Date:		Since June 28, 2012
Filename:	CDialog.h

Last modified: July 13, 2012
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



//! Header and Definition
// ---------------------------------------------------------------
#include <iostream>
#include <stdio.h>
#include <string.h>

/*
#include <gtk-2.0/gtk/gtk.h>
#include <gtk-2.0/gdk-pixbuf/gdk-pixbuf.h>
#include <gtk-2.0/gdk/gdkpixbuf.h>
*/
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gdk/gdkpixbuf.h>

#include <glade/glade.h>


// Thread
#include "CThreadTask.h"
// ---------------------------------------------------------------



//! Definition
// ---------------------------------------------------------------
//#define DEFAULT_STR_MAX_FILENAME	255
//#define DEFAULT_STR_UI_GLADE_FILE	"ui.glade"

/*
enum g_e_ServiceType {
	e_ServiceType_A = 0x001,
	e_ServiceType_B,
	e_ServiceType_NULL = 0x20
};
*/
#ifdef __cplusplus
extern "C" {
#endif
typedef struct _DialogWidgets_st {
	GtkWidget* pWindow;
	GtkWidget* pTopContainer;
	GtkSpinner* pSpinner;
} DialogWidgets_st;

#define GET_OBJECT( builder, name, type, data ) \
	data->name = type( gtk_builder_get_object( builder, #name ) )
#define GET_WIDGET( builder, name, data ) \
	GET_OBJECT( builder, name, GTK_WIDGET, data )
#ifdef __cplusplus
}
#endif
// ---------------------------------------------------------------



//! Prototype
// ---------------------------------------------------------------
namespace g_Dialog_Func {
	#define TAG__g_Dialog_Func		"g_Dialog_Func"

	// Gtk Window Quit
	void main_window_quit(GtkWidget* widget, gpointer user_data);
}	// namespace g_Dialog_Func

namespace g_Dialog_FuncSignalHandler_Dialog {
	#define TAG__g_Dialog_FuncSignalHandler		"g_Dialog_FuncSignalHandler"
}	// namespace g_Dialog_FuncSignalHandler

// ---------------------------------------------------------------



//! Class
// ---------------------------------------------------------------

//
/*
typedef struct Widgets_st {
	GtkWidget* pWindow;
} DialogWidgets_st;
*/

// Event
class CDialogEventData {
private:
protected:
public:
	// Ctor/Dtor
	CDialogEventData(void);
	virtual ~CDialogEventData();
};

// Thread
class CDialogAsyncTask : public CThreadTask {
private:
	const char* TAG;
protected:
public:
	CDialogAsyncTask(void);
	~CDialogAsyncTask();
	void* inBackground(void* pVal = NULL);
	void progressUpdate(void* pVal);
	void postExecute(void* pResult, void* pVal = NULL);
};


// Main class
class CDialog {
private:
	const char* TAG;

	//GladeXML* m_pGladeXml;
	//GtkWidget* m_pWindow;
	GtkBuilder* m_pGladeXml;
	GError* m_pError;
	DialogWidgets_st* m_pWidgets;

	bool m_isWorking;
protected:
public:
	// Ctor/Dtor
	explicit CDialog(void);
	virtual ~CDialog();

	void __init(void);
	void __release(void);

	GtkBuilder* get_gtk_builder() { return m_pGladeXml; }
	DialogWidgets_st* get_widget_st() { return m_pWidgets; }

	// ---------------------------------------------------------------

	// Initialize
	bool init_widget_all(DialogWidgets_st** pWidgets);
	bool init_ui_with_callback(void);

	void set_working(bool val) { m_isWorking = val; }
	bool get_working(void) { return m_isWorking; }

	void fileChooser(GtkWidget* widget, gpointer user_data);
	void progress(GtkWidget* widget, gpointer user_data);
};
// ---------------------------------------------------------------



#endif	// __CDIALOG_H__
