#ifndef __CVIEWMAIN_H__
#define __CVIEWMAIN_H__

/* --------------------------------------------------------------
Project:	ATP(A Tentative Plan) 3
Purpose:	
Author:		Ho-Jung Kim (godmode2k@hotmail.com)
Date:		Since Dec 2, 2014
Filename:	CViewMain.h

Last modified: Feb 7, 2015
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
#include <cstdio>		// <stdio.h>
#include <cstring>		// <string.h>

#ifdef __LINUX__
	#include <sys/stat.h>	// stat
	#include <pwd.h>		// struct passwd
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

// View
#include "CBaseView.h"
#include "CViewAttach.h"

// Keyboard & Mouse Event
#include "CKeyEvent.h"
// ---------------------------------------------------------------



//! Definition
// ---------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif

#define TMP_OBJ_FONT_INFO__FONT_FAMILY_MAX_SIZE		255
#define TAKE_SCREENSHOT__PATHNAME_MAX_SIZE			255

typedef enum _e_objType_t {
	e_objType_UNKNOWN = -1,
	e_objType_RESERVED = 0,

	e_objType_MENU = 1
} e_ObjType_t;

typedef enum _e_attachActionType_t {
	e_attachActionType_ADD = 0,
	e_attachActionType_EDIT
} e_attachActionType_t;

typedef struct _obj_rect_list_st {
	e_ObjType_t type;
	GdkRectangle rect;
} __PACK__ obj_rect_list_st;

typedef struct _tmp_obj_font_info_st {
	bool selected_font;
	bool selected_color;
	bool selected_bg_img;
	char family_name[TMP_OBJ_FONT_INFO__FONT_FAMILY_MAX_SIZE];
	e_ObjAttachFontTypeface_t typeface;
	bool bold;
	double size;
	ColorARGB_st color;
	char* bg_img;
} __PACK__ tmp_obj_font_info_st;

typedef struct _tmp_obj_bg_img_info_st {
	bool selected_bg_img;
	char* bg_img_filename;
} __PACK__ tmp_obj_bg_img_info_st;

#ifdef __cplusplus
}
#endif
// ---------------------------------------------------------------



//! Prototype
// ---------------------------------------------------------------
namespace g_Func {
	#define TAG__g_Func		"g_Func"
} // namespace g_Func

namespace g_FuncSignalHandler {
	#define TAG__g_FuncSignalHandler		"g_FuncSignalHandler"

	// Menu Item: Button
	//void on_button_open_clicked(GtkWidget* widget, gpointer user_data);
} // namespace g_FuncSignalHandler

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
class CViewMain : virtual public CBaseView {
private:
	const char* TAG;

	//GError* m_pError;
	//Widgets_st* m_pWidgets;
	
	//canvas_t* m_canvas;
	int m_width, m_height;
	int m_display_width, m_display_height;

	std::vector<obj_rect_list_st> m_vec_rect;
	std::vector<CViewAttach*>* m_pvec_attach;

	// Attachment Object Action
	e_attachActionType_t m_attach_action;

	// Temporary Font information
	tmp_obj_font_info_st m_tmp_obj_font_info;
	tmp_obj_bg_img_info_st m_tmp_obj_bg_img_info;

	// Take a Screenshot
	bool m_screenshot_region;
	GdkRectangle m_screenshot_rect;
	float m_screenshot_touchX, m_screenshot_touchY;
	e_ObjAttachDirection_t m_screenshot_direction;
	char m_screenshot_pathname[TAKE_SCREENSHOT__PATHNAME_MAX_SIZE];

	//CViewAttach m_attach;		//! TEST
protected:
public:
	// Ctor/Dtor
	explicit CViewMain(void);
	virtual ~CViewMain();

	void __init(void);
	void __release(void);

	// ---------------------------------------------------------------

	// Initialize
	//bool init_widget_all(Widgets_st** pWidgets);
	//bool init_ui_with_callback(void);
	
	virtual const char* get_tag(void) { return TAG; }

	// ---------------------------------------------------------------
	
	//virtual void set_canvas(canvas_t* canvas);
	//virtual canvas_t* get_canvas(void) { return m_canvas; }

	// Widget: width, height
	virtual void set_width_height(int w, int h) { m_width = w; m_height = h; }
	virtual void set_width(int val) { m_width = val; }
	virtual int get_width(void) { return m_width; }
	virtual void set_height(int val) { m_height = val; }
	virtual int get_height(void) { return m_height; }

	// Display: width, height
	virtual void set_display_width_height(int w, int h) { m_display_width = w; m_display_height = h; }
	virtual void set_display_width(int val) { m_display_width = val; }
	virtual int get_display_width(void) { return m_display_width; }
	virtual void set_display_height(int val) { m_display_height = val; }
	virtual int get_display_height(void) { return m_display_height; }

	// Draw
	/*
	virtual void invalidate(void);
	virtual void invalidate(CBaseView* view);
	//virtual void invalidate_rect(float left, float top, float right, float bottom);
	*/
	virtual void draw(CBaseView* view);
	
	// Touch Event
	virtual bool onTouchEvent(CKeyEvent* event);
	virtual void onTouchEventDown(CKeyEvent* event, float x, float y);
	virtual void onTouchEventUp(CKeyEvent* event, float x, float y);
	virtual void onTouchEventMove(CKeyEvent* event, float x, float y);

	// ---------------------------------------------------------------

	// Color
	void draw_paint_color(canvas_t* canvas, double r, double g, double b, double a = 0);
	void draw_paint_color_fraction(canvas_t* canvas,
			guint16 r, guint16 g, guint16 b, guint16 a = 0);
	void draw_paint_color(canvas_t* canvas, bool fraction, ColorARGB_st color);
	void draw_paint_color(canvas_t* canvas, bool fraction,
			e_ObjAttachPaintColor_t color, guint16 a = 0);

	// Figure
	void draw_circle(canvas_t* canvas, float x, float y, double radius, bool fill = false);

	// ---------------------------------------------------------------
	
	void take_screenshot_get_str_direction(e_ObjAttachDirection_t direction);
	e_ObjAttachDirection_t take_screenshot_get_selected_direction(GdkRectangle rect, float x, float y);
	void take_screenshot_update_position(e_ObjAttachDirection_t direction, float x, float y);
	bool take_screenshot_get_region(void);
	void take_screenshot_set_region(bool set_region, bool init = true);
	void take_screenshot_region(canvas_t* canvas);
	const char* take_screenshot_get_pathname(void);
	bool take_screenshot_save(void);
	bool take_screenshot_save(int x, int y, int width, int height,
		const char* filename, const e_takeScreenshot_t format = e_takeScreenshot_PNG);

	// ---------------------------------------------------------------
	
	void add_obj_rect(obj_rect_list_st obj);
	void add_obj_rect(e_ObjType_t type, GdkRectangle rect);
	void set_obj_rect(e_ObjType_t type, GdkRectangle rect);
	void set_obj_rect(GdkRectangle& rect, float x, float y);
	void set_obj_rect(GdkRectangle& rect, float x, float y, float w, float h);
	void get_obj_rect(e_ObjType_t type);
	void del_obj_rect(e_ObjType_t type, float x, float y);
	bool is_obj_selected(GdkRectangle rect, float x, float y);
	e_ObjType_t get_obj_type(float x, float y);

	// Temporary Font information
	void set_tmp_font_info_select(bool select);
	void set_tmp_font_info(const char* family_name, const e_ObjAttachFontTypeface_t typeface,
			const bool bold, const double size);
	void set_tmp_font_info_color(const ColorARGB_st color);
	void set_tmp_font_info_reset(void);
	tmp_obj_font_info_st* get_tmp_font_info(void) { return &m_tmp_obj_font_info; }
	void set_tmp_bg_img_info_select(bool select);
	void set_tmp_bg_img_info(const char* bg_img_filename);
	void set_tmp_bg_img_info_release(void);
	tmp_obj_bg_img_info_st* get_tmp_bg_img_info(void) { return &m_tmp_obj_bg_img_info; }

	// Attachment
	bool attach_add_text(const char* text, const char* bg_img_filename = NULL,
			const char* tag = NULL);
	bool attach_edit_text(const char* text, const char* bg_img_filename = NULL,
			const char* tag = NULL);
	bool attach_add_image(const char* img_filename, const char* tag = NULL);
	bool attach_rotate(void);
	bool attach_delete(void);
	bool attach_delete_all(void);
	CViewAttach* attach_selected(void);
	void attach_set_action(e_attachActionType_t type);
	e_attachActionType_t attach_get_action(void);
	bool attach_invalidate(CBaseView* view);
	bool attach_touch_event(CKeyEvent* event);
	void attach_bring_to_front(void);
private:
	// Draw
	virtual void onDraw(CBaseView* view);
};
// ---------------------------------------------------------------



#endif	// __CVIEWMAIN_H__
