/* --------------------------------------------------------------
Project:	ATP(A Tentative Plan) 3
Purpose:	
Author:		Ho-Jung Kim (godmode2k@hotmail.com)
Date:		Since Dec 2, 2014
Filename:	CBaseView.cpp

Last modified: May 29, 2015
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
#include "CBaseView.h"
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
CBaseView::CBaseView(void) : TAG("CBaseView") {
	__LOGT__( TAG, "CBaseView()" );

	__init();
}

// Dtor
CBaseView::~CBaseView() {
	__LOGT__( TAG, "~CBaseView()" );

	__release();
}

// Initialize
void CBaseView::__init(void) {
	__LOGT__( TAG, "__init()" );

	m_drawing_area = NULL;
	m_canvas = NULL;

	m_width = m_height = 0;
	m_display_width = m_display_height = 0;
}

// Release
void CBaseView::__release(void) {
	__LOGT__( TAG, "__release()" );

	//g_slice_free( Widgets_st, m_pWidgets );
}



// ---------------------------------------------------------------
void CBaseView::set_top_view(drawing_area_t* drawing_area) {
	//__LOGT__( TAG, "set_top_view()" );

	m_drawing_area = drawing_area;
}

void CBaseView::set_top_view_ref(drawing_area_t*& drawing_area) {
	//__LOGT__( TAG, "set_top_view_ref()" );

	m_drawing_area = drawing_area;
}

void CBaseView::set_canvas(canvas_t* canvas) {
	//__LOGT__( TAG, "set_canvas()" );

	m_canvas = canvas;
}

void CBaseView::set_canvas_ref(canvas_t*& canvas) {
	//__LOGT__( TAG, "set_canvas_ref()" );

	m_canvas = canvas;
}

/*
int CBaseView::get_width(void) {
	__LOGT__( TAG, "get_width()" );

	int val = 0;
	return val;
}

int CBaseView::get_height(void) {
	__LOGT__( TAG, "get_height()" );

	int val = 0;
	return val;
}
*/



// Draw
// ---------------------------------------------------------------
void CBaseView::_draw(void) {
	//__LOGT__( TAG, "_draw()" );

	onDraw( this );
}

void CBaseView::onDraw(CBaseView* view) {
	//__LOGT__( TAG, "onDraw()" );

	draw( view );
}

void CBaseView::_invalidate(void) {
	//__LOGT__( TAG, "_invalidate()" );

	// invoke draw()
	//  - v2.x: "expose-event"
	//  - v3.x: "draw"
	{
		if ( !GTK_IS_WIDGET(m_drawing_area) )
			__LOGT__( TAG, "_invalidate(): DrawingArea == widget [FALSE]" );

		// NOTE:
		// - gdk_window_progress_updates( GdkWindow* window, qboolean update_children );
		// - gtk_widget_queue_draw_area( m_drawing_area, x, y, w, h);
		//
		//gdk_threads_enter();
		//...
		//gdk_threads_leave();
		//gdk_flush();

		gtk_widget_queue_draw( m_drawing_area );

		return;
	}
	
	/*
	if ( get_rgba() ) {
		// Transparent
		//cairo_set_source_rgba( m_canvas, 1.0, 1.0, 1.0, 0.0 );

		// white
		//cairo_set_source_rgba( m_canvas, 1.0, 1.0, 1.0, 1.0 );
		cairo_set_source_rgb( m_canvas, 1.0, 1.0, 1.0 );
	}
	else {
		// Opaque white
		cairo_set_source_rgb( m_canvas, 1.0, 1.0, 1.0 );
	}

	//cairo_set_operator( m_canvas, CAIRO_OPERATOR_SOURCE );
	cairo_paint( m_canvas );
	//cairo_destroy( m_canvas );
	*/
}

void CBaseView::invalidate(CBaseView* view) {
	//__LOGT__( TAG, "invalidate()" );

	if ( view ) {
		set_top_view( view->get_top_view() );
		set_canvas( view->get_canvas() );
		onDraw( view );
	}
	else {
		_invalidate();
	}
}

//void CBaseView::invalidate_rect(float left, float top, float right, float bottom) {
//	__LOGT__( TAG, "invalidate_rect()" );
//}

void CBaseView::draw(CBaseView* view) {
	//__LOGT__( TAG, "draw()" );
}



