#ifndef __CANIMTASK_H__
#define __CANIMTASK_H__

/* --------------------------------------------------------------
Project:	CAnimTask
Purpose:	Animation Task
Author:		Ho-Jung Kim (godmode2k@hotmail.com)
Date:		Since Mar 21, 2015
Filename:	CAnimTask.h

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

2.
	perform the task: SEE 'CAnimTask::task_in_thread()'

	//Don't use the following instance for local scope.
	//This isn't used gtk_dialog_run(), use this for global scope.
	//
	//{
	//	//CAnimTask anim;
	//	//anim.show( (GtkWindow*)((Widgets_st*)user_data)->pWindow, NULL, "hello~" );
	//}
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
//#define DEFAULT_STR_MAX_FILENAME		255
//#define DEFAULT_STR_UI_GLADE_FILE		"ui.glade"

#define DEFAULT_STR_TITLE				"In progressing..."
#define DEFAULT_STR_MSG					""

#define DEFAULT_TOAST_RECT_PADDING				10
#define DEFAULT_TOAST_WINDOW_BOTTOM_PADDING		10
//#define DEFAULT_TOAST_RECT_HEIGHT				40
#define DEFAULT_TOAST_WINDOW_OPAQUE				0.7f	// 0 ~ 1
#define DEFAULT_TOAST_WINDOW_BOTTOM_POS(window_height, client_height)	\
	((window_height - client_height) - DEFAULT_TOAST_WINDOW_BOTTOM_PADDING)
#define DEFAULT_TOAST_WINDOW_WIDTH_CENTER_POS(window_width, client_width)	\
	((window_width - client_width) >> 1)
#define DEFAULT_TOAST_WINDOW_HEIGHT_CENTER_POS(window_height, client_height)	\
	((window_height - client_height) >> 1)


#ifdef __cplusplus
extern "C" {
#endif

typedef enum _e_AnimDelay_t {
	// delay (ms)
	e_animDelay_NONE = 0,
	e_animDelay_SHORT = 3000,
	e_animDelay_LONG = 5000
} e_AnimDelay_t;

typedef enum _e_AnimPosition_t {
	e_animPos_TOP = 0,
	e_animPos_CENTER,
	e_animPos_BOTTOM
} e_AnimPos_t;

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

// Main class
class CAnimTask : public CThreadTask {
private:
	const char* TAG;

	//GError* m_pError;
	GtkWidget* m_pMessageDlg;
	GtkWidget* m_pLabel;

	/*
	char* m_title;
	bool m_resizable;
	*/
	int m_width;
	int m_height;

	char* m_msg;
	e_AnimDelay_t m_delay;

	bool m_working;
	bool m_updated;
	bool m_done;

	// ---------------------------------------------------------------
	
	void updates(void);
	void set_working(bool val) { m_working = val; }
	void wait_for_thread_finished(void);
	void __stop();
protected:
public:
	// Ctor/Dtor
	explicit CAnimTask(void);
	virtual ~CAnimTask();

	void __init(void);
	void __release(void);

	// ---------------------------------------------------------------

	void set_updated(bool val = true) { m_updated = val; }
	bool get_updated(void) { return m_updated; }
	bool get_working(void) { return m_working; }
	void set_done(bool val) { m_done = val; }
	bool get_done(void) { return m_done; }

	/*
	void set_title(const char* title = NULL, const bool resizable = false) {
		m_title = (char*)title; m_resizable = resizable;
	}
	const char* get_title(void) { return m_title; }
	bool get_resizable(void) { return m_resizable; }
	*/
	void set_width_height(const int width, const int height) {
		m_width = width; m_height = height;
	}
	void get_width_height(int* width, int* height) {
		*width = m_width; *height = m_height;
	}
	int get_width(void) { return m_width; }
	int get_height(void) { return m_height; }

	void set_message(const char* msg = NULL) { m_msg = (char*)msg; }
	const char* get_message(void) { return m_msg; }
	void set_delay(e_AnimDelay_t delay = e_animDelay_SHORT);
	//e_AnimDelay_t get_delay(void) { return m_delay; }
	int get_delay(void) { return m_delay; }

	virtual bool show(void* user_data = NULL);
#if 0
	virtual bool show(GtkWindow* pWindow, gpointer user_data = NULL,
			const char* msg = NULL, e_AnimDelay_t delay = e_animDelay_SHORT,
			e_AnimPos_t pos = e_animPos_CENTER);
#endif
	virtual void task_in_thread(void* user_data = NULL);
	void destroy_ui(void);
	void stop(void);

	// ---------------------------------------------------------------
	
	// Thread
	void* inBackground(std::vector<void*>* pvecVal = NULL);
	void progressUpdate(void* pVal);
	void postExecute(void* pResult, void* pExtraVal = NULL);
};
// ---------------------------------------------------------------



#endif	// __CANIMTASK_H__
