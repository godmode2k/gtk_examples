#ifndef __CVIEWATTACH_H__
#define __CVIEWATTACH_H__

/* --------------------------------------------------------------
Project:	ATP(A Tentative Plan) 3
Purpose:	
Author:		Ho-Jung Kim (godmode2k@hotmail.com)
Date:		Since Dec 2, 2014
Filename:	CViewAttach.h

Last modified: Jan 21, 2016
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
#include <vector>
#include <cmath>		//<math.h>

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

// Keyboard & Mouse Event
#include "CKeyEvent.h"
// ---------------------------------------------------------------



//! Definition
// ---------------------------------------------------------------
//#define DEFAULT_DRAW_CIRCLE_RADIUS				8

// Text
#define DEFAULT_TEXT_FONT_FAMILY				"Sans"
#define DEFAULT_TEXT_FONT_FAMILY_SIZE			24
#define DEFAULT_TEXT_FONT_FAMILY_SIZE_ALL		128
#define DEFAULT_TEXT_FONT_SIZE					10.0f
#define DEFAULT_TEXT_GLYPH_WHITESPACE_INDEX		3
#define DEFAULT_TEXT_GLYPH_WHITESPACE_WIDTH		10
#define DEFAULT_TEXT_GLYPH_WHITESPACE_WIDTH_NEW(val)	\
	(((val / 2) > 0)? (val / 2) : DEFAULT_TEXT_GLYPH_WHITESPACE_WIDTH)
#define DEFAULT_TEXT_PANGO_SCALE_BIT			10	// PANGO_SCALE(1024)

// Rotation
#define DEFAULT_ROTATION_UI_SLIDER_WIDTH			180
#define DEFAULT_ROTATION_UI_SLIDER_HEIGHT			4
#define DEFAULT_ROTATION_UI_SLIDER_HEIGHT_PADDING	30
#define DEFAULT_ROTATION_UI_SLIDER_POS_ADJUST		0.f
#define DEFAULT_ROTATION_UI_SLIDER_STR_N180			"-180"
#define DEFAULT_ROTATION_UI_SLIDER_STR_P180			"+180"
#define DEFAULT_ROTATION_UI_SLIDER_STR_0			"0"
#define DEFAULT_ROTATION_UI_SLIDER_STR_SIZE			4

// Patchers IO
#define DEFAULT_PATCHERS_IO_CIRCLE_RADIUS			5
#define DEFAULT_PATCHERS_IO_PADDING_WIDTH			10
#define DEFAULT_PATCHERS_IO_PADDING_HEIGHT			13
#define DEFAULT_PATCHERS_IO_NUM_OF_INOUT			1
#define DEFAULT_PATCHERS_IO_NUM_OF_INOUT_LINKAGE	4



#ifdef __cplusplus
extern "C" {
#endif

typedef enum _e_objAttachType_t {
	e_objAttachType_UNKNOWN = -1,
	e_objAttachType_TEXT = 0,
	e_objAttachType_IMAGE
} e_ObjAttachType_t;

typedef enum _e_objAttachFontTypeface_t {
	//e_objAttachFontTypeface_UNKNOWN = -1,
	e_objAttachFontTypeface_NORMAL = 0,
	//e_objAttachFontTypeface_BOLD,
	e_objAttachFontTypeface_ITALIC,
	//e_objAttachFontTypeface_UNDERLINE,
	e_objAttachFontTypeface_OBLIQUE
} e_ObjAttachFontTypeface_t;

/*
// CBaseView.h
// Attached objact direction
typedef enum _e_objAttachDirection_t {
	e_objAttachDirection_UNKNOWN = -1,
	e_objAttachDirection_RESERVED = 0,

	e_objAttachDirection_LEFT = 1,
	e_objAttachDirection_TOP,
	e_objAttachDirection_RIGHT,
	e_objAttachDirection_BOTTOM,
	e_objAttachDirection_LEFT_TOP,
	e_objAttachDirection_RIGHT_TOP,
	e_objAttachDirection_LEFT_BOTTOM,
	e_objAttachDirection_RIGHT_BOTTOM,
	e_objAttachDirection_CENTER,
	e_objAttachDirection_LEFT_CENTER,
	e_objAttachDirection_TOP_CENTER,
	e_objAttachDirection_RIGHT_CENTER,
	e_objAttachDirection_BOTTOM_CENTER,
	e_objAttachDirection_ROTATE_SLIDEBAR_REGION,

	// Patchers IO
	e_objAttachDirection_PATCHERS_IO_INPUT,
	e_objAttachDirection_PATCHERS_IO_OUTPUT
} e_ObjAttachDirection_t;
*/


