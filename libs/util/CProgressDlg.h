#ifndef __CPROGRESSDLG_H__
#define __CPROGRESSDLG_H__

/* --------------------------------------------------------------
Project:	CProgressDlg
Purpose:	Progress Dialog
Author:		Ho-Jung Kim (godmode2k@hotmail.com)
Date:		Since Nov 28, 2014
Filename:	CProgressDlg.h

Last modified: Jan 20, 2015
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
	$ g++ ... -D__LINUX__ -D_REENTRANT -lpthread -lrt	\
		`pkg-config --cflags --libs gtk+-2.0 libglade-2.0`
		or
		`pkg-config --cflags --libs gtk+-3.0 libglade-2.0`

2.
	perform the task: SEE 'CProgressDlg::task_in_thread()'

	Doesn't matter the following instance
	because CProgress_dlg used gtk_dialog_run().

	{
		CProgressDlg dlg;
		dlg.show( (GtkWindow*)((Widgets_st*)user_data)->pWindow, NULL );
	}
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
#include "util/CUtil_DEF.h"
#include "util/CUtil.h"
#include "util/CThreadTask.h"
#include "util/CMutexSemaphore.h"

#include "util/CDialog.h"


// Cutter
//#include "./3rd_party/cutter-1.03/cutter.h"
// ---------------------------------------------------------------



//! Definition
// ---------------------------------------------------------------
//#define DEFAULT_STR_MAX_FILENAME	255
//#define DEFAULT_STR_UI_GLADE_FILE	"ui.glade"

#define DEFAULT_STR_TITLE			"In progressing..."
#define DEFAULT_STR_MSG				""
#define DEFAULT_STR_PROGRESS		"0% (0/100)"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum _e_ShowProgress_t {
	e_showProgress_ALL = 0,
	e_showProgress_PROGRESSBAR,
	e_showProgress_NO_PROGRESSBAR
} e_ShowProgress_t;

#ifdef __cplusplus
}
#endif

//class CEventData;
// ---------------------------------------------------------------



//! Prototype
// ---------------------------------------------------------------
#if 0
namespace g_Func {
	#define TAG__g_Func		"g_Func"

	//void main_window_quit(GtkWidget* widget, gpointer user_data);
} // namespace g_Func

namespace g_FuncSignalHandler {
	#define TAG__g_FuncSignalHandler		"g_FuncSignalHandler"

	// Menu Item: Button
	//void on_button_xxxx_clicked(GtkWidget* widget, gpointer user_data);
} // namespace g_FuncSignalHandler
#endif

// ---------------------------------------------------------------



//! Class
// ---------------------------------------------------------------

// Event
/*
class CEventData {
private:
protected:
public:
	// Ctor/Dtor
	explicit CEventData(void);
	virtual ~CEventData();
};
*/

// Main class
class CProgressDlg : public CThreadTask {
private:
	const char* TAG;

	//GError* m_pError;
	GtkWidget* m_pSpinner_dlg;
	GtkSpinner* m_pSpinner;
	GtkWidget* m_pLabel;
	GtkWidget* m_pLabel_progress;
	GtkWidget* m_pProgressBar;
	GtkWidget* m_pHBox_spinner_progress;
	GtkWidget* m_pVBox;

	char* m_title;
	char* m_msg;
	char* m_progress;
	int m_progress_total;
	int m_progress_pos;
	int m_width;
	int m_height;
	bool m_resizable;

	bool m_working;
	bool m_updated;

	// ---------------------------------------------------------------
	
	void updates(void);
	void set_working(bool val) { m_working = val; }
	void wait_for_thread_finished(void);
	void __stop(void);
protected:
public:
	// Ctor/Dtor
	explicit CProgressDlg(void);
	virtual ~CProgressDlg();

	void __init(void);
	void __release(void);

	// ---------------------------------------------------------------

	void set_updated(bool val = true);
	bool get_updated(void);
	bool get_working(void);

	void set_width_height(const int width, const int height);
	int get_width(void);
	int get_height(void);
	bool get_resizable(void);
	void set_title(const char* title = NULL, const bool resizable = false);
	const char* get_title(void);
	void set_message(const char* msg = NULL);
	const char* get_message(void);
	void set_progress_msg(const char* progress = NULL);
	const char* get_progress_msg(void);
	int set_progress(int pos, int total);
	int get_progress(void);
	double get_progress_fraction(void);

	virtual gint show(GtkWindow* pWindow, gpointer user_data = NULL,
			const char* msg = NULL, const e_ShowProgress_t show_type = e_showProgress_PROGRESSBAR);
	virtual void task_in_thread(void) = 0;
	void destroy_ui(void);
	void stop(void);

	// ---------------------------------------------------------------
	
	// Thread
	void* inBackground(std::vector<void*>* pvecVal = NULL);
	void progressUpdate(void* pVal);
	void postExecute(void* pResult, void* pExtraVal = NULL);
};
// ---------------------------------------------------------------



#endif	// __CPROGRESSDLG_H__