// Touch Event
// ---------------------------------------------------------------
bool CBaseView::onTouchEvent(CKeyEvent* event) {
	//__LOGT__( TAG, "onTouchEvent()" );

	switch ( event->get_type() ) {
		case e_eventType_UNKNOWN:
			{
				//__LOGT__( TAG, "onTouchEvent(): UNKNOWN" );
				return false;
			} break;

		case e_eventType_RESERVED:
			{
				//__LOGT__( TAG, "onTouchEvent(): RESERVED" );
				return false;
			} break;

		// Keyboard
		case e_eventType_KEY_INPUT:
			{
				//__LOGT__( TAG, "onTouchEvent(): KEY_INPUT" );
			} break;

		// Mouse
		case e_eventType_MOUSE_INPUT:
			{
				//__LOGT__( TAG, "onTouchEvent(): MOUSE_INPUT" );
			} break;

		default:
			{
				//__LOGT__( TAG, "onTouchEvent(): default" );
				return false;
			} break;
	}

	switch ( event->get_action() ) {
		case e_eventType_UNKNOWN:
			{
				//__LOGT__( TAG, "onTouchEvent(): UNKNOWN" );
				return false;
			} break;

		case e_eventType_RESERVED:
			{
				//__LOGT__( TAG, "onTouchEvent(): RESERVED" );
				return false;
			} break;

		// Keyboard
		case e_eventAction_key_pressed_down:
			{
			} break;

		case e_eventAction_key_ctrl:
			{
			} break;

		case e_eventAction_key_shift:
			{
			} break;

		// Mouse
		case e_eventAction_mouse_pressed:
			{
				onTouchEventDown( event, event->get_mouse_x(), event->get_mouse_y() );
			} break;

		case e_eventAction_mouse_released:
			{
				onTouchEventUp( event, event->get_mouse_x(), event->get_mouse_y() );
			} break;

		case e_eventAction_mouse_move:
			{
				onTouchEventMove( event, event->get_mouse_x(), event->get_mouse_y() );

				if ( !(event->is_mouse_lbtn() || event->is_mouse_rbtn() ||
						event->is_mouse_wbtn()) ) {
					return false;
				}
			} break;

		case e_eventAction_mouse_wbtn_pressed:
			{
			} break;

		case e_eventAction_mouse_wheel_up:
			{
			} break;

		case e_eventAction_mouse_wheel_down:
			{
			} break;

		default:
			{
				//__LOGT__( TAG, "onTouchEvent(): default" );
				return false;
			} break;
	}


	// Redraw
	invalidate();



	return true;
}

void CBaseView::onTouchEventDown(CKeyEvent* event, float x, float y) {
	//__LOGT__( TAG, "onTouchEventDown()" );

	//__LOGT__( TAG, "onTouchEventDown(): x = %f, y = %f", x, y );
}

void CBaseView::onTouchEventUp(CKeyEvent* event, float x, float y) {
	//__LOGT__( TAG, "onTouchEventUp()" );

	//__LOGT__( TAG, "onTouchEventUp(): x = %f, y = %f", x, y );
}

void CBaseView::onTouchEventMove(CKeyEvent* event, float x, float y) {
	//__LOGT__( TAG, "onTouchEventMove()" );
	
	//__LOGT__( TAG, "onTouchEventMove(): x = %f, y = %f", x, y );
}

void CBaseView::draw_paint_color(canvas_t* canvas, double r, double g, double b, double a) {
	//__LOGT__( TAG, "draw_paint_color()" );
	
	if ( !canvas ) {
		__LOGT__( TAG, "draw_paint_color(): canvas_t == NULL" );
		return;
	}

	if ( a > 0 )
		cairo_set_source_rgba( canvas, r, g, b, a );
	else
		cairo_set_source_rgb( canvas, r, g, b );
}