// Patchers IO
typedef struct _objAttachPatchersIO_XY_st {
	float x, y;
} __PACK__ objAttachPatchersIO_XY_st;
typedef struct _objAttachPatchersIO_st {
	char uuid[g_INT_MAX_UUID_SIZE];
	unsigned short idx;
	bool selected;
	bool linked_input, linked_output;
	bool drawn_input, drawn_output;
	ColorARGB_st color_link_line;

	objAttachPatchersIO_XY_st input_point;
	objAttachPatchersIO_XY_st output_point;
	int input_val1, input_val2, input_val3;
	int output_val1, output_val2, output_val3;
	//
	// link lines: src output -> dst input
	float input_x, input_y; 	// currently position
	float output_x, output_y;	// currently position
	//
	char ln_pio_input_uuid[g_INT_MAX_UUID_SIZE];
	_objAttachPatchersIO_st* ln_pio_input;	// linking patchers io; src output -> dst input
	//_objAttachPatchersIO_st* ln_pio_output;	// linking patchers io; src input -> dst output
	//
	//_objAttachPatchersIO_st* ln_pio_input[DEFAULT_PATCHERS_IO_NUM_OF_INOUT_LINKAGE];
	_objAttachPatchersIO_st* ln_pio_output[DEFAULT_PATCHERS_IO_NUM_OF_INOUT_LINKAGE];
	int ln_pio_input_registered_idx;		// default: -1
	//int ln_pio_output_registered_idx;		// default: -1

	void (*fn)(void* arg1, void* arg2, void* arg3);
	//std::function<void (void*, void*, void*)> fn;
	//fn = [](void* arg1, void* arg2, void* arg3)->void {
	//};
	

	// Ctor
	//_objAttachPatchersIO_st() : fn(NULL) { }
} __PACK__ objAttachPatchersIO_st;
#define PATCHERS_IO_ADD_OUT_TO_IN(dst_pio_input, src_out) \
	[this](_objAttachPatchersIO_st* dst_pio_input, _objAttachPatchersIO_st* src_out)->bool { \
		bool ret = false; \
		if ( dst_pio_input && src_out ) { \
			if ( !dst_pio_input->ln_pio_output ) return false; \
			for ( int i = 0; i < DEFAULT_PATCHERS_IO_NUM_OF_INOUT_LINKAGE; i++ ) { \
				if ( !dst_pio_input->ln_pio_output[i] ) { \
					dst_pio_input->ln_pio_output[i] = src_out; \
					src_out->ln_pio_input_registered_idx = i; \
					snprintf( src_out->ln_pio_input_uuid, sizeof(src_out->ln_pio_input_uuid), "%s", \
								dst_pio_input->uuid ); \
					ret = true; \
					__LOGT__( TAG, "PATCHERS_IO_ADD_OUT_TO_IN(): added pos = %d\n", i ); \
					break; \
				} \
			} \
		} \
		return ret; \
	} ( dst_pio_input, src_out );
#define PATCHERS_IO_DEL_OUT_FROM_IN(dst_pio_input, src_out) \
	[this](_objAttachPatchersIO_st* dst_pio_input, _objAttachPatchersIO_st* src_out)->bool { \
		bool ret = false; \
		if ( dst_pio_input && src_out ) { \
			const int idx = src_out->ln_pio_input_registered_idx; \
			if ( !dst_pio_input->ln_pio_output ) return false; \
			if ( (idx < 0) || (idx > DEFAULT_PATCHERS_IO_NUM_OF_INOUT_LINKAGE) ) return false; \
			if ( dst_pio_input->ln_pio_output[idx] == src_out) { \
				dst_pio_input->ln_pio_output[idx] = NULL; \
				src_out->ln_pio_input_registered_idx = -1; \
				src_out->ln_pio_input = NULL; \
				memset( src_out->ln_pio_input_uuid, 0x00, sizeof(src_out->ln_pio_input_uuid) ); \
				ret = true; \
				__LOGT__( TAG, "PATCHERS_IO_DEL_OUT_FROM_IN(): deleted pos = %d\n", idx ); \
			} \
		} \
		return ret; \
	} ( dst_pio_input, src_out );
