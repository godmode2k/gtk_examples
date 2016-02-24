#ifndef __CKEYEVENT_H__
#define __CKEYEVENT_H__

/* --------------------------------------------------------------
Project:	ATP(A Tentative Plan) 3
Purpose:	
Author:		Ho-Jung Kim (godmode2k@hotmail.com)
Date:		Since Dec 2, 2014
Filename:	CKeyEvent.h

Last modified: Dec 30, 2014
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
#include <cstdio>		//<stdio.h>
#include <cstring>		//<string.h>

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
// ---------------------------------------------------------------



//! Definition
// ---------------------------------------------------------------
//#define DEFAULT_STR_MAX_FILENAME	255
//#define DEFAULT_STR_UI_GLADE_FILE	"ui.glade"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum _e_eventType_t {
	e_eventType_UNKNOWN = -1,
	e_eventType_RESERVED = 0,

	// Keyboard
	e_eventType_KEY_INPUT = 1,

	// Mouse
	e_eventType_MOUSE_INPUT
} e_EventType_t;

typedef enum _e_eventAction_t {
	e_eventAction_UNKNOWN = -1,
	e_eventAction_RESERVED = 0,

	// Keyboard
	e_eventAction_key_pressed_down,
	e_eventAction_key_ctrl,
	e_eventAction_key_shift,

	// Mouse
	e_eventAction_mouse_pressed,
	e_eventAction_mouse_released,
	e_eventAction_mouse_move,
	e_eventAction_mouse_wbtn_pressed,
	e_eventAction_mouse_wheel_up,
	e_eventAction_mouse_wheel_down
} e_EventAction_t;

#ifdef __cplusplus
}
#endif
// ---------------------------------------------------------------



//! Prototype
// ---------------------------------------------------------------
/*
namespace g_Func {
	#define TAG__g_Func		"g_Func"
} // namespace g_Func

namespace g_FuncSignalHandler {
	#define TAG__g_FuncSignalHandler		"g_FuncSignalHandler"

	// Menu Item: Button
	//void on_button_open_clicked(GtkWidget* widget, gpointer user_data);
} // namespace g_FuncSignalHandler
*/

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


// Main class
class CKeyEvent {
private:
	const char* TAG;

	//GError* m_pError;
	//Widgets_st* m_pWidgets;


	e_EventType_t m_event_type;
	e_EventAction_t m_event_action;

	int m_key_int;
	char m_key_char;
	bool m_key_pressed_down;
	bool m_key_ctrl;
	bool m_key_shift;

	bool m_mouse_lbtn_pressed;
	bool m_mouse_rbtn_pressed;
	bool m_mouse_released;
	bool m_mouse_wbtn_pressed;
	bool m_mouse_wheel_up;
	bool m_mouse_wheel_down;
	float m_mouse_x, m_mouse_y;
protected:
public:
	// Ctor/Dtor
	explicit CKeyEvent(void);
	virtual ~CKeyEvent();

	void __init(void);
	void __release(void);

	// ---------------------------------------------------------------

	// Initialize
	//bool init_widget_all(Widgets_st** pWidgets);
	//bool init_ui_with_callback(void);
	
	//virtual void key(GdkEvent* event, bool pressed_down, GtkWidget* widget = NULL, CViewMain* view = NULL);
	//virtual void mouse(GdkEvent* event, GtkWidget* widget = NULL, CViewMain* view = NULL);
	virtual void key(GdkEvent* event, bool pressed_down, GtkWidget* widget = NULL);
	virtual void mouse(GdkEvent* event, GtkWidget* widget = NULL);
	virtual e_EventType_t get_type(void) { return m_event_type; }
	virtual e_EventAction_t get_action(void) { return m_event_action; }
	virtual float get_mouse_x(void) { return m_mouse_x; }
	virtual float get_mouse_y(void) { return m_mouse_y; }
	virtual bool is_mouse_lbtn(void) { return m_mouse_lbtn_pressed; }
	virtual bool is_mouse_rbtn(void) { return m_mouse_rbtn_pressed; }
	virtual bool is_mouse_wbtn(void) { return m_mouse_wbtn_pressed; }

	// ---------------------------------------------------------------
};
// ---------------------------------------------------------------



#endif	// __CKEYEVENT_H__
