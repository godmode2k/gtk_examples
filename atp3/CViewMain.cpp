/* --------------------------------------------------------------
Project:	ATP(A Tentative Plan) 3
Purpose:	
Author:		Ho-Jung Kim (godmode2k@hotmail.com)
Date:		Since Dec 2, 2014
Filename:	CViewMain.cpp

Last modified: Feb 8, 2015
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
#include "CViewMain.h"
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
CViewMain::CViewMain(void) : TAG("CViewMain") {
	__LOGT__( TAG, "CViewMain()" );

	__init();
}

// Dtor
CViewMain::~CViewMain() {
	__LOGT__( TAG, "~CViewMain()" );

	__release();
}

// Initialize
void CViewMain::__init(void) {
	__LOGT__( TAG, "__init()" );

	//m_canvas = NULL;

	m_width = m_height = 0;
	m_display_width = m_display_height = 0;


	// Attachment
	m_pvec_attach = new std::vector<CViewAttach*>;


	// Temporary
	//  - Font information
	set_tmp_font_info_reset();
	//  - Background Image information
	set_tmp_bg_img_info_release();


	// Take a Screenshot
	m_screenshot_region = false;
	m_screenshot_rect.x = 50;
	m_screenshot_rect.y = 50;
	m_screenshot_rect.width = 100;
	m_screenshot_rect.height = 100;
	m_screenshot_touchX = m_screenshot_touchY = 0.f;
	m_screenshot_direction = e_objAttachDirection_UNKNOWN;
	memset( (void*)&m_screenshot_pathname, 0x00, sizeof(m_screenshot_pathname) );

	//! TEST
	// --------------------
	//attach_add_image( "./res/img1.png", "#1" );
	//attach_add_image( "./res/img2.png" );
	//attach_add_text( "0test...1test...2test...3test...4test...5test...", NULL, "#2" );
	//attach_add_text( "0test...1test...2test...3test...4test...5test...", "./res/img2.png", "#3" );
	//attach_add_text( "0test...1test...2test...3test...4test...5test...", "./res/attach_balloon_1.9.png", "#4" );
}

// Release
void CViewMain::__release(void) {
	__LOGT__( TAG, "__release()" );

	//g_slice_free( Widgets_st, m_pWidgets );
	

	// Temporary Background Image  information
	set_tmp_bg_img_info_release();


	// Attachment
	if ( m_pvec_attach ) {
		__LOGT__( TAG, "__release(): Attachment: size = %d", m_pvec_attach->size() );

		if ( m_pvec_attach->size() > 0 ) {
			std::vector<CViewAttach*>::iterator iter;

			for ( iter = m_pvec_attach->begin(); iter != m_pvec_attach->end(); ) {
				CViewAttach* attach = (*iter);

				if ( attach ) {
					delete attach;
					attach = NULL;
				}

				iter = m_pvec_attach->erase( iter );
			}

			m_pvec_attach->clear();
		}

		delete m_pvec_attach;
		m_pvec_attach = NULL;
	}
}

#if 0
// Initialize Widget structure
bool CViewMain::init_widget_all(Widgets_st** pWidgets) {
	__LOGT__( TAG, "init_widget_all()" );

	(*pWidgets) = g_slice_new( Widgets_st );

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
bool CViewMain::init_ui_with_callback(void) {
	__LOGT__( TAG, "init_ui_with_callback()" );

	return true;
}
#endif



// ---------------------------------------------------------------
/*
void CViewMain::set_canvas(canvas_t* canvas) {
	__LOGT__( TAG, "set_canvas()" );

	m_canvas = canvas;
}
*/

/*
int CViewMain::get_width(void) {
	__LOGT__( TAG, "get_width()" );

	int val = 0;
	return val;
}

int CViewMain::get_height(void) {
	__LOGT__( TAG, "get_height()" );

	int val = 0;
	return val;
}
*/



// Draw
// ---------------------------------------------------------------
void CViewMain::onDraw(CBaseView* view) {
	//__LOGT__( TAG, "onDraw()" );

	draw( view );
}

//! WILL BE DELETED ---------------------------------------------
/*
void CViewMain::invalidate(void) {
	//__LOGT__( TAG, "invalidate()" );

	_invalidate();
	onDraw( this );
}

void CViewMain::invalidate(CBaseView* view) {
	//__LOGT__( TAG, "invalidate()" );

	// Parent's view
	//
	//set_canvas( view->get_canvas() );
	//view->_invalidate();
	
	onDraw( view );
}

//void CViewMain::invalidate_rect(float left, float top, float right, float bottom) {
//	__LOGT__( TAG, "invalidate_rect()" );
//}
*/
//! WILL BE DELETED ---------------------------------------------

void CViewMain::draw(CBaseView* view) {
	//__LOGT__( TAG, "draw()" );

	if ( !view ) {
		__LOGT__( TAG, "draw(): CBaseView == NULL" );
		return;
	}


	// Childs views
	{
		//__LOGT__( TAG, "draw(): attach tag = %s", attach.get_tag() );
		//m_attach.invalidate( view );

		attach_invalidate( view );
	}


	// Screenshot
	{
		canvas_pt canvas = view->get_canvas();

		if ( !canvas ) {
			__LOGT__( TAG, "draw(): CBaseView == NULL" );
			return;
		}

		if ( take_screenshot_get_region() ) {
			take_screenshot_region( canvas );
		}
	}
}



// Touch Event
// ---------------------------------------------------------------
bool CViewMain::onTouchEvent(CKeyEvent* event) {
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


	// Take a Screenshot
	if ( take_screenshot_get_region() ) {
		return true;
	}


	// Childs views
	{
		//m_attach.onTouchEvent( event );

		attach_touch_event( event );
	}


	return true;
}

void CViewMain::onTouchEventDown(CKeyEvent* event, float x, float y) {
	//__LOGT__( TAG, "onTouchEventDown()" );

	//__LOGT__( TAG, "onTouchEventDown(): x = %f, y = %f", x, y );
	
	m_screenshot_touchX = x;
	m_screenshot_touchY = y;
	m_screenshot_direction = take_screenshot_get_selected_direction( m_screenshot_rect,
					m_screenshot_touchX, m_screenshot_touchY );
}

void CViewMain::onTouchEventUp(CKeyEvent* event, float x, float y) {
	//__LOGT__( TAG, "onTouchEventUp()" );

	//__LOGT__( TAG, "onTouchEventUp(): x = %f, y = %f", x, y );
}

void CViewMain::onTouchEventMove(CKeyEvent* event, float x, float y) {
	//__LOGT__( TAG, "onTouchEventMove()" );
	
	//__LOGT__( TAG, "onTouchEventMove(): x = %f, y = %f", x, y );

	/*
	if ( event->is_mouse_lbtn() ) {
		__LOGT__( TAG, "onTouchEventMove(): MOUSE L_BTN" );

		if ( is_obj_selected(m_rect_menu, x, y) ) {
			__LOGT__( TAG, "onTouchEventMove(): SELECTED" );
			set_obj_rect( m_rect_menu, x, y );
		}
	}
	*/

	if ( event->is_mouse_lbtn() ) {
		take_screenshot_update_position( m_screenshot_direction, x, y );
	}
}

// ---------------------------------------------------------------

