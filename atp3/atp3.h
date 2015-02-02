#ifndef __ATP3_H__
#define __ATP3_H__

/* --------------------------------------------------------------
Project:	ATP(A Tentative Plan) 3
Purpose:	
Author:		Ho-Jung Kim (godmode2k@hotmail.com)
Date:		Since Dec 2, 2014
Filename:	atp3.h

Last modified: Jan 23, 2015
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

2. Gtk+ reference
 // GTK+ 2
 - https://developer.gnome.org/gtk2/stable/index.html
 // GTK+ 3
 - https://developer.gnome.org/gtk3/stable/index.html
 // GTK+ 2.x to GTK+ 3
 - https://developer.gnome.org/gtk3/stable/migrating.html
-------------------------------------------------------------- */



//! Header and Definition
// ---------------------------------------------------------------
#include <iostream>
#include <functional>
#include <cstdio>		//<stdio.h>
#include <cstring>		//<string.h>

#ifdef __LINUX__
	#include <X11/Xlib.h>	// XInitThreads()
#elif _WINDOWS
#else
#endif

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
#include "util/CUtil_DEF.h"
#include "util/CUtil.h"
#include "util/CThreadTask.h"
#include "util/CMutexSemaphore.h"

// GTK Dialog
#include "util/CDialog.h"
#include "util/CProgressDlg.h"
#include "util/CToastMsgDlg.h"

// View
#include "CViewMain.h"
// Keyboard & Mouse Event
#include "CKeyEvent.h"
// ---------------------------------------------------------------



//! Definition
// ---------------------------------------------------------------
#define DEFAULT_STR_MAX_FILENAME	255

// UI XML
#ifdef __cplusplus
extern "C" {
#endif

#ifdef __GTKv2__
	#define DEFAULT_STR_UI_GLADE_FILE		"ui_gtk2.glade"
	#define DEFAULT_STR_UI_GLADE_STR_VAR	ui_gtk2_glade
	#define DEFAULT_STR_UI_GLADE_STR_LEN	ui_gtk2_glade_len
#elif __GTKv3__
	#define DEFAULT_STR_UI_GLADE_FILE		"ui_gtk3.glade"
	#define DEFAULT_STR_UI_GLADE_STR_VAR	ui_gtk3_glade
	#define DEFAULT_STR_UI_GLADE_STR_LEN	ui_gtk3_glade_len
#else
#endif

	// Will be generated C-Style include file when build
	#include "ui_glade.h"				// ui_glade, ui_glade_len
#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
extern "C" {
#endif

typedef struct _Widgets_st {
	GtkWidget* pWindow;
	GtkWidget* pTopContainer;
	GtkWidget* pContainerScrolledWindow;
	GtkWidget* pMenuItem;
	GtkWidget* pToolbarButton;
	GtkWidget* pPopupMenu;
	//GtkTreeView* pTreeViewCtrl;
	GtkSpinner* pSpinner;
	GtkWidget* pDrawingArea;		// GtkDrawingArea
	cairo_t* pCanvas;				// DrawingArea: "expose-event"
	GtkWidget* pToastMsg;			// Toast message: GtkTextView
	GtkTextBuffer* pToastMsgBuf;	// Toast message buffer
} __PACK__ Widgets_st;

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
#include "incl_g_func.hxx"

// Signal Handler
#include "incl_g_func_signal_handler.hxx"
// ---------------------------------------------------------------



//! Class
// ---------------------------------------------------------------

#if 0
// Thread
class CAsyncTask : public CThreadTask {
private:
	const char* TAG;
protected:
public:
	explicit CAsyncTask(void);
	~CAsyncTask();
	void* inBackground(std::vector<void*>* pvecVal = NULL);
	void progressUpdate(void* pVal);
	void postExecute(void* pResult, void* pExtraVal = NULL);
};
#endif


// Progress Dialog
#include "incl_progress_dlg.hxx"
// Toast Message Dialog
#include "incl_toastmsg_dlg.hxx"



// Main class
class CAtp3 {
private:
	const char* TAG;

	//GladeXML* m_pGladeXml;
	//GtkWidget* m_pWindow;
	GtkBuilder* m_pGladeXml;
	GError* m_pError;
	Widgets_st* m_pWidgets;

	CViewMain* m_pCViewMain;
	CKeyEvent m_key_event;
	bool m_show_popup_menu;
protected:
public:
	// Ctor/Dtor
	explicit CAtp3(void);
	virtual ~CAtp3();

	void __init(void);
	void __release(void);

	
	GtkBuilder* get_gtk_builder(void) { return m_pGladeXml; }
	Widgets_st* get_widget_st(void) { return m_pWidgets; }

	// ---------------------------------------------------------------

	// Initialize
	bool init_widget_all(Widgets_st** pWidgets);
	bool init_ui_with_callback(void);

	// Keyboard & Mouse Event
	// ---------------------------------------------------------------
	CKeyEvent& get_key_event(void) { return m_key_event; }

	// View
	// ---------------------------------------------------------------
	CViewMain* get_view_main(void) { return m_pCViewMain; }

	// Popup Menu
	bool create_popup_menu(e_popupMenu_t type, bool show);
	void release_popup_menu(void);
	bool show_popup_menu(void);
	bool is_show_popup_menu(void);

	// ---------------------------------------------------------------
};
// ---------------------------------------------------------------



#endif	// __ATP3_H__