/*
#define PATCHERS_IO_DEL_OUT_FROM_IN_BY_POS(_ret, dst_pio_input, pos) \
	[this](_objAttachPatchersIO_st* dst_pio_input, _objAttachPatchersIO_st* src_out)->bool { \
		bool ret = false; \
		if ( dst_pio_input ) { \
			if ( pos < 0 || (pos > DEFAULT_PATCHERS_IO_NUM_OF_INOUT_LINKAGE) ) return false; \
			dst_pio_input->ln_pio_output[pos] = NULL; \
			src_out->ln_pio_input_registered_idx = -1; \
			src_out->ln_pio_input = NULL; \
			memset( src_out->ln_pio_input_uuid, 0x00, sizeof(src_out->ln_pio_input_uuid) ); \
			ret = true; \
			__LOGT__( TAG, "PATCHERS_IO_DEL_OUT_FROM_IN_BY_POS(): deleted pos = %d\n", i ); \
		} \
		return ret; \
	} ( dst_pio_input, pos );
*/
#define PATCHERS_IO_DEL_OUT_FROM_IN_ALL(dst_pio_input) \
	[this](_objAttachPatchersIO_st* dst_pio_input, _objAttachPatchersIO_st* src_out)->bool { \
		bool ret = false; \
		if ( dst_pio_input ) { \
			if ( !dst_pio_input->ln_pio_output ) return false; \
			for ( int i = 0; i < DEFAULT_PATCHERS_IO_NUM_OF_INOUT_LINKAGE; i++ ) { \
				if ( dst_pio_input->ln_pio_output[i] ) { \
					dst_pio_input->ln_pio_output[i]->ln_pio_input = NULL; \
					dst_pio_input->ln_pio_output[i]->ln_pio_input_registered_idx = -1; \
					memset( dst_pio_input->ln_pio_output[i]->ln_pio_input_uuid, 0x00, \
							sizeof(dst_pio_input->ln_pio_output[i]->ln_pio_input_uuid) ); \
					dst_pio_input->ln_pio_output[i] = NULL; \
					ret = true; \
					__LOGT__( TAG, "PATCHERS_IO_ADD_OUT_TO_IN(): added pos = %d\n", i ); \
					break; \
				} \
			} \
		} \
		return ret; \
	} ( dst_pio_input, src_out );



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
class CViewAttach : virtual public CBaseView {
private:
	const char* TAG;
	char* TAG2;
	char m_uuid[g_INT_MAX_UUID_SIZE];

	//canvas_t* m_canvas;
	int m_width;
	int m_height;

	e_ObjAttachType_t m_obj_type;
	bool m_selected;

	// Rectangle
	GdkRectangle m_rect;

	// Touch event
	CKeyEvent* m_touch_event;
	float m_touchX, m_touchY;
	e_ObjAttachDirection_t m_direction;

	char* m_text;
	double m_text_font_size;
	ColorARGB_st m_text_font_color;
	char* m_text_font_face;
	e_ObjAttachFontTypeface_t m_text_font_typeface;
	bool m_text_font_bold;
	cairo_surface_t* m_image;
	bool m_text_show_boundary;

	// Rotation
	bool m_obj_rotate;
	double m_obj_rotate_degree;
	bool m_obj_rotate_degree_positive;
	int m_obj_rotate_slidebar_pos;		// relative position
	GdkRectangle m_obj_rotate_slide_rect;
	GdkRectangle m_obj_rotate_slidebar_rect;
	float m_obj_rotate_slide_touchX, m_obj_rotate_slide_touchY;

	// Patchers IO
	std::vector<objAttachPatchersIO_st*>* m_pvec_patchers_io;
	std::vector<CViewAttach*>* m_pvec_attach;
	bool m_show_patchers_io;
protected:
public:
	// Ctor/Dtor
	explicit CViewAttach(void);
	virtual ~CViewAttach();

	void __init(void);
	void __release(void);

	// ---------------------------------------------------------------

	// Initialize
	//bool init_widget_all(Widgets_st** pWidgets);
	//bool init_ui_with_callback(void);
	
	virtual const char* get_tag(void) { return TAG; }
	void set_tag2(const char* tag) { TAG2 = (char*)tag; }
	const char* get_tag2(void) { return TAG2; }
	const char* get_uuid_str(void) { return m_uuid; }

	// ---------------------------------------------------------------
	
	//virtual void set_canvas(canvas_t* canvas);
	//virtual canvas_t* get_canvas(void) { return m_canvas; }
	
