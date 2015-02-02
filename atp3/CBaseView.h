#ifndef __CBASEVIEW_H__
#define __CBASEVIEW_H__

/* --------------------------------------------------------------
Project:	ATP(A Tentative Plan) 3
Purpose:	
Author:		Ho-Jung Kim (godmode2k@hotmail.com)
Date:		Since Dec 2, 2014
Filename:	CBaseView.h

Last modified: Jan 24, 2015
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

// Keyboard & Mouse Event
#include "CKeyEvent.h"
// ---------------------------------------------------------------



//! Definition
// ---------------------------------------------------------------
// Paint Color
#define PAINT_COLOR__DEFAULT		0xff000000 // Black
#define PAINT_COLOR__BLACK			0xff000000
#define PAINT_COLOR__BLUE			0xff0000ff
#define PAINT_COLOR__CYAN			0xff00ffff
#define PAINT_COLOR__DKGRAY			0xff444444
#define PAINT_COLOR__GRAY			0xff888888
#define PAINT_COLOR__GREEN			0xff00ff00
#define PAINT_COLOR__LTGRAY			0xffcccccc
#define PAINT_COLOR__MAGENTA		0xffff00ff
#define PAINT_COLOR__RED			0xffff0000
#define PAINT_COLOR__TRANSPARENT	0x00000000
#define PAINT_COLOR__WHITE			0xffffffff
#define PAINT_COLOR__YELLOW			0xffffff00
#define PAINT_COLOR_A(COLOR)		( (COLOR >> 24) & 0xff )
#define PAINT_COLOR_R(COLOR)		( (COLOR >> 16) & 0xff )
#define PAINT_COLOR_G(COLOR)		( (COLOR >> 8) & 0xff )
#define PAINT_COLOR_B(COLOR)		( COLOR & 0xff )
#define PAINT_COLOR_UINT16_8(COLOR)	( (COLOR >> 8) & 0xff )		// 16bit to 8bit: 0xffff -> 0xff
#define PAINT_COLOR_UINT8_16(COLOR)	( (COLOR << 8) | COLOR )	// 8bit to 16bit: 0xff   -> 0xffff
#define PAINT_COLOR_UINT16_FRACTION_CAIRO(COLOR)	\
	( (float)COLOR / 65535 )	// Cairo color fraction range: 0 to 1
/*
#define PAINT_COLOR_RGBA(COLOR, vR, vG, vB, vA)	do {	\
	##vR = PAINT_COLOR_R( COLOR );			\
	##vG = PAINT_COLOR_G( COLOR );			\
	##vB = PAINT_COLOR_B( COLOR );			\
	if ( vA ) {								\
		##vA = PAINT_COLOR_A( COLOR );		\
	}										\
} while (0)
*/