void CBaseView::draw_paint_color_fraction(canvas_t* canvas,
		guint16 r, guint16 g, guint16 b, guint16 a) {
	//__LOGT__( TAG, "draw_paint_color_fraction()" );
	
	if ( !canvas ) {
		__LOGT__( TAG, "draw_paint_color_fraction(): canvas_t == NULL" );
		return;
	}

	{
		double f_r = PAINT_COLOR_UINT16_FRACTION_CAIRO( r );
		double f_g = PAINT_COLOR_UINT16_FRACTION_CAIRO( g );
		double f_b = PAINT_COLOR_UINT16_FRACTION_CAIRO( b );
		double f_a = PAINT_COLOR_UINT16_FRACTION_CAIRO( a );

		if ( a > 0 )
			cairo_set_source_rgba( canvas, f_r, f_g, f_b, f_a );
		else
			cairo_set_source_rgb( canvas, f_r, f_g, f_b );
	}
}

void CBaseView::draw_paint_color(canvas_t* canvas, bool fraction, ColorARGB_st color) {
	//__LOGT__( TAG, "draw_paint_color()" );
	
	if ( !canvas ) {
		__LOGT__( TAG, "draw_paint_color(): canvas_t == NULL" );
		return;
	}

	{
		if ( fraction ) {
			draw_paint_color_fraction( canvas, color.r, color.g, color.b, color.a );
		}
		else {
			double r, g, b, a;

			a = (double)color.a;
			r = (double)color.r;
			g = (double)color.g;
			b = (double)color.b;

			draw_paint_color( canvas, r, g, b, a );
		}
	}
}

void CBaseView::draw_paint_color(canvas_t* canvas, bool fraction,
		e_ObjAttachPaintColor_t color, guint16 a) {
	//__LOGT__( TAG, "draw_paint_color()" );
	
	if ( !canvas ) {
		__LOGT__( TAG, "draw_paint_color(): canvas_t == NULL" );
		return;
	}

	{
		guint16 r = 0;
		guint16 g = 0;
		guint16 b = 0;

		/*
		switch ( color ) {
			case e_objAttachPaintColor_BLACK:
			case e_objAttachPaintColor_BLUE:
			case e_objAttachPaintColor_CYAN:
			case e_objAttachPaintColor_DKGRAY:
			case e_objAttachPaintColor_GRAY:
			case e_objAttachPaintColor_GREEN:
			case e_objAttachPaintColor_LTGRAY:
			case e_objAttachPaintColor_MAGENTA:
			case e_objAttachPaintColor_RED:
			//case e_objAttachPaintColor_TRANSPARENT:
			case e_objAttachPaintColor_WHITE:
			case e_objAttachPaintColor_YELLOW:
			default:
				{
					color = e_objAttachPaintColor_BLACK;
				} break;
		} // switch ()
		*/


		PAINT_COLOR_RGBA_16( color, &r, &g, &b );

		//__LOGT__( TAG, "draw_paint_color(): R = %#x(h), G = %#x(h), B = %#x(h)",
		//			r, g, b );
		//__LOGT__( TAG, "draw_paint_color(): 16: R = %d, G = %d, B = %d", r, g, b );
		//__LOGT__( TAG, "draw_paint_color(): 8: R = %d, G = %d, B = %d",
		//			PAINT_COLOR_UINT16_8(r),
		//			PAINT_COLOR_UINT16_8(g),
		//			PAINT_COLOR_UINT16_8(b) );


		if ( fraction ) {
			draw_paint_color_fraction( canvas, r, g, b, a );
		}
		else {
			draw_paint_color( canvas, (double)r, (double)g, (double)b, (double)a );
		}
	}
}

void CBaseView::draw_circle(canvas_t* canvas, float x, float y, double radius, bool fill) {
	//__LOGT__( TAG, "draw_circle()" );
	
	if ( !canvas ) {
		__LOGT__( TAG, "draw_circle(): canvas_t == NULL" );
		return;
	}

	{
		// move the drawing origin to the x and y
		//cairo_translate( canvas, x, y );
		
		// add a new circular path to the cairo drawing context
		//cairo_arc( canvas, x, y, radius, 0, (2 * M_PI) );

		// the existing path is not affected before calling cairo_arc()
		//cairo_new_sub_path( canvas );


		cairo_new_sub_path( canvas );
		cairo_arc( canvas, x, y, radius, 0, (2 * M_PI) );

		if ( fill )
			cairo_fill( canvas );


		// draws the outline of the circle
		//cairo_stroke_preserve( canvas );
	}
}
// ---------------------------------------------------------------



//! Implementation
// ---------------------------------------------------------------
// Global variable
//
// ---------------------------------------------------------------