	// Widget: width, height
	virtual void set_width_height(int w, int h) { m_width = w; m_height = h; }
	virtual void set_width(int val) { m_width = val; }
	virtual int get_width(void) { return m_width; }
	virtual void set_height(int val) { m_height = val; }
	virtual int get_height(void) { return m_height; }

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
	void onTouchEvent_set_event(CKeyEvent* event) { m_touch_event = event; }
	CKeyEvent* onTouchEvent_get_event(void) { return m_touch_event; }

	// ---------------------------------------------------------------
	
	void set_obj_type(e_ObjAttachType_t type);
	e_ObjAttachType_t get_obj_type(void);
	void set_select(bool select);
	bool get_selected(void);
	void set_obj_rect(GdkRectangle& rect, float x, float y);
	void set_obj_rect(GdkRectangle& rect, float x, float y, float w, float h);
	void set_obj_rect(float x, float y, float w, float h);
	bool is_obj_selected(GdkRectangle rect, float x, float y);
	void get_str_direction(e_ObjAttachDirection_t direction);
	e_ObjAttachDirection_t is_obj_selected_direction(GdkRectangle rect, float x, float y);
	e_ObjAttachDirection_t get_selected_direction(void);
	void update_position(e_ObjAttachDirection_t direction, float x, float y);
	GdkRectangle get_rect(void);

	// Color
	void draw_paint_color(canvas_t* canvas, double r, double g, double b, double a = 0);
	void draw_paint_color_fraction(canvas_t* canvas,
			guint16 r, guint16 g, guint16 b, guint16 a = 0);
	void draw_paint_color(canvas_t* canvas, bool fraction, ColorARGB_st color);
	void draw_paint_color(canvas_t* canvas, bool fraction,
			e_ObjAttachPaintColor_t color, guint16 a = 0);

	// Figure, Line
	void draw_circle(canvas_t* canvas, float x, float y, double radius, bool fill = false);
	void draw_triangle(canvas_t* canvas, float x, float y, float size, bool fill = false);
	void draw_triangle(canvas_t* canvas, double x, double y, double diff_x, double diff_y,
			float size, bool fill = false);
	void draw_crossline(canvas_t* canvas, float x, float y, float size);
	void draw_rectangle(canvas_t* canvas, float x, float y, double radius, bool fill = false);
	void draw_rectangle(canvas_t* canvas, float x, float y, float w, float h, bool fill = false);
	void draw_drag_area(canvas_t* canvas, float x, float y, double radius, bool circle = false, bool fill = false);
	void draw_rectangle_boundary(canvas_t* canvas, const GdkRectangle rect, bool fill = false);
	void draw_rectangle_boundary(canvas_t* canvas, float x, float y, float w, float h, bool fill = false);
	void draw_curved_line(canvas_t* canvas, double start_x, double start_y, double end_x, double end_y);

	// Image
	bool load_image(const char* filename);
	bool get_has_image(void);
	void draw_image(canvas_t* canvas);
	void draw_image(canvas_t* canvas, const GdkRectangle rect);
	void draw_image(canvas_t* canvas, double x, double y, double w, double h);

	// Text
	void set_text(const char* text);
	const char* get_text(void);
	void set_text_font_size(double size = DEFAULT_TEXT_FONT_SIZE);
	double get_text_font_size(void);
	double get_text_font_size_default(void);
	void set_text_font_color_rgba(guint16 r, guint16 g, guint16 b, guint16 a);
	void set_text_font_color(ColorARGB_st color);
	void set_text_font_color(e_ObjAttachPaintColor_t color = e_objAttachPaintColor_BLACK,
			guint16 a = PAINT_COLOR_UINT8_16(255));
	ColorARGB_st get_text_font_color(void);
	void set_text_font_face(const char* font_family = DEFAULT_TEXT_FONT_FAMILY);
	const char* get_text_font_face(void);
	void set_text_font_typeface(e_ObjAttachFontTypeface_t typeface = e_objAttachFontTypeface_NORMAL);
	e_ObjAttachFontTypeface_t get_text_font_typeface(void);
	const char* get_text_font_typeface_str(bool show = true);
	cairo_font_slant_t get_text_font_typeface_cairo(e_ObjAttachFontTypeface_t typeface);
	void set_text_font_typeface_bold(bool bold = true);
	bool get_text_font_typeface_bold(void);
	cairo_font_weight_t get_text_font_typeface_bold_cairo(void);
	void show_text_font_info_all(void);
	void show_text_font_info_color(void);
	void draw_text(canvas_t* canvas);
	void draw_text(canvas_t* canvas, const char* text, const GdkRectangle rect);
	void draw_text(canvas_t* canvas, const char* text, double x, double y);
	void draw_text(canvas_t* canvas, const char* text, double x, double y, double w, double h);
	void draw_text_only(canvas_t* canvas, const char* text, double x, double y, double w, double h);
	void draw_text_pango(canvas_t* canvas, bool simple = true);
	void draw_text_pango(canvas_t* canvas, const char* text, const GdkRectangle rect, bool simple = true);
	void draw_text_pango(canvas_t* canvas, const char* text, double x, double y, bool simple = true);
	void draw_text_pango(canvas_t* canvas, const char* text, double x, double y, double w, double h, bool simple = true);
	void set_text_show_boundary(bool show) { m_text_show_boundary = show; }
	bool get_text_show_boundary(void) { return m_text_show_boundary; }