void CViewMain::draw_paint_color(canvas_t* canvas, double r, double g, double b, double a) {
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

void CViewMain::draw_paint_color_fraction(canvas_t* canvas,
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

void CViewMain::draw_paint_color(canvas_t* canvas, bool fraction, ColorARGB_st color) {
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

void CViewMain::draw_paint_color(canvas_t* canvas, bool fraction,
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

void CViewMain::draw_circle(canvas_t* canvas, float x, float y, double radius, bool fill) {
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
//
void CViewMain::take_screenshot_get_str_direction(e_ObjAttachDirection_t direction) {
	//__LOGT__( TAG, "take_screenshot_get_str_direction()" );
	
	switch ( direction ) {
		/*
		case e_objAttachDirection_UNKNOWN:
			{} break;
		case e_objAttachDirection_RESERVED:
			{} break;
		*/
		case e_objAttachDirection_LEFT:
			{
				__LOGT__( TAG, "take_screenshot_get_str_direction(): LEFT" );
			} break;
		case e_objAttachDirection_TOP:
			{
				__LOGT__( TAG, "take_screenshot_get_str_direction(): TOP" );
			} break;
		case e_objAttachDirection_RIGHT:
			{
				__LOGT__( TAG, "take_screenshot_get_str_direction(): RIGHT" );
			} break;
		case e_objAttachDirection_BOTTOM:
			{
				__LOGT__( TAG, "take_screenshot_get_str_direction(): BOTTOM" );
			} break;
		case e_objAttachDirection_LEFT_TOP:
			{
				__LOGT__( TAG, "take_screenshot_get_str_direction(): LEFT_TOP" );
			} break;
		case e_objAttachDirection_RIGHT_TOP:
			{
				__LOGT__( TAG, "take_screenshot_get_str_direction(): RIGHT_TOP" );
			} break;
		case e_objAttachDirection_LEFT_BOTTOM:
			{
				__LOGT__( TAG, "take_screenshot_get_str_direction(): LEFT_BOTTOM" );
			} break;
		case e_objAttachDirection_RIGHT_BOTTOM:
			{
				__LOGT__( TAG, "take_screenshot_get_str_direction(): RIGHT_BOTTOM" );
			} break;
		case e_objAttachDirection_CENTER:
			{
				__LOGT__( TAG, "take_screenshot_get_str_direction(): CENTER" );
			} break;
		case e_objAttachDirection_LEFT_CENTER:
			{
				__LOGT__( TAG, "take_screenshot_get_str_direction(): LEFT_CENTER" );
			} break;
		case e_objAttachDirection_TOP_CENTER:
			{
				__LOGT__( TAG, "take_screenshot_get_str_direction(): TOP_CENTER" );
			} break;
		case e_objAttachDirection_RIGHT_CENTER:
			{
				__LOGT__( TAG, "take_screenshot_get_str_direction(): RIGHT_CENTER" );
			} break;
		case e_objAttachDirection_BOTTOM_CENTER:
			{
				__LOGT__( TAG, "take_screenshot_get_str_direction(): BOTTOM_CENTER" );
			} break;
		default:
			{
				__LOGT__( TAG, "take_screenshot_get_str_direction(): UNKNOWN" );
			} break;
	}
}

e_ObjAttachDirection_t CViewMain::take_screenshot_get_selected_direction(GdkRectangle rect, float x, float y) {
	//__LOGT__( TAG, "take_screenshot_get_selected_direction()" );
	
	const int radius = DEFAULT_DRAW_CIRCLE_RADIUS;
	e_ObjAttachDirection_t ret = e_objAttachDirection_UNKNOWN;


	// LEFT-TOP
	if ( ((int)x >= ((rect.x-radius) - radius)) &&
		((int)x <= ((rect.x-radius) + radius)) &&
		((int)y >= ((rect.y-radius) - radius)) &&
		((int)y <= ((rect.y-radius) + radius)) ) {
		ret = e_objAttachDirection_LEFT_TOP;
	}
	// RIGHT-TOP
	else if ( ((int)x >= (((rect.x+radius) + rect.width) - radius)) &&
			((int)x <= (((rect.x+radius) + rect.width) + radius)) &&
			((int)y >= ((rect.y-radius) - radius)) &&
			((int)y <= ((rect.y-radius) + radius)) ) {
		ret = e_objAttachDirection_RIGHT_TOP;
	}
	// LEFT-BOTTOM
	else if ( ((int)x >= ((rect.x-radius) - radius)) &&
			((int)x <= ((rect.x-radius) + radius)) &&
			((int)y >= (((rect.y+radius) + rect.height) - radius)) &&
			((int)y <= (((rect.y+radius) + rect.height) + radius)) ) {
		ret = e_objAttachDirection_LEFT_BOTTOM;
	}
	// RIGHT-BOTTOM
	else if ( ((int)x >= (((rect.x+radius) + rect.width) - radius)) &&
			((int)x <= (((rect.x+radius) + rect.width) + radius)) &&
			((int)y >= (((rect.y+radius) + rect.height) - radius)) &&
			((int)y <= (((rect.y+radius) + rect.height) + radius)) ) {
		ret = e_objAttachDirection_RIGHT_BOTTOM;
	}
	// LEFT-CENTER
	else if ( ((int)x >= ((rect.x-radius) - radius)) &&
			((int)x <= ((rect.x-radius) + radius)) &&
			((int)y >= ((rect.y + (rect.height >> 1)) - radius)) &&
			((int)y <= ((rect.y + (rect.height >> 1)) + radius)) ) {
		ret = e_objAttachDirection_LEFT_CENTER;
	}
	// RIGHT-CENTER
	else if ( ((int)x >= (((rect.x+radius) + rect.width) - radius)) &&
			((int)x <= (((rect.x+radius) + rect.width) + radius)) &&
			((int)y >= ((rect.y + (rect.height >> 1)) - radius)) &&
			((int)y <= ((rect.y + (rect.height >> 1)) + radius)) ) {
		ret = e_objAttachDirection_RIGHT_CENTER;
	}
	// TOP-CENTER
	else if ( ((int)x >= ((rect.x + (rect.width >> 1)) - radius)) &&
			((int)x <= ((rect.x + (rect.width >> 1)) + radius)) &&
			((int)y >= ((rect.y-radius) - radius)) &&
			((int)y <= ((rect.y-radius) + radius)) ) {
		ret = e_objAttachDirection_TOP_CENTER;
	}
	// BOTTOM-CENTER
	else if ( ((int)x >= ((rect.x + (rect.width >> 1)) - radius)) &&
			((int)x <= ((rect.x + (rect.width >> 1)) + radius)) &&
			((int)y >= (((rect.y+radius) + rect.height) - radius)) &&
			((int)y <= (((rect.y+radius) + rect.height) + radius)) ) {
		ret = e_objAttachDirection_BOTTOM_CENTER;
	}
	// CENTER
	else if ( ((int)x >= rect.x) && ((int)x <= (rect.x + rect.width)) &&
			((int)y >= rect.y) && ((int)y <= (rect.y + rect.height)) ) {
		ret = e_objAttachDirection_CENTER;
	}


	return ret;
}

void CViewMain::take_screenshot_update_position(e_ObjAttachDirection_t direction, float x, float y) {
	//__LOGT__( TAG, "take_screenshot_update_position()" );
	
	{
		//const int radius = DEFAULT_DRAW_CIRCLE_RADIUS;
		float pos_x = 0.f;
		float pos_y = 0.f;

		// Direction
		switch ( direction ) {
			case e_objAttachDirection_LEFT:
				{
				} break;
			case e_objAttachDirection_TOP:
				{
				} break;
			case e_objAttachDirection_RIGHT:
				{
				} break;
			case e_objAttachDirection_BOTTOM:
				{
				} break;

			// Diagonal line position
			case e_objAttachDirection_LEFT_TOP:
				{
					// TOP
					//if ( ((m_screenshot_rect.y + m_screenshot_rect.height) - y) < radius ) {
					//	return;
					//}

					if ( m_screenshot_rect.y > y ) {
						pos_y = (m_screenshot_rect.y - y);
						m_screenshot_rect.y -= (int)pos_y;
						m_screenshot_rect.height += (int)pos_y;
					}
					else if ( m_screenshot_rect.y < y ) {
						pos_y = (y - m_screenshot_rect.y);
						m_screenshot_rect.y += (int)pos_y;
						m_screenshot_rect.height -= (int)pos_y;
					}

					// LEFT
					//if ( ((m_screenshot_rect.x + m_screenshot_rect.width) - x) < radius ) {
					//	return;
					//}

					if ( m_screenshot_rect.x > x ) {
						pos_x = (m_screenshot_rect.x - x);
						m_screenshot_rect.x -= (int)pos_x;
						m_screenshot_rect.width += (int)pos_x;
					}
					else if ( m_screenshot_rect.x < x ) {
						pos_x = (x - m_screenshot_rect.x);
						m_screenshot_rect.x += (int)pos_x;
						m_screenshot_rect.width -= (int)pos_x;
					}
				} break;
			case e_objAttachDirection_RIGHT_TOP:
				{
					// TOP
					//if ( ((m_screenshot_rect.y + m_screenshot_rect.height) - y) < radius ) {
					//	return;
					//}

					if ( m_screenshot_rect.y > y ) {
						pos_y = (m_screenshot_rect.y - y);
						m_screenshot_rect.y -= (int)pos_y;
						m_screenshot_rect.height += (int)pos_y;
					}
					else if ( m_screenshot_rect.y < y ) {
						pos_y = (y - m_screenshot_rect.y);
						m_screenshot_rect.y += (int)pos_y;
						m_screenshot_rect.height -= (int)pos_y;
					}

					// RIGHT
					//if ( (x - m_screenshot_rect.x) < radius ) {
					//	return;
					//}

					if ( (m_screenshot_rect.x + m_screenshot_rect.width) > x ) {
						pos_x = (m_screenshot_rect.x + m_screenshot_rect.width) - x;
						m_screenshot_rect.width -= (int)pos_x;
					}
					else if ( (m_screenshot_rect.x + m_screenshot_rect.width) < x ) {
						pos_x = x - (m_screenshot_rect.x + m_screenshot_rect.width);
						m_screenshot_rect.width += (int)pos_x;
					}
				} break;
			case e_objAttachDirection_LEFT_BOTTOM:
				{
					// BOTTOM
					//if ( (y - m_screenshot_rect.y) < radius ) {
					//	return;
					//}

					if ( (m_screenshot_rect.y + m_screenshot_rect.height) > y ) {
						pos_y = (m_screenshot_rect.y + m_screenshot_rect.height) - y;
						m_screenshot_rect.height -= (int)pos_y;
					}
					else if ( m_screenshot_rect.height < y ) {
						pos_y = y - (m_screenshot_rect.y + m_screenshot_rect.height);
						m_screenshot_rect.height += (int)pos_y;
					}

					// LEFT
					//if ( ((m_screenshot_rect.x + m_screenshot_rect.width) - x) < radius ) {
					//	return;
					//}

					if ( m_screenshot_rect.x > x ) {
						pos_x = (m_screenshot_rect.x - x);
						m_screenshot_rect.x -= (int)pos_x;
						m_screenshot_rect.width += (int)pos_x;
					}
					else if ( m_screenshot_rect.x < x ) {
						pos_x = (x - m_screenshot_rect.x);
						m_screenshot_rect.x += (int)pos_x;
						m_screenshot_rect.width -= (int)pos_x;
					}
				} break;
			case e_objAttachDirection_RIGHT_BOTTOM:
				{
					// BOTTOM
					//if ( (y - m_screenshot_rect.y) < radius ) {
					//	return;
					//}

					if ( (m_screenshot_rect.y + m_screenshot_rect.height) > y ) {
						pos_y = (m_screenshot_rect.y + m_screenshot_rect.height) - y;
						m_screenshot_rect.height -= (int)pos_y;
					}
					else if ( m_screenshot_rect.height < y ) {
						pos_y = y - (m_screenshot_rect.y + m_screenshot_rect.height);
						m_screenshot_rect.height += (int)pos_y;
					}

					// RIGHT
					//if ( (x - m_screenshot_rect.x) < radius ) {
					//	return;
					//}

					if ( (m_screenshot_rect.x + m_screenshot_rect.width) > x ) {
						pos_x = (m_screenshot_rect.x + m_screenshot_rect.width) - x;
						m_screenshot_rect.width -= (int)pos_x;
					}
					else if ( (m_screenshot_rect.x + m_screenshot_rect.width) < x ) {
						pos_x = x - (m_screenshot_rect.x + m_screenshot_rect.width);
						m_screenshot_rect.width += (int)pos_x;
					}

				} break;
			
			// CENTER
			case e_objAttachDirection_CENTER:
				{
					if ( m_screenshot_touchX > x ) {
						pos_x = (m_screenshot_touchX - x);
						m_screenshot_rect.x -= (int)pos_x;
						m_screenshot_touchX -= (int)pos_x;
					}
					else if ( m_screenshot_touchX < x ) {
						pos_x = (x - m_screenshot_touchX);
						m_screenshot_rect.x += (int)pos_x;
						m_screenshot_touchX += (int)pos_x;
					}
					if ( m_screenshot_touchY > y ) {
						pos_y = (m_screenshot_touchY - y);
						m_screenshot_rect.y -= (int)pos_y;
						m_screenshot_touchY -= (int)pos_y;
					}
					else if ( m_screenshot_touchY < y ) {
						pos_y = (y - m_screenshot_touchY);
						m_screenshot_rect.y += (int)pos_y;
						m_screenshot_touchY += (int)pos_y;
					}
				} break;
			case e_objAttachDirection_LEFT_CENTER:
				{
					//if ( ((m_screenshot_rect.x + m_screenshot_rect.width) - x) < radius ) {
					//	return;
					//}

					if ( m_screenshot_rect.x > x ) {
						pos_x = (m_screenshot_rect.x - x);
						m_screenshot_rect.x -= (int)pos_x;
						m_screenshot_rect.width += (int)pos_x;
					}
					else if ( m_screenshot_rect.x < x ) {
						pos_x = (x - m_screenshot_rect.x);
						m_screenshot_rect.x += (int)pos_x;
						m_screenshot_rect.width -= (int)pos_x;
					}
				} break;
			case e_objAttachDirection_TOP_CENTER:
				{
					//if ( ((m_screenshot_rect.y + m_screenshot_rect.height) - y) < radius ) {
					//	return;
					//}

					if ( m_screenshot_rect.y > y ) {
						pos_y = (m_screenshot_rect.y - y);
						m_screenshot_rect.y -= (int)pos_y;
						m_screenshot_rect.height += (int)pos_y;
					}
					else if ( m_screenshot_rect.y < y ) {
						pos_y = (y - m_screenshot_rect.y);
						m_screenshot_rect.y += (int)pos_y;
						m_screenshot_rect.height -= (int)pos_y;
					}
				} break;
			case e_objAttachDirection_RIGHT_CENTER:
				{
					//if ( ((x - m_screenshot_rect.x) < radius ) {
					//	return;
					//}

					if ( (m_screenshot_rect.x + m_screenshot_rect.width) > x ) {
						pos_x = (m_screenshot_rect.x + m_screenshot_rect.width) - x;
						m_screenshot_rect.width -= (int)pos_x;
					}
					else if ( (m_screenshot_rect.x + m_screenshot_rect.width) < x ) {
						pos_x = x - (m_screenshot_rect.x + m_screenshot_rect.width);
						m_screenshot_rect.width += (int)pos_x;
					}
				} break;
			case e_objAttachDirection_BOTTOM_CENTER:
				{
					//if ( (y - m_screenshot_rect.y) < radius ) {
					//	return;
					//}

					if ( (m_screenshot_rect.y + m_screenshot_rect.height) > y ) {
						pos_y = (m_screenshot_rect.y + m_screenshot_rect.height) - y;
						m_screenshot_rect.height -= (int)pos_y;
					}
					else if ( m_screenshot_rect.height < y ) {
						pos_y = y - (m_screenshot_rect.y + m_screenshot_rect.height);
						m_screenshot_rect.height += (int)pos_y;
					}
				} break;
			default:
				{
				} break;
		}
	}
}

bool CViewMain::take_screenshot_get_region(void) {
	//__LOGT__( TAG, "take_screenshot_get_region()" );
	
	return m_screenshot_region;
}

void CViewMain::take_screenshot_set_region(bool set_region, bool init) {
	//__LOGT__( TAG, "take_screenshot_set_region()" );
	
	m_screenshot_region = set_region;

	if ( init ) {
		m_screenshot_rect.x = 50;
		m_screenshot_rect.y = 50;
		m_screenshot_rect.width = 100;
		m_screenshot_rect.height = 100;
	}
}

void CViewMain::take_screenshot_region(canvas_t* canvas) {
	//__LOGT__( TAG, "take_screenshot_region()" );
	
	//cairo_save( canvas );
	{
		const int radius = DEFAULT_DRAW_CIRCLE_RADIUS;
		int x = 0;
		int y = 0;


		// Clear previous path
		//cairo_new_sub_path( canvas );

		// Paint
		draw_paint_color( canvas, true, e_objAttachPaintColor_RED );

		{
			// dotted(dashed) rectangle
			const double dashed[] = { 3.0f, 3.0f };
			const int dashed_len = sizeof(dashed) / sizeof(dashed[0]);

			cairo_set_line_width( canvas, 1.0f );
			cairo_set_dash( canvas, dashed, dashed_len, 0 );
			cairo_rectangle( canvas, m_screenshot_rect.x, m_screenshot_rect.y,
								m_screenshot_rect.width, m_screenshot_rect.height );
			cairo_stroke( canvas );
		}

		//
		// Draw a circle for respective direction as following:
		//  - 4 ways(Left, Right, Top, Bottom)
		//  - Diagonal line position
		//
		//		x------x------x
		//		| .         . |
		//		|   .     .   |
		//		x      x      x
		//		|    .   .    |
		//		| .         . |
		//		x------x------x
		//
		//
		//  - outside figures for capture region
		//
		//		x       x       x
		//		 +-------------+
		//		 | .         . |
		//		 |   .     .   |
		//		x|      x      |x
		//		 |    .   .    |
		//		 | .         . |
		//		 +-------------+
		//		x       x       x
		//

		x = m_screenshot_rect.x - radius;
		y = m_screenshot_rect.y - radius;
		// move the drawing origin to the x and y
		//cairo_translate( canvas, x, y );
		// add a new circular path to the cairo drawing context
		//cairo_arc( canvas, x, y, radius, 0, (2 * M_PI) );
		// the existing path is not affected before calling cairo_arc()
		//cairo_new_sub_path( canvas );

		// Left-Top
		draw_circle( canvas, x, y, radius );
		// Right-Top
		x = ( m_screenshot_rect.x + m_screenshot_rect.width ) + radius;
		draw_circle( canvas, x, y, radius );
		// Center-Top
		x = ( m_screenshot_rect.x + (m_screenshot_rect.width >> 1) );
		draw_circle( canvas, x, y, radius );

		x = m_screenshot_rect.x - radius;
		y = ( m_screenshot_rect.y + m_screenshot_rect.height ) + radius;

		// Left-Bottom
		draw_circle( canvas, x, y, radius );
		// Right-Bottom
		x = ( m_screenshot_rect.x + m_screenshot_rect.width ) + radius;
		draw_circle( canvas, x, y, radius );
		// Center-Bottom
		x = ( m_screenshot_rect.x + (m_screenshot_rect.width >> 1) );
		draw_circle( canvas, x, y, radius );

		x = m_screenshot_rect.x - radius;
		y = ( m_screenshot_rect.y + (m_screenshot_rect.height >> 1) );

		// Left,Right-Center
		draw_circle( canvas, x, y, radius );
		x = ( m_screenshot_rect.x + m_screenshot_rect.width ) + radius;
		draw_circle( canvas, x, y, radius );



		// draws the outline of the circle
		cairo_stroke_preserve( canvas );
	}
	//cairo_restore( canvas );
}

const char* CViewMain::take_screenshot_get_pathname(void) {
	//__LOGT__( TAG, "take_screenshot_get_pathname()" );
	
	return m_screenshot_pathname;
}

bool CViewMain::take_screenshot_save(void) {
	//__LOGT__( TAG, "take_screenshot()" );

	// default image format: PNG
	
	const int padding = 1;
	int x, y, width, height;
	const char* filename = "screenshot";
	const char* ext = "png";
	const e_takeScreenshot_t format = e_takeScreenshot_PNG;

	char pathname[TAKE_SCREENSHOT__PATHNAME_MAX_SIZE] = { 0, };
	char uuid[g_INT_MAX_UUID_SIZE] = { 0, };
#ifdef __LINUX__
	{
		struct stat st;
		struct passwd* pw;


		//! TODO: thread-safe
		pw = getpwuid( getuid() );
		if ( pw ) {
			snprintf( pathname, sizeof(pathname), "/home/%s/Pictures/",
						pw->pw_name );

			// check a directory exist
			if ( stat(pathname, &st) != 0 ) {
				memset( (void*)&pathname, 0x00, sizeof(pathname) );
				snprintf( pathname, sizeof(pathname), "/home/%s/", pw->pw_name );

				if ( stat(pathname, &st) != 0 ) {
					return false;
				}
			}

			memset( (void*)&pathname, 0x00, sizeof(pathname) );
			if ( get_uuid(uuid, sizeof(uuid)) ) {
				// "/home/" + pw->pw_name + "/Pictures/" + filename + "-" + uuid + ext
				snprintf( pathname, sizeof(pathname), "/home/%s/Pictures/%s-%s.%s",
							pw->pw_name, filename, uuid, ext );
			}
			else {
				// "/home/" + pw->pw_name + filename
				snprintf( pathname, sizeof(pathname), "/home/%s/%s.%s",
							pw->pw_name, filename, ext );
			}

			//__LOGT__( TAG, "take_screenshot(): pathname = %s", pathname );
		}
	}
#else
	{
		if ( get_uuid(uuid, sizeof(uuid)) ) {
			// filename + "-" + uuid + ext
			snprintf( pathname, sizeof(pathname), "%s-%s.%s", filename, uuid, ext );
		}
		else {
			// filename + ext
			snprintf( pathname, sizeof(pathname), "%s.%s", filename, ext );
		}

		//__LOGT__( TAG, "take_screenshot(): pathname = %s", pathname );
	}
#endif

	x = m_screenshot_rect.x + padding;
	y = m_screenshot_rect.y + padding;
	width = m_screenshot_rect.width - (padding << 1);
	height = m_screenshot_rect.height - (padding << 1);

	memset( (void*)&m_screenshot_pathname, 0x00, sizeof(m_screenshot_pathname) );
	snprintf( m_screenshot_pathname, sizeof(m_screenshot_pathname), "%s",  pathname );


	return take_screenshot_save( x, y, width, height, pathname, format );
}

bool CViewMain::take_screenshot_save(int x, int y, int width, int height,
	const char* filename, const e_takeScreenshot_t format) {
	//__LOGT__( TAG, "take_screenshot_save()" );
	
	GdkPixbuf* pixbuf = NULL;
	GdkWindow* gdk_window = NULL;
	drawing_area_t* view = NULL;
	char* img_format = NULL;
	GError* error = NULL;
	bool saved = false;


	if ( format == e_takeScreenshot_PNG )
		img_format = (char*)"png";
	else if ( format == e_takeScreenshot_JPEG )
		img_format = (char*)"jpeg";

	if ( !img_format ) {
		__LOGT__( TAG, "take_screenshot_save(): image format == NULL" );
		return false;
	}


	view = get_top_view();
	if ( !view ) {
		__LOGT__( TAG, "take_screenshot_save(): Top View == NULL" );
		return false;
	}

	gdk_window = view->window;
	if ( !gdk_window )  {
		__LOGT__( TAG, "take_screenshot_save(): GdkWindow == NULL" );
		return false;
	}

	{
		//gdk_drawable_get_size(GDK_DRAWABLE(view), &width, &height );
		pixbuf = gdk_pixbuf_get_from_drawable( NULL, GDK_DRAWABLE(gdk_window),
						NULL, x, y, 0, 0, width, height );

		if ( !pixbuf ) {
			__LOGT__( TAG, "take_screenshot_save(): GdkWindow == NULL" );
			return false;
		}

		saved = gdk_pixbuf_save( pixbuf, filename, (const char*)img_format, &error,
									"quality", "100", NULL );

		g_object_unref( pixbuf );

		if ( !saved ) {
			__LOGT__( TAG, "take_screenshot_save(): Saved [FALSE]" );

			if ( error ) {
				__LOGT__( TAG, "take_screenshot_save(): %s", error->message );
				g_free( error );
				error = NULL;
			}

			return false;
		}
	}

	//__LOGT__( TAG, "take_screenshot_save(): Saved [TRUE]" );
	//__LOGT__( TAG, "take_screenshot_save(): Saved: %s", filename );


	return true;
}

// ---------------------------------------------------------------

void CViewMain::add_obj_rect(obj_rect_list_st obj) {
	//__LOGT__( TAG, "add_obj_rect()" );
	
	/*
	m_vec_rect.push_back( obj );
	*/
}

void CViewMain::add_obj_rect(e_ObjType_t type, GdkRectangle rect) {
	//__LOGT__( TAG, "add_obj_rect()" );
	
	/*
	obj_rect_list_st obj;

	obj.type = type;
	obj.rect.x = rect.x;
	obj.rect.y = rect.y;
	obj.rect.width = rect.width;
	obj.rect.height = rect.height;

	m_vec_rect.push_back( obj );
	*/
}

void CViewMain::get_obj_rect(e_ObjType_t type) {
	//__LOGT__( TAG, "get_obj_rect()" );
	
	switch ( type ) {
		case e_objType_UNKNOWN:
			{
				__LOGT__( TAG, "get_obj_rect(): UNKNOWN" );
			} break;

		case e_objType_RESERVED:
			{
				__LOGT__( TAG, "get_obj_rect(): RESERVED" );
			} break;

		case e_objType_MENU:
			{
				__LOGT__( TAG, "get_obj_rect(): MENU" );
			} break;

		default:
			{
				__LOGT__( TAG, "get_obj_rect(): default" );
			} break;
	}
}

void CViewMain::set_obj_rect(e_ObjType_t type, GdkRectangle rect) {
	//__LOGT__( TAG, "set_obj_rect()" );
	
	/*
	std::vector<obj_rect_list_st>::iterator iter;

	for ( iter = m_vec_rect.begin(); iter != m_vec_rect.end(); ++iter ) {
		obj_rect_list_st obj = (*iter);

		if ( (rect.x >= obj.rect.x) && (rect.x <= (obj.rect.x + obj.rect.width)) &&
			 (rect.y >= obj.rect.y) && (rect.y <= (obj.rect.y + obj.rect.height)) ) {
			obj.rect.x = rect.x;
			obj.rect.y = rect.y;
			obj.rect.width = rect.width;
			obj.rect.height = rect.height;
			break;
		}
	}
	*/
}

void CViewMain::set_obj_rect(GdkRectangle& rect, float x, float y) {
	//__LOGT__( TAG, "set_obj_rect()" );
	
	rect.x = (int)x;
	rect.y = (int)y;
}

void CViewMain::set_obj_rect(GdkRectangle& rect, float x, float y, float w, float h) {
	//__LOGT__( TAG, "set_obj_rect()" );
	
	rect.x = (int)x;
	rect.y = (int)y;
	rect.width = (int)w;
	rect.height = (int)h;
}

void CViewMain::del_obj_rect(e_ObjType_t type, float x, float y) {
	//__LOGT__( TAG, "del_obj_rect()" );
	
	/*
	std::vector<obj_rect_list_st>::iterator iter;

	for ( iter = m_vec_rect.begin(); iter != m_vec_rect.end(); ++iter ) {
		obj_rect_list_st obj = (*iter);

		if ( (x >= obj.rect.x) && (x <= (obj.rect.x + obj.rect.width)) &&
			 (y >= obj.rect.y) && (y <= (obj.rect.y + obj.rect.height)) ) {
			m_vec_rect.erase( iter );
			break;
		}
	}
	*/
}

bool CViewMain::is_obj_selected(GdkRectangle rect, float x, float y) {
	//__LOGT__( TAG, "is_obj_selected()" );
	
	if ( ((int)x >= rect.x) && ((int)x <= (rect.x + rect.width)) &&
		 ((int)y >= rect.y) && ((int)y <= (rect.y + rect.height)) )
		return true;

	return false;
}

e_ObjType_t CViewMain::get_obj_type(float x, float y) {
	//__LOGT__( TAG, "get_obj_type()" );
	
	e_ObjType_t ret = e_objType_UNKNOWN;
	/*
	std::vector<obj_rect_list_st>::iterator iter;

	for ( iter = m_vec_rect.begin(); iter != m_vec_rect.end(); ++iter ) {
		obj_rect_list_st obj = (*iter);

		if ( (x >= obj.rect.x) && (x <= (obj.rect.x + obj.rect.width)) &&
			 (y >= obj.rect.y) && (y <= (obj.rect.y + obj.rect.height)) ) {
			ret = obj.type;
			break;
		}
	}
	*/

	return ret;
}

// Temporary Font information
void CViewMain::set_tmp_font_info_select(bool select) {
	//__LOGT__( TAG, "set_tmp_font_info()" );
	
	m_tmp_obj_font_info.selected_font = select;
	m_tmp_obj_font_info.selected_color = select;
}

void CViewMain::set_tmp_font_info(const char* family_name, const e_ObjAttachFontTypeface_t typeface,
		const bool bold, const double size) {
	//__LOGT__( TAG, "set_tmp_font_info()" );
	
	memset( (void*)&m_tmp_obj_font_info.family_name, 0x00, sizeof(m_tmp_obj_font_info.family_name) );
	snprintf( m_tmp_obj_font_info.family_name,
				sizeof(m_tmp_obj_font_info.family_name), "%s", family_name );
	m_tmp_obj_font_info.typeface = typeface;
	m_tmp_obj_font_info.bold = bold;
	m_tmp_obj_font_info.size = size;

	m_tmp_obj_font_info.selected_font = true;
}

void CViewMain::set_tmp_font_info_color(const ColorARGB_st color) {
	//__LOGT__( TAG, "set_tmp_font_info_color()" );
	
	m_tmp_obj_font_info.color.a = color.a;
	m_tmp_obj_font_info.color.r = color.r;
	m_tmp_obj_font_info.color.g = color.g;
	m_tmp_obj_font_info.color.b = color.b;

	m_tmp_obj_font_info.selected_color = true;
}

void CViewMain::set_tmp_font_info_reset(void) {
	//__LOGT__( TAG, "set_tmp_font_info_reset()" );
	
	memset( (void*)&m_tmp_obj_font_info.family_name, 0x00, sizeof(m_tmp_obj_font_info.family_name) );
	snprintf( m_tmp_obj_font_info.family_name,
				sizeof(m_tmp_obj_font_info.family_name), "Sans" );
	m_tmp_obj_font_info.typeface = e_objAttachFontTypeface_NORMAL;
	m_tmp_obj_font_info.bold = false;
	m_tmp_obj_font_info.size = 0.f;
	m_tmp_obj_font_info.color = (ColorARGB_st) { PAINT_COLOR_UINT8_16(255), 0, 0, 0 };
	m_tmp_obj_font_info.selected_font = false;
	m_tmp_obj_font_info.selected_color = false;
}

// Temporary Background Image  information
void CViewMain::set_tmp_bg_img_info_select(bool select) {
	//__LOGT__( TAG, "set_tmp_bg_img_info_select()" );
	m_tmp_obj_bg_img_info.selected_bg_img = select;
}

void CViewMain::set_tmp_bg_img_info(const char* bg_img_filename) {
	//__LOGT__( TAG, "set_tmp_bg_img_info()" );
	
	m_tmp_obj_bg_img_info.selected_bg_img = false;

	if ( m_tmp_obj_bg_img_info.bg_img_filename ) {
		delete m_tmp_obj_bg_img_info.bg_img_filename;
		m_tmp_obj_bg_img_info.bg_img_filename = NULL;
	}

	if ( bg_img_filename ) {
		int len = (strlen(bg_img_filename) + 1);

		m_tmp_obj_bg_img_info.bg_img_filename = new char[len];
		memset ( (void*)m_tmp_obj_bg_img_info.bg_img_filename, 0x00, len ); 

		snprintf( m_tmp_obj_bg_img_info.bg_img_filename, len, "%s", bg_img_filename );

		m_tmp_obj_bg_img_info.selected_bg_img = true;
	}
}

void CViewMain::set_tmp_bg_img_info_release(void) {
	//__LOGT__( TAG, "set_tmp_bg_img_info_release()" );
	
	if ( m_tmp_obj_bg_img_info.bg_img_filename ) {
		delete m_tmp_obj_bg_img_info.bg_img_filename;
		m_tmp_obj_bg_img_info.bg_img_filename = NULL;
	}

	m_tmp_obj_bg_img_info.selected_bg_img = false;
}

bool CViewMain::attach_add_text(const char* text, const char* bg_img_filename,
		const char* tag) {
	//__LOGT__( TAG, "attach_add_text()" );
	
	if ( !m_pvec_attach ) {
		__LOGT__( TAG, "attach_add_text(): Attachment Container == NULL" );
		return false;
	}

	{
		CViewAttach* attach = new CViewAttach();
		e_ObjAttachType_t type = e_objAttachType_TEXT;

		if ( !attach || !text ) {
			if ( !attach )
				__LOGT__( TAG, "attach_add_text(): Attach == NULL" );

			if ( !text )
				__LOGT__( TAG, "attach_add_text(): Text == NULL" );

			if ( attach ) {
				delete attach;
				attach = NULL;
			}

			return false;
		}

		attach->set_obj_type( type );
		attach->set_tag2( tag );
		attach->set_obj_rect( 50.f, 50.f, 100.f, 100.f );
		attach->set_text( text );


		// Font
		if ( !m_tmp_obj_font_info.selected_font ) {
			attach->set_text_font_face();
			attach->set_text_font_size();
			attach->set_text_font_typeface_bold( false );
		}
		else {
			const char* family_name = m_tmp_obj_font_info.family_name;
			const e_ObjAttachFontTypeface_t typeface = m_tmp_obj_font_info.typeface;
			const bool bold = m_tmp_obj_font_info.bold;
			const double size = m_tmp_obj_font_info.size;

			// Font: "Sans Bold Italic 12"
			//
			// Family name
			attach->set_text_font_face( family_name );

			// Typefaces (style)
			attach->set_text_font_typeface( typeface );
			attach->set_text_font_typeface_bold( bold );

			// Size
			attach->set_text_font_size( size );


			// Reset
			//set_tmp_font_info_reset();
		}


		// Font color
		if ( !m_tmp_obj_font_info.selected_color ) {
			attach->set_text_font_color( e_objAttachPaintColor_BLACK );
		}
		else {
			const ColorARGB_st color = m_tmp_obj_font_info.color;
			const guint16 a = color.a;
			const guint16 r = color.r;
			const guint16 g = color.g;
			const guint16 b = color.b;


			//__LOGT__( TAG, "attach_add_text(): font face = %s", family_name );
			//attach->get_text_font_typeface_str( true );
			//__LOGT__( TAG, "attach_add_text(): bold = %s", (bold)? "true" : "false" );
			//__LOGT__( TAG, "attach_add_text(): size = %f", size );
			//__LOGT__( TAG, "attach_add_text(): color = A(%d), R(%d), G(%d), B(%d)", a, r, g, b );


			// Color
			attach->set_text_font_color_rgba( r, g, b, a );
		}


		// Background Image
		if ( !m_tmp_obj_bg_img_info.selected_bg_img ) {
			if ( bg_img_filename ) {
				if ( !attach->load_image(bg_img_filename) ) {
					if ( attach ) {
						delete attach;
						attach = NULL;
					}

					return false;
				}
			}
		}
		else {
			if ( m_tmp_obj_bg_img_info.bg_img_filename ) {
				if ( !attach->load_image(m_tmp_obj_bg_img_info.bg_img_filename) ) {
					if ( attach ) {
						delete attach;
						attach = NULL;
					}

					return false;
				}
			}
		}

		m_pvec_attach->push_back( attach );
	}


	return true;
}

bool CViewMain::attach_edit_text(const char* text, const char* bg_img_filename,
		const char* tag) {
	//__LOGT__( TAG, "attach_edit_text()" );
	
	if ( !m_pvec_attach ) {
		__LOGT__( TAG, "attach_edit_text(): Attachment Container == NULL" );
		return false;
	}

	{
		CViewAttach* attach = attach_selected();
		e_ObjAttachType_t type = e_objAttachType_TEXT;

		if ( !attach || !text ) {
			if ( !attach )
				__LOGT__( TAG, "attach_edit_text(): Attach == NULL" );

			if ( !text )
				__LOGT__( TAG, "attach_edit_text(): Text == NULL" );

			return false;
		}

		attach->set_obj_type( type );
		attach->set_tag2( tag );
		attach->set_text( text );

		/*
		if ( !m_tmp_obj_font_info.selected_font ) {
			attach->set_text_font_face();
			attach->set_text_font_size();
			attach->set_text_font_typeface_bold( false );
		}
		if ( !m_tmp_obj_font_info.selected_color ) {
			attach->set_text_font_color( e_objAttachPaintColor_BLACK );
		}
		*/

		if ( m_tmp_obj_font_info.selected_font ) {
			const char* family_name = m_tmp_obj_font_info.family_name;
			const e_ObjAttachFontTypeface_t typeface = m_tmp_obj_font_info.typeface;
			const bool bold = m_tmp_obj_font_info.bold;
			const double size = m_tmp_obj_font_info.size;


			// Font: "Sans Bold Italic 12"
			//
			// Family name
			attach->set_text_font_face( family_name );

			// Typefaces (style)
			attach->set_text_font_typeface( typeface );
			attach->set_text_font_typeface_bold( bold );

			// Size
			attach->set_text_font_size( size );


			// Reset
			//set_tmp_font_info_reset();
		}

		if ( m_tmp_obj_font_info.selected_color ) {
			const ColorARGB_st color = m_tmp_obj_font_info.color;
			const guint16 a = color.a;
			const guint16 r = color.r;
			const guint16 g = color.g;
			const guint16 b = color.b;


			//__LOGT__( TAG, "attach_edit_text(): font face = %s", family_name );
			//attach->get_text_font_typeface_str( true );
			//__LOGT__( TAG, "attach_edit_text(): bold = %s", (bold)? "true" : "false" );
			//__LOGT__( TAG, "attach_edit_text(): size = %f", size );
			//__LOGT__( TAG, "attach_edit_text(): color = A(%d), R(%d), G(%d), B(%d)", a, r, g, b );


			// Color
			attach->set_text_font_color_rgba( r, g, b, a );
		}


		if ( m_tmp_obj_bg_img_info.selected_bg_img ) {
			if ( m_tmp_obj_bg_img_info.bg_img_filename ) {
				attach->load_image( m_tmp_obj_bg_img_info.bg_img_filename );
			}
		}
		else {
			if ( bg_img_filename ) {
				attach->load_image( bg_img_filename );
			}
		}
	}


	return true;
}

bool CViewMain::attach_add_image(const char* img_filename, const char* tag) {
	//__LOGT__( TAG, "attach_add_image()" );
	
	if ( !m_pvec_attach ) {
		__LOGT__( TAG, "attach_add_image(): Attachment Container == NULL" );
		return false;
	}

	{
		CViewAttach* attach = new CViewAttach();
		e_ObjAttachType_t type = e_objAttachType_IMAGE;

		if ( !attach || !img_filename ) {
			if ( !attach )
				__LOGT__( TAG, "attach_add_image(): Attach == NULL" );

			if ( !img_filename )
				__LOGT__( TAG, "attach_add_image(): Image filename == NULL" );

			if ( attach ) {
				delete attach;
				attach = NULL;
			}

			return false;
		}

		//! TODO: stat ( img_filename )
		//

		attach->set_obj_type( type );
		attach->set_tag2( tag );
		attach->set_obj_rect( 50.f, 50.f, 100.f, 100.f );
		if ( !attach->load_image(img_filename) ) {
			if ( attach ) {
				delete attach;
				attach = NULL;
			}

			return false;
		}

		m_pvec_attach->push_back( attach );
	}


	return true;
}

bool CViewMain::attach_rotate(void) {
	//__LOGT__( TAG, "attach_rotate()" );
	
	bool ret = false;

	if ( !m_pvec_attach ) {
		__LOGT__( TAG, "attach_rotate(): Attachment Container == NULL" );
		return false;
	}

	if ( m_pvec_attach->size() > 0 ) {
		std::vector<CViewAttach*>::iterator iter;

		for ( iter = m_pvec_attach->begin(); iter != m_pvec_attach->end(); ++iter ) {
			CViewAttach* attach = (*iter);

			if ( attach ) {
				if ( attach->get_selected() ) {
					attach->set_obj_rotate( true );
					ret = true;

					break;
				}
			}
		} // for()
	}


	return ret;
}

bool CViewMain::attach_delete(void) {
	//__LOGT__( TAG, "attach_delete()" );
	
	bool ret = false;

	if ( !m_pvec_attach ) {
		__LOGT__( TAG, "attach_delete(): Attachment Container == NULL" );
		return false;
	}

	if ( m_pvec_attach->size() > 0 ) {
		std::vector<CViewAttach*>::iterator iter;

		for ( iter = m_pvec_attach->begin(); iter != m_pvec_attach->end(); ++iter ) {
			CViewAttach* attach = (*iter);

			if ( attach ) {
				if ( attach->get_selected() ) {
					delete attach;
					attach = NULL;

					m_pvec_attach->erase( iter );

					ret = true;

					break;
				}
			}
		} // for()
	}


	return ret;
}

bool CViewMain::attach_delete_all(void) {
	//__LOGT__( TAG, "attach_delete_all()" );
	
	if ( !m_pvec_attach ) {
		__LOGT__( TAG, "attach_delete_all(): Attachment Container == NULL" );
		return false;
	}

	if ( m_pvec_attach->size() > 0 ) {
		std::vector<CViewAttach*>::iterator iter;

		for ( iter = m_pvec_attach->begin(); iter != m_pvec_attach->end(); ) {
			CViewAttach* attach = (*iter);

			if ( attach ) {
				delete attach;
				attach = NULL;
			}

			iter = m_pvec_attach->erase( iter );
		} // for()

		m_pvec_attach->clear();
	}


	return true;
}

CViewAttach* CViewMain::attach_selected(void) {
	//__LOGT__( TAG, "attach_selected()" );
	
	CViewAttach* attach = NULL;

	if ( !m_pvec_attach ) {
		__LOGT__( TAG, "attach_selected(): Attachment Container == NULL" );
		return NULL;
	}

	if ( m_pvec_attach->size() > 0 ) {
		std::vector<CViewAttach*>::iterator iter;

		for ( iter = m_pvec_attach->begin(); iter != m_pvec_attach->end(); ++iter ) {
			attach = (*iter);

			if ( attach ) {
				if ( attach->get_selected() ) {
					break;
				}
			}

			attach = NULL;
		} // for()
	}


	return attach;
}

void CViewMain::attach_set_action(e_attachActionType_t type) {
	//__LOGT__( TAG, "attach_set_action()" );
	
	m_attach_action = type;
}

e_attachActionType_t CViewMain::attach_get_action(void) {
	//__LOGT__( TAG, "attach_get_action()" );
	
	return m_attach_action;
}

bool CViewMain::attach_invalidate(CBaseView* view) {
	//__LOGT__( TAG, "attach_invalidate()" );
	
	if ( !view ) {
		__LOGT__( TAG, "attach_invalidate(): CBaseView == NULL" );
		return false;
	}

	if ( !m_pvec_attach ) {
		__LOGT__( TAG, "attach_invalidate(): Attachment Container == NULL" );
		return false;
	}


	if ( m_pvec_attach->size() > 0 ) {
		std::vector<CViewAttach*>::iterator iter;

		for ( iter = m_pvec_attach->begin(); iter != m_pvec_attach->end(); ++iter ) {
			CViewAttach* attach = (*iter);

			if ( attach ) {
				if ( take_screenshot_get_region() ) {
					attach->set_select( false );
					attach->set_obj_rotate( false );
				}

				attach->invalidate( view );
			}
		} // for()
	}


	return true;
}

bool CViewMain::attach_touch_event(CKeyEvent* event) {
	//__LOGT__( TAG, "attach_touch_event()" );
	
	if ( !event ) {
		__LOGT__( TAG, "attach_touch_event(): CKeyEvent == NULL" );
		return false;
	}

	if ( !m_pvec_attach ) {
		__LOGT__( TAG, "attach_touch_event(): Attachment Container == NULL" );
		return false;
	}


	if ( m_pvec_attach->size() > 0 ) {
		std::vector<CViewAttach*>::reverse_iterator iter;
		bool found = false;

		for ( iter = m_pvec_attach->rbegin(); iter != m_pvec_attach->rend(); ++iter ) {
			CViewAttach* attach = (*iter);

			if ( attach ) {
				if ( found ) {
					attach->set_select( false );
					attach->set_obj_rotate( false );
					continue;
				}

				attach->onTouchEvent( event );

				if ( attach->get_selected() ) {
					//__LOGT__( TAG, "attach_touch_event(): tag2 = %s", attach->get_tag2() );
					found = true;
				}
			}
		} // for()
	}

	attach_bring_to_front();


	return true;
}

void CViewMain::attach_bring_to_front(void) {
	//__LOGT__( TAG, "attach_bring_to_front()" );
	
	if ( !m_pvec_attach ) {
		__LOGT__( TAG, "attach_bring_to_front(): Attachment Container == NULL" );
		return;
	}


	if ( m_pvec_attach->size() > 0 ) {
		std::vector<CViewAttach*>::reverse_iterator iter;
		bool found = false;

		for ( iter = m_pvec_attach->rbegin(); iter != m_pvec_attach->rend(); ++iter ) {
			CViewAttach* attach = (*iter);

			if ( attach ) {
				//__LOGT__( TAG, "attach_bring_to_front(): tag2 = %s", attach->get_tag2() );

				if ( attach->get_selected() ) {
					found = true;
					break;
				}
			}
		} // for()

		if ( found ) {
			//__LOGT__( TAG, "attach_bring_to_front(): move element to end" );

			{
				std::vector<CViewAttach*>::iterator iter_erased;
				CViewAttach* attach = (*iter);

				if ( !(*iter) ) {
					__LOGT__( TAG, "attach_bring_to_front(): iter == NULL" );
					return;
				}

				/*
				// Reverse iterator to Forward iterator
				//
				// reverse_iterator == (iterator - 1)
				-----------------------------------------
						[Reverse Iterator]
				-----------------------------------------
				rend()      r_iter[3] rbegin()[5]
				  |NULL          |     |
				  v              v     v
				.......[0][1][2][3][4][5]................
					    ^           ^      ^
					    |[0]        |[4]   |NULL
				      begin()    f_iter   end()
				-----------------------------------------
						[Forward Iterator]
				-----------------------------------------
				*/
				iter_erased = (--iter.base());
				if ( !(*iter_erased) ) {
					__LOGT__( TAG, "attach_bring_to_front(): iter.base() == NULL" );
					return;
				}

				m_pvec_attach->erase( iter_erased );

				if ( attach ) {
					m_pvec_attach->push_back( attach );
				}
				else {
					__LOGT__( TAG, "attach_bring_to_front(): move to end [FALSE]" );
					return;
				}
			}
		}

		/*
		__LOGT__( TAG, "attach_bring_to_front(): ----- check -----" );
		for ( iter = m_pvec_attach->rbegin(); iter != m_pvec_attach->rend(); ++iter ) {
			CViewAttach* attach = (*iter);

			if ( attach ) {
				__LOGT__( TAG, "attach_bring_to_front(): tag2 = %s, selectd = %s",
						attach->get_tag2(), (attach->get_selected()? "True" : "False") );
			}
		} // for()
		*/
	}
}
// ---------------------------------------------------------------



//! Implementation
// ---------------------------------------------------------------
// Global variable

namespace g_Func {
} // namespace g_Func

namespace g_FuncSignalHandler {
} // namespace g_FuncSignalHandler


#if 0
// class CASyncTask [
CAsyncTask::CAsyncTask(void) : TAG("CAsyncTask") {
	__LOGT__( TAG, "CAsyncTask()" );

	m_pSpinner = NULL;
}

CAsyncTask::~CAsyncTask(void) {
	__LOGT__( TAG, "~CAsyncTask()" );
}

void* CAsyncTask::inBackground(std::vector<void*>* pvecVal) {
	__LOGT__( TAG, "inBackground()" );

	// TEST
	/*
	for ( int i = 0; i < 10; i++ ) {
		__LOGT__( TAG, "inBackground(): i = %d", i );
		update( (void*)"#---update---#" );
	}
	__LOGT__( TAG, "" );

	if ( pvecVal && (pvecVal->size() > 0) ) {
		std::vector<void*>::iterator iter;
		//for ( iter = pvecVal->begin(); iter != pvecVal->end(); iter++ ) {
		//	__LOGT__( TAG, "inBackground(): str -> %s", (char*)(*iter) );
		//}

		__LOGT__( TAG, "inBackground(): str[0] -> %s", (char*)(*pvecVal)[0] );
		__LOGT__( TAG, "inBackground(): str[1] -> %s", (char*)(*pvecVal)[1] );
		__LOGT__( TAG, "inBackground(): str[2] -> %d", (int)(*pvecVal)[2] );
		__LOGT__( TAG, "inBackground(): str[3] -> %s", (char*)(*pvecVal)[3] );
	}


	//return NULL;
	return ((void*)true);
	*/



	return ((void*)true);
}

void CAsyncTask::progressUpdate(void* pVal) {
	__LOGT__( TAG, "progressUpdate()" );

	char* pStr = reinterpret_cast<char*>(pVal);
	if ( pStr )
		__LOGT__( TAG, "%s", pStr );
}

void CAsyncTask::postExecute(void* pResult, void* pExtraVal) {
	__LOGT__( TAG, "postExecute()" );

	int result = reinterpret_cast<int>(pResult);
	__LOGT__( TAG, "postExecute(): result = %s", (result? "TRUE" : "FALSE") );

	if ( pExtraVal ) {
		GtkWidget* pDlg = NULL;
		pDlg = GTK_WIDGET( pExtraVal );
		
		//! DO NOT USE THE FOLLOWING
		/*
		//GtkSpinner* pSpinner = ((Widgets_st*)pExtraVal)->pSpinner;
		if ( m_pSpinner ) {
			__LOGT__( TAG, "postExecute(): Stop Spinner" );
			gtk_spinner_stop( m_pSpinner );
			m_pSpinner = NULL;
		}

		if ( pDlg ) {
			__LOGT__( TAG, "postExecute(): Destroy Dialog" );
			gtk_widget_hide( pDlg );
			gtk_widget_destroy( pDlg );
			pDlg = NULL;
		}
		*/

		if ( pDlg ) {
			__LOGT__( TAG, "postExecute(): Send a signal to Destroy Dialog" );
			gtk_dialog_response( GTK_DIALOG(pDlg), GTK_RESPONSE_NONE );
		}
	}
}
// class CASyncTask ]
#endif
// ---------------------------------------------------------------