#ifdef __cplusplus
extern "C" {
#endif

typedef cairo_t canvas_t;
typedef cairo_t* canvas_pt;
typedef GtkWidget drawing_area_t;
typedef GtkWidget* drawing_area_pt;


typedef struct _ColorARGB_st {
	guint16 a;
	guint16 r;
	guint16 g;
	guint16 b;
} ColorARGB_st;

typedef enum _e_objAttachPaintColor_t {
	e_objAttachPaintColor_BLACK = 1,
	e_objAttachPaintColor_BLUE,
	e_objAttachPaintColor_CYAN,
	e_objAttachPaintColor_DKGRAY,
	e_objAttachPaintColor_GRAY,
	e_objAttachPaintColor_GREEN,
	e_objAttachPaintColor_LTGRAY,
	e_objAttachPaintColor_MAGENTA,
	e_objAttachPaintColor_RED,
	//e_objAttachPaintColor_TRANSPARENT,
	e_objAttachPaintColor_WHITE,
	e_objAttachPaintColor_YELLOW
} e_ObjAttachPaintColor_t;

inline void PAINT_COLOR_RGBA_8(e_ObjAttachPaintColor_t _color,
		guint* r, guint* g, guint* b, guint* a = NULL) {
	unsigned int color = PAINT_COLOR__DEFAULT;

	switch ( _color ) {
		case e_objAttachPaintColor_BLACK:
			{ color = PAINT_COLOR__BLACK; } break;
		case e_objAttachPaintColor_BLUE:
			{ color = PAINT_COLOR__BLUE; } break;
		case e_objAttachPaintColor_CYAN:
			{ color = PAINT_COLOR__CYAN; } break;
		case e_objAttachPaintColor_DKGRAY:
			{ color = PAINT_COLOR__DKGRAY; } break;
		case e_objAttachPaintColor_GRAY:
			{ color = PAINT_COLOR__GRAY; } break;
		case e_objAttachPaintColor_GREEN:
			{ color = PAINT_COLOR__GREEN; } break;
		case e_objAttachPaintColor_LTGRAY:
			{ color = PAINT_COLOR__LTGRAY; } break;
		case e_objAttachPaintColor_MAGENTA:
			{ color = PAINT_COLOR__MAGENTA; } break;
		case e_objAttachPaintColor_RED:
			{ color = PAINT_COLOR__RED; } break;
		//case e_objAttachPaintColor_TRANSPARENT:
		//	{ color = PAINT_COLOR__TRANSPARENT; } break;
		case e_objAttachPaintColor_WHITE:
			{ color = PAINT_COLOR__WHITE; } break;
		case e_objAttachPaintColor_YELLOW:
			{ color = PAINT_COLOR__YELLOW; } break;
	}

	*r = PAINT_COLOR_R( color );
	*g = PAINT_COLOR_G( color );
	*b = PAINT_COLOR_B( color );

	if ( a ) {
		*a = PAINT_COLOR_A( color );
	}
}

inline void PAINT_COLOR_RGBA_16(e_ObjAttachPaintColor_t _color,
		guint16* r, guint16* g, guint16* b, guint16* a = NULL) {
	unsigned int color = PAINT_COLOR__DEFAULT;

	switch ( _color ) {
		case e_objAttachPaintColor_BLACK:
			{ color = PAINT_COLOR__BLACK; } break;
		case e_objAttachPaintColor_BLUE:
			{ color = PAINT_COLOR__BLUE; } break;
		case e_objAttachPaintColor_CYAN:
			{ color = PAINT_COLOR__CYAN; } break;
		case e_objAttachPaintColor_DKGRAY:
			{ color = PAINT_COLOR__DKGRAY; } break;
		case e_objAttachPaintColor_GRAY:
			{ color = PAINT_COLOR__GRAY; } break;
		case e_objAttachPaintColor_GREEN:
			{ color = PAINT_COLOR__GREEN; } break;
		case e_objAttachPaintColor_LTGRAY:
			{ color = PAINT_COLOR__LTGRAY; } break;
		case e_objAttachPaintColor_MAGENTA:
			{ color = PAINT_COLOR__MAGENTA; } break;
		case e_objAttachPaintColor_RED:
			{ color = PAINT_COLOR__RED; } break;
		//case e_objAttachPaintColor_TRANSPARENT:
		//	{ color = PAINT_COLOR__TRANSPARENT; } break;
		case e_objAttachPaintColor_WHITE:
			{ color = PAINT_COLOR__WHITE; } break;
		case e_objAttachPaintColor_YELLOW:
			{ color = PAINT_COLOR__YELLOW; } break;
	}

	*r = PAINT_COLOR_UINT8_16( PAINT_COLOR_R(color) );
	*g = PAINT_COLOR_UINT8_16( PAINT_COLOR_G(color) );
	*b = PAINT_COLOR_UINT8_16( PAINT_COLOR_B(color) );

	if ( a ) {
		*a = PAINT_COLOR_UINT8_16( PAINT_COLOR_A(color) );
	}
}

inline void PAINT_COLOR_SHOW(const ColorARGB_st color) {
	//__LOGT__( TAG, "PAINT_COLOR_SHOW()" );
	
	const char* TAG = "";

	__LOGT__( TAG, "PAINT_COLOR_SHOW(): color(16) = A(%d), R(%d), G(%d), B(%d)",
				color.a, color.r, color.g, color.b );

	__LOGT__( TAG, "PAINT_COLOR_SHOW(): color(8) = A(%d), R(%d), G(%d), B(%d)",
				PAINT_COLOR_UINT16_8(color.a),
				PAINT_COLOR_UINT16_8(color.r),
				PAINT_COLOR_UINT16_8(color.g),
				PAINT_COLOR_UINT16_8(color.b) );

	__LOGT__( TAG, "PAINT_COLOR_SHOW(): FRACTION value (0 to 1): A(%f), R(%f), G(%f), B(%f)",
				PAINT_COLOR_UINT16_FRACTION_CAIRO(color.a),
				PAINT_COLOR_UINT16_FRACTION_CAIRO(color.r),
				PAINT_COLOR_UINT16_FRACTION_CAIRO(color.g),
				PAINT_COLOR_UINT16_FRACTION_CAIRO(color.b) );
}

typedef enum _e_popupMenu_t {
	e_popupMenu_UNKNOWN = 0,
	e_popupMenu_ATTACHMENT_OBJ
} e_popupMenu_t;

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

// Main class
class CBaseView {
private:
	const char* TAG;

	//GError* m_pError;
	//Widgets_st* m_pWidgets;
	
	canvas_t* m_canvas;
	drawing_area_t* m_drawing_area;
	bool m_rgba;

	int m_width, m_height;
	int m_display_width, m_display_height;
protected:
public:
	// Ctor/Dtor
	explicit CBaseView(void);
	virtual ~CBaseView();

	void __init(void);
	void __release(void);

	// ---------------------------------------------------------------

	// Initialize
	//virtual bool init_widget_all(Widgets_st** pWidgets);
	//virtual bool init_ui_with_callback(void);
	
	virtual const char* get_tag(void) { return TAG; }

	// ---------------------------------------------------------------
	
	virtual void set_canvas(canvas_t* canvas);
	virtual void set_canvas_ref(canvas_t*& canvas);
	virtual canvas_t* get_canvas(void) { return m_canvas; }
	virtual void set_top_view(drawing_area_t* drawing_area);
	virtual void set_top_view_ref(drawing_area_t*& drawing_area);
	virtual drawing_area_t* get_top_view(void) { return m_drawing_area; }

	// Widget: width, height
	virtual void set_width_height(int w, int h) { m_width = w; m_height = h; }
	virtual void set_width(int val) { m_width = val; }
	virtual int get_width(void) { return m_width; }
	virtual void set_height(int val) { m_height = val; }
	virtual int get_height(void) { return m_height; }
	virtual void set_rgba(bool val) { m_rgba = val; }
	virtual bool get_rgba(void) { return m_rgba; }

	// Display: width, height
	virtual void set_display_width_height(int w, int h) { m_display_width = w; m_display_height = h; }
	virtual void set_display_width(int val) { m_display_width = val; }
	virtual int get_display_width(void) { return m_display_width; }
	virtual void set_display_height(int val) { m_display_height = val; }
	virtual int get_display_height(void) { return m_display_height; }

	// Draw
	virtual void _draw(void);
	virtual void invalidate(CBaseView* view = NULL);
	//virtual void invalidate_rect(float left, float top, float right, float bottom);
	virtual void draw(CBaseView* view) = 0;
	
	// Touch Event
	virtual bool onTouchEvent(CKeyEvent* event);
	virtual void onTouchEventDown(CKeyEvent* event, float x, float y);
	virtual void onTouchEventUp(CKeyEvent* event, float x, float y);
	virtual void onTouchEventMove(CKeyEvent* event, float x, float y);

	// ---------------------------------------------------------------
private:
	virtual void _invalidate(void);
	// Draw
	virtual void onDraw(CBaseView* view) = 0;
};
// ---------------------------------------------------------------



#endif	// __CBASEVIEW_H__