	// Rotation
	//!  - Rotate the object image and text(Pango only currently)
	void set_obj_rotate(bool rotate);
	void set_obj_rotate(bool rotate, double degree);
	bool get_obj_rotate(void);
	void set_obj_rotate_degree(double degree);
	double get_obj_rotate_degree(void);
	void set_obj_rotate_degree_positive(bool positive);
	bool get_obj_rotate_degree_positive(void);
	void draw_obj_rotate(canvas_t* canvas, const GdkRectangle rect, double obj_w, double obj_h);
	void draw_obj_rotate(canvas_t* canvas, double x, double y, double w, double h,
			double obj_w, double obj_h);
	void draw_obj_rotate(canvas_t* canvas, double degree, double x, double y, double w, double h,
			double obj_w, double obj_h);
	void draw_obj_rotate_ui(canvas_t* canvas, double x, double y);
	void draw_obj_rotate_ui(canvas_t* canvas, double x, double y, double w, double h);
	void set_obj_rotate_update_position(e_ObjAttachDirection_t direction, float x, float y);

	// Patchers IO
	void patchers_io_set_show(bool show) { m_show_patchers_io = show; }
	bool patchers_io_get_show(void) { return m_show_patchers_io; }
	void patchers_io_set_attached_list(std::vector<CViewAttach*>& attach) { m_pvec_attach = &attach; }
	std::vector<CViewAttach*>* patchers_io_get_attached_list(void) { return m_pvec_attach; }
	bool patchers_io_add(objAttachPatchersIO_st* io);
	bool patchers_io_set(objAttachPatchersIO_st* io, const unsigned short idx);
	bool patchers_io_delete(const unsigned short idx);
	bool patchers_io_delete_all(void);
	objAttachPatchersIO_st* patchers_io_get(const unsigned short idx);
	std::vector<objAttachPatchersIO_st*>* patchers_io_get_all(void) { return m_pvec_patchers_io; }
	bool patchers_io_is_selected(GdkRectangle rect, float x, float y);
	bool patchers_io_set_select(const unsigned short idx);
	bool patchers_io_get_selected(const unsigned short idx);
	bool patchers_io_set_inout_xy(const unsigned short idx, float x, float y, bool in);
	bool patchers_io_set_inout_xy(const unsigned short idx, float in_x, float in_y,
			float out_x, float out_y);
	bool patchers_io_linking_detection(float* _ret_x = NULL, float* _ret_y = NULL);
	bool patchers_io_linking_detection(float x, float y, float* _ret_x = NULL, float* _ret_y = NULL);
	bool patchers_io_linking_detection(std::vector<CViewAttach*>* attach,
			float _x = 0.f, float _y = 0.f, bool use_xy = false,
			float* _ret_x = NULL, float* _ret_y = NULL);
	void patchers_io_info(void);
	void draw_patchers_io(canvas_t* canvas);
	//
	bool __test_patchers_io_add(unsigned short patchers = 1);
	//! ---DELETE---
	//bool __test_patchers_io_link(const unsigned short src_idx, CViewAttach* dst,
	//		const unsigned short dst_idx, bool dst_in = true) const;
	//bool __test_patchers_io_link_adjust(unsigned short src, unsigned short dst);



	// Draw all of objects
	void draw_obj(canvas_t* canvas);
private:
	// Draw
	virtual void onDraw(CBaseView* view);
};
// ---------------------------------------------------------------



#endif	// __CVIEWATTACH_H__
