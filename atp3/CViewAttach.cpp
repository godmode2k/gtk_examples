/* --------------------------------------------------------------
Project:	ATP(A Tentative Plan) 3
Purpose:	
Author:		Ho-Jung Kim (godmode2k@hotmail.com)
Date:		Since Dec 2, 2014
Filename:	CViewAttach.cpp

Last modified: Jan 30, 2015
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
#include "CViewAttach.h"
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
CViewAttach::CViewAttach(void) : TAG("CViewAttach") {
	__LOGT__( TAG, "CViewAttach()" );

	__init();
}

// Dtor
CViewAttach::~CViewAttach() {
	__LOGT__( TAG, "~CViewAttach()" );

	__release();
}

// Initialize
void CViewAttach::__init(void) {
	__LOGT__( TAG, "__init()" );

	//m_canvas = NULL;
	
	// Object type
	m_obj_type = e_objAttachType_UNKNOWN;
	m_selected = false;
	TAG2 = NULL;

	// Text
	m_text = NULL;
	m_text_font_size = DEFAULT_TEXT_FONT_SIZE;
	//m_text_font_color = (ColorARGB_st) { PAINT_COLOR_UINT8_16(255), 0, 0, 0 };
	m_text_font_color.a = PAINT_COLOR_UINT8_16(255);
	m_text_font_color.r = 0;
	m_text_font_color.g = 0;
	m_text_font_color.b = 0;
	m_text_font_face = NULL;
	m_text_font_typeface = e_objAttachFontTypeface_NORMAL;
	m_text_font_bold = false;

	// Image
	m_image = NULL;
	
	// Touch point, direction
	m_touchX = m_touchY = 0.f;
	m_direction = e_objAttachDirection_UNKNOWN;

	// Object rectangle
	m_rect.x = 50;
	m_rect.y = 50;
	m_rect.width = 100;
	m_rect.height = 100;



	// --------------------
	//load_image( "./test.png" );
}

// Release
void CViewAttach::__release(void) {
	__LOGT__( TAG, "__release()" );

	//g_slice_free( Widgets_st, m_pWidgets );
	
	if ( m_text ) {
		delete m_text;
		m_text = NULL;
	}

	if ( m_text_font_face ) {
		delete m_text_font_face;
		m_text_font_face = NULL;
	}

	if ( m_image ) {
		cairo_surface_destroy( m_image );
		m_image = NULL;
	}
}

#if 0
// Initialize Widget structure
bool CViewAttach::init_widget_all(Widgets_st** pWidgets) {
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
bool CViewAttach::init_ui_with_callback(void) {
	__LOGT__( TAG, "init_ui_with_callback()" );

	return true;
}
#endif



// ---------------------------------------------------------------
/*
void CViewAttach::set_canvas(canvas_t* canvas) {
	__LOGT__( TAG, "set_canvas()" );

	m_canvas = canvas;
}
*/

/*
int CViewAttach::get_width(void) {
	__LOGT__( TAG, "get_width()" );

	int val = 0;
	return val;
}

int CViewAttach::get_height(void) {
	__LOGT__( TAG, "get_height()" );

	int val = 0;
	return val;
}
*/



// Draw
// ---------------------------------------------------------------
void CViewAttach::onDraw(CBaseView* view) {
	//__LOGT__( TAG, "onDraw()" );

	draw( view );
}

/*
void CViewAttach::invalidate(void) {
	//__LOGT__( TAG, "invalidate()" );

	_invalidate();
	onDraw( this );
}

void CViewAttach::invalidate(CBaseView* view) {
	//__LOGT__( TAG, "invalidate()" );

	// Parent's view
	//
	//set_canvas( view->get_canvas() );
	set_width( view->get_width() );
	set_height( view->get_height() );
	//view->_invalidate();
	
	onDraw( view );
}

//void CViewAttach::invalidate_rect(float left, float top, float right, float bottom) {
//	__LOGT__( TAG, "invalidate_rect()" );
//}
*/

void CViewAttach::draw(CBaseView* view) {
	//__LOGT__( TAG, "draw()" );

	if ( !view ) {
		__LOGT__( TAG, "draw(): CBaseView == NULL" );
		return;
	}

	//__LOGT__( TAG, "draw(): display width = %d, height = %d",
	//			view->get_display_width(), view->get_display_height() );
	//__LOGT__( TAG, "draw(): width = %d, height = %d", get_width(), get_height() );



	// TODO:

	//set_canvas( view->get_canvas() );
	//canvas_t* canvas = get_canvas();
	
	{
		//canvas_t* canvas = view->get_canvas();
		canvas_pt canvas = view->get_canvas();

		if ( !canvas ) {
			__LOGT__( TAG, "draw(): CBaseView == NULL" );
			return;
		}


		// ...
		draw_obj( canvas );
	}
}



// Touch Event
// ---------------------------------------------------------------
bool CViewAttach::onTouchEvent(CKeyEvent* event) {
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

				//! SEE CViewMain.cpp:: onTouchEvent()
				/*
				if ( !(event->is_mouse_lbtn() || event->is_mouse_rbtn() ||
						event->is_mouse_wbtn()) ) {
					return false;
				}
				*/
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

void CViewAttach::onTouchEventDown(CKeyEvent* event, float x, float y) {
	//__LOGT__( TAG, "onTouchEventDown()" );

	//__LOGT__( TAG, "onTouchEventDown(): x = %f, y = %f", x, y );


	m_touchX = x;
	m_touchY = y;

	m_direction = is_obj_selected_direction( m_rect, m_touchX, m_touchY );
}

void CViewAttach::onTouchEventUp(CKeyEvent* event, float x, float y) {
	//__LOGT__( TAG, "onTouchEventUp()" );

	//__LOGT__( TAG, "onTouchEventUp(): x = %f, y = %f", x, y );
	
	//set_select( false );
}

void CViewAttach::onTouchEventMove(CKeyEvent* event, float x, float y) {
	//__LOGT__( TAG, "onTouchEventMove()" );
	
	//__LOGT__( TAG, "onTouchEventMove(): x = %f, y = %f", x, y );

	if ( event->is_mouse_lbtn() && get_selected() ) {
		update_position( m_direction, x, y );
	}

	//__LOGT__( TAG, "onTouchEventMove(): attach x = %d, y = %d, w = %d, h = %d",
	//			m_rect.x, m_rect.y, m_rect.width, m_rect.height );
}

// ---------------------------------------------------------------

void CViewAttach::set_obj_type(e_ObjAttachType_t type) {
	//__LOGT__( TAG, "set_obj_type()" );
	
	m_obj_type = type;
}

e_ObjAttachType_t CViewAttach::get_obj_type(void) {
	//__LOGT__( TAG, "get_obj_type()" );
	
	return m_obj_type;
}

void CViewAttach::set_select(bool select) {
	//__LOGT__( TAG, "set_select()" );
	
	m_selected = select;
}

bool CViewAttach::get_selected(void) {
	//__LOGT__( TAG, "get_selected()" );
	
	return m_selected;
}

void CViewAttach::set_obj_rect(GdkRectangle& rect, float x, float y) {
	//__LOGT__( TAG, "set_obj_rect()" );
	
	rect.x = (int)x;
	rect.y = (int)y;
}

void CViewAttach::set_obj_rect(GdkRectangle& rect, float x, float y, float w, float h) {
	//__LOGT__( TAG, "set_obj_rect()" );
	
	rect.x = (int)x;
	rect.y = (int)y;
	rect.width = (int)w;
	rect.height = (int)h;
}

void CViewAttach::set_obj_rect(float x, float y, float w, float h) {
	//__LOGT__( TAG, "set_obj_rect()" );
	
	m_rect.x = (int)x;
	m_rect.y = (int)y;
	m_rect.width = (int)w;
	m_rect.height = (int)h;
}

bool CViewAttach::is_obj_selected(GdkRectangle rect, float x, float y) {
	//__LOGT__( TAG, "is_obj_selected()" );
	
	if ( ((int)x >= rect.x) && ((int)x <= (rect.x + rect.width)) &&
		 ((int)y >= rect.y) && ((int)y <= (rect.y + rect.height)) )
		return true;

	return false;
}

void CViewAttach::get_str_direction(e_ObjAttachDirection_t direction) {
	//__LOGT__( TAG, "get_str_direction()" );
	
	switch ( direction ) {
		/*
		case e_objAttachDirection_UNKNOWN:
			{} break;
		case e_objAttachDirection_RESERVED:
			{} break;
		*/
		case e_objAttachDirection_LEFT:
			{
				__LOGT__( TAG, "get_str_direction(): LEFT" );
			} break;
		case e_objAttachDirection_TOP:
			{
				__LOGT__( TAG, "get_str_direction(): TOP" );
			} break;
		case e_objAttachDirection_RIGHT:
			{
				__LOGT__( TAG, "get_str_direction(): RIGHT" );
			} break;
		case e_objAttachDirection_BOTTOM:
			{
				__LOGT__( TAG, "get_str_direction(): BOTTOM" );
			} break;
		case e_objAttachDirection_LEFT_TOP:
			{
				__LOGT__( TAG, "get_str_direction(): LEFT_TOP" );
			} break;
		case e_objAttachDirection_RIGHT_TOP:
			{
				__LOGT__( TAG, "get_str_direction(): RIGHT_TOP" );
			} break;
		case e_objAttachDirection_LEFT_BOTTOM:
			{
				__LOGT__( TAG, "get_str_direction(): LEFT_BOTTOM" );
			} break;
		case e_objAttachDirection_RIGHT_BOTTOM:
			{
				__LOGT__( TAG, "get_str_direction(): RIGHT_BOTTOM" );
			} break;
		case e_objAttachDirection_CENTER:
			{
				__LOGT__( TAG, "get_str_direction(): CENTER" );
			} break;
		case e_objAttachDirection_LEFT_CENTER:
			{
				__LOGT__( TAG, "get_str_direction(): LEFT_CENTER" );
			} break;
		case e_objAttachDirection_TOP_CENTER:
			{
				__LOGT__( TAG, "get_str_direction(): TOP_CENTER" );
			} break;
		case e_objAttachDirection_RIGHT_CENTER:
			{
				__LOGT__( TAG, "get_str_direction(): RIGHT_CENTER" );
			} break;
		case e_objAttachDirection_BOTTOM_CENTER:
			{
				__LOGT__( TAG, "get_str_direction(): BOTTOM_CENTER" );
			} break;
		default:
			{
				__LOGT__( TAG, "get_str_direction(): UNKNOWN" );
			} break;
	}
}

e_ObjAttachDirection_t CViewAttach::is_obj_selected_direction(GdkRectangle rect, float x, float y) {
	//__LOGT__( TAG, "is_obj_selected_direction()" );
	
	const int collision_padding = DEFAULT_DRAW_CIRCLE_RADIUS;
	e_ObjAttachDirection_t ret = e_objAttachDirection_UNKNOWN;


	// LEFT-TOP
	if ( ((int)x >= (rect.x - collision_padding)) &&
			  ((int)x <= (rect.x + collision_padding)) &&
		 	  ((int)y >= (rect.y - collision_padding)) &&
			  ((int)y <= (rect.y + collision_padding)) ) {
		ret = e_objAttachDirection_LEFT_TOP;
	}
	// RIGHT-TOP
	else if ( ((int)x >= ((rect.x + rect.width) - collision_padding)) &&
			  ((int)x <= ((rect.x + rect.width) + collision_padding)) &&
		 	  ((int)y >= (rect.y - collision_padding)) &&
			  ((int)y <= (rect.y + collision_padding)) ) {
		ret = e_objAttachDirection_RIGHT_TOP;
	}
	// LEFT-BOTTOM
	else if ( ((int)x >= (rect.x - collision_padding)) &&
			  ((int)x <= (rect.x + collision_padding)) &&
		 	  ((int)y >= ((rect.y + rect.height) - collision_padding)) &&
			  ((int)y <= ((rect.y + rect.height) + collision_padding)) ) {
		ret = e_objAttachDirection_LEFT_BOTTOM;
	}
	// RIGHT-BOTTOM
	else if ( ((int)x >= ((rect.x + rect.width) - collision_padding)) &&
			  ((int)x <= ((rect.x + rect.width) + collision_padding)) &&
		 	  ((int)y >= ((rect.y + rect.height) - collision_padding)) &&
			  ((int)y <= ((rect.y + rect.height) + collision_padding)) ) {
		ret = e_objAttachDirection_RIGHT_BOTTOM;
	}
	// LEFT-CENTER
	else if ( ((int)x >= (rect.x - collision_padding)) &&
			  ((int)x <= (rect.x + collision_padding)) &&
		 	  ((int)y >= ((rect.y + (rect.height >> 1)) - collision_padding)) &&
			  ((int)y <= ((rect.y + (rect.height >> 1)) + collision_padding)) ) {
		ret = e_objAttachDirection_LEFT_CENTER;
	}
	// RIGHT-CENTER
	else if ( ((int)x >= ((rect.x + rect.width) - collision_padding)) &&
			  ((int)x <= ((rect.x + rect.width) + collision_padding)) &&
		 	  ((int)y >= ((rect.y + (rect.height >> 1)) - collision_padding)) &&
			  ((int)y <= ((rect.y + (rect.height >> 1)) + collision_padding)) ) {
		ret = e_objAttachDirection_RIGHT_CENTER;
	}
	// TOP-CENTER
	else if ( ((int)x >= ((rect.x + (rect.width >> 1)) - collision_padding)) &&
			  ((int)x <= ((rect.x + (rect.width >> 1)) + collision_padding)) &&
		 	  ((int)y >= (rect.y - collision_padding)) &&
			  ((int)y <= (rect.y + collision_padding)) ) {
		ret = e_objAttachDirection_TOP_CENTER;
	}
	// BOTTOM-CENTER
	else if ( ((int)x >= ((rect.x + (rect.width >> 1)) - collision_padding)) &&
			  ((int)x <= ((rect.x + (rect.width >> 1)) + collision_padding)) &&
		 	  ((int)y >= ((rect.y + rect.height) - collision_padding)) &&
			  ((int)y <= ((rect.y + rect.height) + collision_padding)) ) {
		ret = e_objAttachDirection_BOTTOM_CENTER;
	}
	// CENTER
	else if ( ((int)x >= rect.x) && ((int)x <= (rect.x + rect.width)) &&
		 ((int)y >= rect.y) && ((int)y <= (rect.y + rect.height)) ) {
		ret = e_objAttachDirection_CENTER;
	}


	// Selected
	if ( (ret == e_objAttachDirection_UNKNOWN) || (ret == e_objAttachDirection_RESERVED) )
		set_select( false );
	else
		set_select( true );

	//__LOGT__( TAG, "is_obj_selected_direction(): selected = %s", (get_selected()? "TRUE" : "FALSE") );


	return ret;
}

void CViewAttach::update_position(e_ObjAttachDirection_t direction, float x, float y) {
	//__LOGT__( TAG, "update_position()" );
	
	//__LOGT__( TAG, "update_position(): x = %f, y = %f", x, y );
	//get_str_direction( m_direction );

	{
		//const int collision_padding = DEFAULT_DRAW_CIRCLE_RADIUS;
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
					//if ( ((m_rect.y + m_rect.height) - y) < collision_padding ) {
					//	return;
					//}

					if ( m_rect.y > y ) {
						pos_y = (m_rect.y - y);
						m_rect.y -= (int)pos_y;
						m_rect.height += (int)pos_y;
					}
					else if ( m_rect.y < y ) {
						pos_y = (y - m_rect.y);
						m_rect.y += (int)pos_y;
						m_rect.height -= (int)pos_y;
					}

					// LEFT
					//if ( ((m_rect.x + m_rect.width) - x) < collision_padding ) {
					//	return;
					//}

					if ( m_rect.x > x ) {
						pos_x = (m_rect.x - x);
						m_rect.x -= (int)pos_x;
						m_rect.width += (int)pos_x;
					}
					else if ( m_rect.x < x ) {
						pos_x = (x - m_rect.x);
						m_rect.x += (int)pos_x;
						m_rect.width -= (int)pos_x;
					}
				} break;
			case e_objAttachDirection_RIGHT_TOP:
				{
					// TOP
					//if ( ((m_rect.y + m_rect.height) - y) < collision_padding ) {
					//	return;
					//}

					if ( m_rect.y > y ) {
						pos_y = (m_rect.y - y);
						m_rect.y -= (int)pos_y;
						m_rect.height += (int)pos_y;
					}
					else if ( m_rect.y < y ) {
						pos_y = (y - m_rect.y);
						m_rect.y += (int)pos_y;
						m_rect.height -= (int)pos_y;
					}

					// RIGHT
					//if ( (x - m_rect.x) < collision_padding ) {
					//	return;
					//}

					if ( (m_rect.x + m_rect.width) > x ) {
						pos_x = (m_rect.x + m_rect.width) - x;
						m_rect.width -= (int)pos_x;
					}
					else if ( (m_rect.x + m_rect.width) < x ) {
						pos_x = x - (m_rect.x + m_rect.width);
						m_rect.width += (int)pos_x;
					}
				} break;
			case e_objAttachDirection_LEFT_BOTTOM:
				{
					// BOTTOM
					//if ( (y - m_rect.y) < collision_padding ) {
					//	return;
					//}

					if ( (m_rect.y + m_rect.height) > y ) {
						pos_y = (m_rect.y + m_rect.height) - y;
						m_rect.height -= (int)pos_y;
					}
					else if ( m_rect.height < y ) {
						pos_y = y - (m_rect.y + m_rect.height);
						m_rect.height += (int)pos_y;
					}

					// LEFT
					//if ( ((m_rect.x + m_rect.width) - x) < collision_padding ) {
					//	return;
					//}

					if ( m_rect.x > x ) {
						pos_x = (m_rect.x - x);
						m_rect.x -= (int)pos_x;
						m_rect.width += (int)pos_x;
					}
					else if ( m_rect.x < x ) {
						pos_x = (x - m_rect.x);
						m_rect.x += (int)pos_x;
						m_rect.width -= (int)pos_x;
					}
				} break;
			case e_objAttachDirection_RIGHT_BOTTOM:
				{
					// BOTTOM
					//if ( (y - m_rect.y) < collision_padding ) {
					//	return;
					//}

					if ( (m_rect.y + m_rect.height) > y ) {
						pos_y = (m_rect.y + m_rect.height) - y;
						m_rect.height -= (int)pos_y;
					}
					else if ( m_rect.height < y ) {
						pos_y = y - (m_rect.y + m_rect.height);
						m_rect.height += (int)pos_y;
					}

					// RIGHT
					//if ( (x - m_rect.x) < collision_padding ) {
					//	return;
					//}

					if ( (m_rect.x + m_rect.width) > x ) {
						pos_x = (m_rect.x + m_rect.width) - x;
						m_rect.width -= (int)pos_x;
					}
					else if ( (m_rect.x + m_rect.width) < x ) {
						pos_x = x - (m_rect.x + m_rect.width);
						m_rect.width += (int)pos_x;
					}

				} break;
			
			// CENTER
			case e_objAttachDirection_CENTER:
				{
					if ( m_touchX > x ) {
						pos_x = (m_touchX - x);
						m_rect.x -= (int)pos_x;
						m_touchX -= (int)pos_x;
					}
					else if ( m_touchX < x ) {
						pos_x = (x - m_touchX);
						m_rect.x += (int)pos_x;
						m_touchX += (int)pos_x;
					}
					if ( m_touchY > y ) {
						pos_y = (m_touchY - y);
						m_rect.y -= (int)pos_y;
						m_touchY -= (int)pos_y;
					}
					else if ( m_touchY < y ) {
						pos_y = (y - m_touchY);
						m_rect.y += (int)pos_y;
						m_touchY += (int)pos_y;
					}
				} break;
			case e_objAttachDirection_LEFT_CENTER:
				{
					//if ( ((m_rect.x + m_rect.width) - x) < collision_padding ) {
					//	return;
					//}

					if ( m_rect.x > x ) {
						pos_x = (m_rect.x - x);
						m_rect.x -= (int)pos_x;
						m_rect.width += (int)pos_x;
					}
					else if ( m_rect.x < x ) {
						pos_x = (x - m_rect.x);
						m_rect.x += (int)pos_x;
						m_rect.width -= (int)pos_x;
					}
				} break;
			case e_objAttachDirection_TOP_CENTER:
				{
					//if ( ((m_rect.y + m_rect.height) - y) < collision_padding ) {
					//	return;
					//}

					if ( m_rect.y > y ) {
						pos_y = (m_rect.y - y);
						m_rect.y -= (int)pos_y;
						m_rect.height += (int)pos_y;
					}
					else if ( m_rect.y < y ) {
						pos_y = (y - m_rect.y);
						m_rect.y += (int)pos_y;
						m_rect.height -= (int)pos_y;
					}
				} break;
			case e_objAttachDirection_RIGHT_CENTER:
				{
					//if ( ((x - m_rect.x) < collision_padding ) {
					//	return;
					//}

					if ( (m_rect.x + m_rect.width) > x ) {
						pos_x = (m_rect.x + m_rect.width) - x;
						m_rect.width -= (int)pos_x;
					}
					else if ( (m_rect.x + m_rect.width) < x ) {
						pos_x = x - (m_rect.x + m_rect.width);
						m_rect.width += (int)pos_x;
					}
				} break;
			case e_objAttachDirection_BOTTOM_CENTER:
				{
					//if ( (y - m_rect.y) < collision_padding ) {
					//	return;
					//}

					if ( (m_rect.y + m_rect.height) > y ) {
						pos_y = (m_rect.y + m_rect.height) - y;
						m_rect.height -= (int)pos_y;
					}
					else if ( m_rect.height < y ) {
						pos_y = y - (m_rect.y + m_rect.height);
						m_rect.height += (int)pos_y;
					}
				} break;
			default:
				{
				} break;
		}
	}
}

GdkRectangle CViewAttach::get_rect(void) {
	//__LOGT__( TAG, "get_rect()" );
	
	return m_rect;
}

void CViewAttach::draw_paint_color(canvas_t* canvas, double r, double g, double b, double a) {
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

void CViewAttach::draw_paint_color_fraction(canvas_t* canvas,
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

void CViewAttach::draw_paint_color(canvas_t* canvas, bool fraction, ColorARGB_st color) {
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

void CViewAttach::draw_paint_color(canvas_t* canvas, bool fraction,
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

void CViewAttach::draw_circle(canvas_t* canvas, float x, float y, double radius, bool fill) {
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

bool CViewAttach::load_image(const char* filename) {
	//__LOGT__( TAG, "load_image()" );
	
	if ( !filename ) {
		__LOGT__( TAG, "load_image(): filename == NULL" );
		return false;
	}

	if ( m_image ) {
		cairo_surface_destroy( m_image );
		m_image = NULL;
	}

	/*
	// PNG Only
	{
		m_image = cairo_image_surface_create_from_png( filename );

		cairo_status_t status = cairo_surface_status( m_image );
		switch ( status ) {
			case CAIRO_STATUS_NO_MEMORY:
			case CAIRO_STATUS_FILE_NOT_FOUND:
			case CAIRO_STATUS_READ_ERROR:
				{
					__LOGT__( TAG, "load_image(): Load an Image: "
								"CAIRO_STATUS_NO_MEMORY, CAIRO_STATUS_FILE_NOT_FOUND, "
								"CAIRO_STATUS_READ_ERROR" );
					__LOGT__( TAG, "load_image(): Load an Image, status = %d [FALSE]", status );
				} break;
		}
		if ( status != CAIRO_STATUS_SUCCESS ) {
			__LOGT__( TAG, "load_image(): Load an Image, status = %d [FALSE]", status );
			return false;
		}

		//int w = cairo_image_surface_get_width( m_image );
		//int h = cairo_image_surface_get_height( m_image );
		//cairo_translate( canvas, 0, 0 );
		//cairo_scale( canvas, 256.0/w, 256.0/h );

		//cairo_set_source_surface( canvas, m_image, 0, 0 );
		//cairo_paint( canvas );
		//cairo_surface_destroy( m_image );
	}
	*/


	// All formats
	{
		//! Non-PNG
		GdkPixbuf* new_image = NULL;
		cairo_t* new_cr = NULL;
		int new_width = 0;
		int new_height = 0;


		// Screenshot example
		//gdk_drawable_get_size(GDK_DRAWABLE(GdkWindow), &width, &height );
		//new_image = gdk_pixbuf_get_from_drawable( NULL, GDK_DRAWABLE(GdkWindow),
		//				NULL, 0, 0, 0, 0, width, height );


		new_image = gdk_pixbuf_new_from_file( filename, NULL );
		if ( !new_image ) {
			__LOGT__( TAG, "load_image(): Load a new Image == NULL" );
			return false;
		}

		new_width = gdk_pixbuf_get_width( new_image );
		new_height = gdk_pixbuf_get_height( new_image );

		m_image = cairo_image_surface_create( CAIRO_FORMAT_ARGB32, new_width, new_height );
		if ( !m_image ) {
			__LOGT__( TAG, "load_image(): Image == NULL" );
			return false;
		}

		new_cr = cairo_create( m_image );
		if ( !new_cr ) {
			__LOGT__( TAG, "load_image(): Image == NULL" );
			return false;
		}

		gdk_cairo_set_source_pixbuf( new_cr, new_image, 0, 0 );
		cairo_paint( new_cr );
		//cairo_surface_write_to_png( m_image, "output.png" );


		// Release
		if ( new_cr ) {
			cairo_destroy( new_cr );
			new_cr = NULL;
		}
	}
	


	if ( !m_image ) {
		__LOGT__( TAG, "load_image(): Image == NULL" );
	}


	return (m_image != NULL);
}

bool CViewAttach::get_has_image(void) {
	//__LOGT__( TAG, "get_has_image()" );
	
	return (m_image != NULL);
}

void CViewAttach::draw_image(canvas_t* canvas) {
	//__LOGT__( TAG, "draw_image()" );
	
	if ( !canvas ) {
		__LOGT__( TAG, "draw_image(): canvas_t == NULL" );
		return;
	}

	if ( !m_image ) {
		__LOGT__( TAG, "draw_image(): cairo_surface_t == NULL" );
		return;
	}

	cairo_save( canvas );
	{
		int w = cairo_image_surface_get_width( m_image );
		int h = cairo_image_surface_get_height( m_image );

		cairo_translate( canvas, 0, 0 );
		cairo_scale( canvas, 256.0/w, 256.0/h );

		cairo_set_source_surface( canvas, m_image, 0, 0 );
		cairo_paint( canvas );
	}
	cairo_restore( canvas );
}

void CViewAttach::draw_image(canvas_t* canvas, const GdkRectangle rect) {
	//__LOGT__( TAG, "draw_image()" );
	
	draw_image( canvas, (double)rect.x, (double)rect.y,
				(double) rect.width, (double)rect.height );
}

void CViewAttach::draw_image(canvas_t* canvas, double x, double y, double w, double h) {
	//__LOGT__( TAG, "draw_image()" );
	
	if ( !canvas ) {
		__LOGT__( TAG, "draw_image(): canvas_t == NULL" );
		return;
	}

	if ( !m_image ) {
		__LOGT__( TAG, "draw_image(): cairo_surface_t == NULL" );
		return;
	}

	cairo_save( canvas );
	{
		int img_w = cairo_image_surface_get_width( m_image );
		int img_h = cairo_image_surface_get_height( m_image );

		cairo_translate( canvas, x, y );
		cairo_scale( canvas, w/img_w, h/img_h );

		cairo_set_source_surface( canvas, m_image, 0, 0 );
		cairo_paint( canvas );
	}
	cairo_restore( canvas );
}

void CViewAttach::set_text(const char* text) {
	//__LOGT__( TAG, "set_text()" );
	
	int len = ( strlen(text) + 1 );

	if ( m_text ) {
		delete m_text;
		m_text = NULL;
	}

	m_text = new char[len];

	if ( m_text ) {
		memset( (void*)m_text, 0x00, len );
		snprintf( m_text, len, "%s", text );
	}
}

const char* CViewAttach::get_text(void) {
	//__LOGT__( TAG, "get_text()" );
	
	return m_text;
}

void CViewAttach::set_text_font_size(double size) {
	//__LOGT__( TAG, "set_text_font_size()" );
	
	m_text_font_size = size;
}

double CViewAttach::get_text_font_size(void) {
	//__LOGT__( TAG, "get_text_font_size()" );
	
	return (m_text_font_size > 0)? m_text_font_size : DEFAULT_TEXT_FONT_SIZE;
}

double get_text_font_size_default(void) {
	//__LOGT__( TAG, "get_text_font_size_default()" );
	
	return DEFAULT_TEXT_FONT_SIZE;
}

void CViewAttach::set_text_font_color_rgba(guint16 r, guint16 g, guint16 b, guint16 a) {
	//__LOGT__( TAG, "set_text_font_color_rgba()" );
	
	//m_text_font_color = (ColorARGB_st) { a, r, g, b };
	
	m_text_font_color.a = a;
	m_text_font_color.r = r;
	m_text_font_color.g = g;
	m_text_font_color.b = b;
}

void CViewAttach::set_text_font_color(ColorARGB_st color) {
	//__LOGT__( TAG, "set_text_font_color()" );
	
	//m_text_font_color = color;
	
	m_text_font_color.a = color.a;
	m_text_font_color.r = color.r;
	m_text_font_color.g = color.g;
	m_text_font_color.b = color.b;
}

void CViewAttach::set_text_font_color(e_ObjAttachPaintColor_t color, guint16 a) {
	//__LOGT__( TAG, "set_text_font_color()" );
	
	guint16 r, g, b;

	PAINT_COLOR_RGBA_16( color, &r, &g, &b );
	//m_text_font_color = (ColorARGB_st) { a, r, g, b };
	
	m_text_font_color.a = a;
	m_text_font_color.r = r;
	m_text_font_color.g = g;
	m_text_font_color.b = b;
}

ColorARGB_st CViewAttach::get_text_font_color(void) {
	//__LOGT__( TAG, "get_text_font_color()" );
	
	return m_text_font_color;
}

void CViewAttach::set_text_font_face(const char* font_family) {
	//__LOGT__( TAG, "set_text_font_face()" );
	
	if ( m_text_font_face ) {
		delete m_text_font_face;
		m_text_font_face = NULL;
	}

	m_text_font_face = new char[DEFAULT_TEXT_FONT_FAMILY_SIZE];

	if ( m_text_font_face ) {
		memset( (void*)m_text_font_face, 0x00, DEFAULT_TEXT_FONT_FAMILY_SIZE );
		snprintf( m_text_font_face, DEFAULT_TEXT_FONT_FAMILY_SIZE, "%s", font_family );
	}
}

const char* CViewAttach::get_text_font_face(void) {
	//__LOGT__( TAG, "get_text_font_face()" );
	
	return m_text_font_face;
}

void CViewAttach::set_text_font_typeface(e_ObjAttachFontTypeface_t typeface) {
	//__LOGT__( TAG, "set_text_font_typeface()" );
	
	m_text_font_typeface = typeface;
}

e_ObjAttachFontTypeface_t CViewAttach::get_text_font_typeface(void) {
	//__LOGT__( TAG, "get_text_font_typeface()" );
	
	return m_text_font_typeface;
}

const char* CViewAttach::get_text_font_typeface_str(bool show) {
	//__LOGT__( TAG, "get_text_font_typeface_str()" );
	
	switch ( get_text_font_typeface() ) {
		//case e_objAttachFontTypeface_UNKNOWN:
		//	{} break;
		case e_objAttachFontTypeface_NORMAL:
			{
				if ( show ) __LOGT__( TAG, "get_text_font_typeface_str(): NORMAL" );
				return (char*)"Normal";
			} break;
		//case e_objAttachFontTypeface_BOLD:
		//	{} break;
		case e_objAttachFontTypeface_ITALIC:
			{
				if ( show ) __LOGT__( TAG, "get_text_font_typeface_str(): ITALIC" );
				return (char*)"Italic";
			} break;
		// case e_objAttachFontTypeface_UNDERLINE:
		//	{} break;
		case e_objAttachFontTypeface_OBLIQUE:
			{
				if ( show ) __LOGT__( TAG, "get_text_font_typeface_str(): OBLIQUE" );
				return (char*)"Oblique";
			} break;
		default:
			{
				if ( show ) __LOGT__( TAG, "get_text_font_typeface_str(): UNKNOWN" );
			} break;
	}

	return NULL;
}

cairo_font_slant_t CViewAttach::get_text_font_typeface_cairo(e_ObjAttachFontTypeface_t typeface) {
	//__LOGT__( TAG, "get_text_font_typeface_cairo()" );
	
	cairo_font_slant_t ret = CAIRO_FONT_SLANT_NORMAL;

	switch ( typeface ) {
		//case e_objAttachFontTypeface_UNKNOWN:
		//	{
		//	} break;
		case e_objAttachFontTypeface_NORMAL:
			{
				ret = CAIRO_FONT_SLANT_NORMAL;
			} break;
		//case e_objAttachFontTypeface_BOLD:
		//	{
		//		ret = CAIRO_FONT_SLANT_NORMAL;
		//	} break;
		case e_objAttachFontTypeface_ITALIC:
			{
				//! ITALIC, OBLIQUE doesn't work in Cairo here
				ret = CAIRO_FONT_SLANT_ITALIC;
			} break;
		//case e_objAttachFontTypeface_UNDERLINE:
		//	{
		//		ret = CAIRO_FONT_SLANT_NORMAL;
		//	} break;
		case e_objAttachFontTypeface_OBLIQUE:
			{
				//! ITALIC, OBLIQUE doesn't work in Cairo here
				ret = CAIRO_FONT_SLANT_OBLIQUE;
			} break;
	}

	return ret;
}

void CViewAttach::set_text_font_typeface_bold(bool bold) {
	//__LOGT__( TAG, "set_text_font_typeface_bold()" );
	
	m_text_font_bold = bold;
}

bool CViewAttach::get_text_font_typeface_bold(void) {
	//__LOGT__( TAG, "get_text_font_typeface_bold()" );
	
	return m_text_font_bold;
}

cairo_font_weight_t CViewAttach::get_text_font_typeface_bold_cairo(void) {
	//__LOGT__( TAG, "get_text_font_typeface_bold_cairo()" );
	
	cairo_font_weight_t ret = CAIRO_FONT_WEIGHT_NORMAL;

	if ( m_text_font_bold )
		ret = CAIRO_FONT_WEIGHT_BOLD;

	return ret;
}

void CViewAttach::show_text_font_info_all(void) {
	//__LOGT__( TAG, "show_text_font_info_all()" );
	
	ColorARGB_st color = get_text_font_color();

	__LOGT__( TAG, "show_text_font_info_all(): font face = %s", get_text_font_face() );
	__LOGT__( TAG, "show_text_font_info_all(): typeface = %s", get_text_font_typeface_str(false) );
	__LOGT__( TAG, "show_text_font_info_all(): bold = %s", (get_text_font_typeface_bold())? "ture" : "false" );
	__LOGT__( TAG, "show_text_font_info_all(): size = %f", get_text_font_size() );

	__LOGT__( TAG, "show_text_font_info_all(): color(16) = A(%d), R(%d), G(%d), B(%d)",
				color.a, color.r, color.g, color.b );
	__LOGT__( TAG, "show_text_font_info_all(): color(8) = A(%d), R(%d), G(%d), B(%d)",
				PAINT_COLOR_UINT16_8(color.a),
				PAINT_COLOR_UINT16_8(color.r),
				PAINT_COLOR_UINT16_8(color.g),
				PAINT_COLOR_UINT16_8(color.b) );
	__LOGT__( TAG, "show_text_font_info_all(): FRACTION value (0 to 1): A(%f), R(%f), G(%f), B(%f)",
				PAINT_COLOR_UINT16_FRACTION_CAIRO(color.a),
				PAINT_COLOR_UINT16_FRACTION_CAIRO(color.r),
				PAINT_COLOR_UINT16_FRACTION_CAIRO(color.g),
				PAINT_COLOR_UINT16_FRACTION_CAIRO(color.b) );
}

void CViewAttach::show_text_font_info_color(void) {
	//__LOGT__( TAG, "show_text_font_info_color()" );
	
	ColorARGB_st color = get_text_font_color();

	__LOGT__( TAG, "show_text_font_info_color(): color(16) = A(%d), R(%d), G(%d), B(%d)",
				color.a, color.r, color.g, color.b );

	__LOGT__( TAG, "show_text_font_info_all(): color(8) = A(%d), R(%d), G(%d), B(%d)",
				PAINT_COLOR_UINT16_8(color.a),
				PAINT_COLOR_UINT16_8(color.r),
				PAINT_COLOR_UINT16_8(color.g),
				PAINT_COLOR_UINT16_8(color.b) );

	__LOGT__( TAG, "show_text_font_info_color(): FRACTION value (0 to 1): A(%f), R(%f), G(%f), B(%f)",
				PAINT_COLOR_UINT16_FRACTION_CAIRO(color.a),
				PAINT_COLOR_UINT16_FRACTION_CAIRO(color.r),
				PAINT_COLOR_UINT16_FRACTION_CAIRO(color.g),
				PAINT_COLOR_UINT16_FRACTION_CAIRO(color.b) );
}

void CViewAttach::draw_text(canvas_t* canvas) {
	//__LOGT__( TAG, "draw_text()" );
	
	double x = 0;
	double y = 0;

	draw_text( canvas, get_text(), x, y );
}

void CViewAttach::draw_text(canvas_t* canvas, const char* text, const GdkRectangle rect) {
	//__LOGT__( TAG, "draw_text()" );
	
	draw_text( canvas, text, (double)rect.x, (double)rect.y, (double)rect.width, (double)rect.height );
}

void CViewAttach::draw_text(canvas_t* canvas, const char* text, double x, double y) {
	//__LOGT__( TAG, "draw_text()" );
	
	GdkRectangle rect = get_rect();

	draw_text( canvas, text, x, y, (double)rect.width, (double)rect.height );
}

void CViewAttach::draw_text(canvas_t* canvas, const char* text,
		double x, double y, double w, double h) {
	//__LOGT__( TAG, "draw_text()" );
	
	if ( !canvas ) {
		__LOGT__( TAG, "draw_text(): canvas_t == NULL" );
		return;
	}

	if ( !text ) {
		__LOGT__( TAG, "draw_text(): text == NULL" );
		return;
	}

	if ( !get_text_font_face() ) {
		__LOGT__( TAG, "draw_text(): font family == NULL" );
		return;
	}


	// Background image
	if ( get_has_image() ) {
		draw_image( canvas, x, y, w, h );
	}


	// Text
	cairo_save( canvas );
	{
		cairo_font_slant_t typeface =
				get_text_font_typeface_cairo( get_text_font_typeface() );
		cairo_font_weight_t typeface_bold = get_text_font_typeface_bold_cairo();
		cairo_text_extents_t exts;


		//! ITALIC, OBLIQUE doesn't work in Cairo here
		// - Should be considered change to 'PANGO Text APIs'
		// - (replace 'Cairo Text APIs' by 'PANGO Text APIs')
		cairo_select_font_face( canvas, get_text_font_face(),
				typeface, typeface_bold );
		cairo_set_font_size( canvas, get_text_font_size() );
		cairo_text_extents( canvas, text, &exts );

		// Font Color
		draw_paint_color( canvas, true, get_text_font_color() );

		//cairo_move_to( canvas, x, (y - exts.y_bearing) );
		//cairo_show_text( canvas, text );



		// Auto CR/LF
		{
			//! TODO: UTF-8

			int glyph_size = 0;
			int cluster_size = 0;

			cairo_scaled_font_t* scaled_font;
			cairo_font_options_t* font_options;
			cairo_font_face_t* font_face;
			cairo_matrix_t ctm, font_matrix;

			cairo_glyph_t* glyphs = NULL;
			cairo_text_cluster_t* clusters = NULL;
			cairo_text_cluster_flags_t cluster_flags;
			cairo_status_t status;

			font_options = cairo_font_options_create();
			font_face = cairo_get_font_face( canvas );
			cairo_get_matrix( canvas, &ctm );
			cairo_get_font_matrix( canvas, &font_matrix );
			font_matrix.xx = font_matrix.yy = get_text_font_size();	// font size (20.0f)

			if ( !font_options || !font_face ) {
				__LOGT__( TAG, "draw_text(): cairo_font_options_t, cairo_font_face_t == NULL" );
				cairo_restore( canvas );
				return;
			}

			scaled_font = cairo_scaled_font_create( font_face, &font_matrix, &ctm, font_options );

			if ( !scaled_font ) {
				__LOGT__( TAG, "draw_text(): cairo_scaled_font_t == NULL" );
				cairo_restore( canvas );
				return;
			}

			status = cairo_scaled_font_text_to_glyphs(
						scaled_font,
						x, y,
						get_text(), strlen(get_text()),
						&glyphs, &glyph_size, 
						&clusters, &cluster_size, &cluster_flags );

			if ( (status == CAIRO_STATUS_SUCCESS) && (strlen(get_text()) > 0) ) {
				{
					cairo_text_extents_t exts_glyph;
					int glyph_w = 0;
					int glyph_h = 0;
					int next_one_glyph_w = 0;
					double glyph_x = x;
					double glyph_y = (y - exts.y_bearing);


					//__LOGT__( TAG, "draw_text(): rectangle x = %f, y = %f, w = %f(%f), h = %f(%f)",
					//		x, y, w, (x + w), h, (y + h) );
					//__LOGT__( TAG, "draw_text(): glyphs = %d, clusters = %d", glyph_size, cluster_size );

					for ( int i = 0, cr = 0; i < glyph_size; i++ ) {
						bool new_line = false;

						{
							//__LOGT__( TAG, "draw_text(): (%f >= %f)",
							//			(glyph_x + (double)glyph_w + (double)next_one_glyph_w), (x + w) );

							if ( (glyph_x + (double)glyph_w + (double)next_one_glyph_w) >= (x + w) ) {
								new_line = true;
								++cr;
								glyph_x = x;
								glyph_y += (double)glyph_h;
							}

							if ( glyph_y >= (y + h) ) {
								glyph_size = cluster_size = i;
								break;
							}
						}


						if ( (i > 0) && !new_line )
							glyph_x += (double)glyph_w;


						//! affected next glyph one
						{
							// get the one glyph's extents
							cairo_glyph_extents( canvas,/*glyphs*/ &glyphs[i], /*glyph_size*/1, &exts_glyph );
							glyph_w = exts_glyph.width;
							glyph_h = (glyph_h > exts_glyph.height)? glyph_h : exts_glyph.height;

							// Whitespace
							if ( (glyphs[i].index == DEFAULT_TEXT_GLYPH_WHITESPACE_INDEX) || (glyph_w <= 0) ) {
								glyph_w = DEFAULT_TEXT_GLYPH_WHITESPACE_WIDTH_NEW( get_text_font_size() );
								//__LOGT__( TAG, "draw_text(): [%d] glyphs (%c, %ld) -> whitespace",
								//		i, *(get_text() + i), glyphs[i].index );
							}


							// get the next one glyph's extents
							if ( (i + 1) < glyph_size ) {
								cairo_glyph_extents( canvas,/*glyphs*/ &glyphs[i+1], /*glyph_size*/1, &exts_glyph );
								next_one_glyph_w = exts_glyph.width;

								// Whitespace
								if ( (glyphs[i+1].index == DEFAULT_TEXT_GLYPH_WHITESPACE_INDEX) || (next_one_glyph_w <= 0) ) {
									next_one_glyph_w = DEFAULT_TEXT_GLYPH_WHITESPACE_WIDTH_NEW( get_text_font_size() );
									//__LOGT__( TAG, "draw_text(): [%d] glyphs (%c, %ld) -> whitespace",
									//		i, *(get_text() + i+1), glyphs[i+1].index );
								}
							}

							//__LOGT__( TAG, "draw_text(): [%d] glyphs (%c), w = %d, h = %d",
							//			i, *(get_text() + i), glyph_w, glyph_h );
						}



						glyphs[i] = (cairo_glyph_t) { glyphs[i].index, glyph_x, glyph_y };
						//__LOGT__( TAG, "draw_text(): [%d] glyphs (%c, %ld), x = %f, y = %f, w = %d, h = %d",
						//			i, *(get_text() + i), glyphs[i].index, glyph_x, glyph_y, glyph_w, glyph_h );


						// --------------------

						#if 0
						//! TEST:
						{
							int one_glyph_w = 0;
							int one_glyph_h = 0;
							cairo_text_extents_t one_exts_glyph;

							// get the one glyph's extents
							cairo_glyph_extents( canvas,/*glyphs*/ &glyphs[i], /*glyph_size*/1, &one_exts_glyph );
							one_glyph_w = one_exts_glyph.width;
							one_glyph_h = one_exts_glyph.height;

							__LOGT__( TAG, "draw_text(): [%d] glyphs (%c), x = %f, y = %f, w = %d, h = %d",
										i, *(get_text() + i), glyph_x, glyph_y, one_glyph_w, one_glyph_h );
							//cairo_show_glyphs( canvas, glyphs, glyph_size );
						}
						#endif
					} // for()

					{
						if ( glyph_size <= 0 ) {
							glyph_size = cluster_size = 0;
						}
					}
				}


				// Show
				//__LOGT__( TAG, "draw_text(): glyphs = %d, clusters = %d", glyph_size, cluster_size );
				cairo_show_text_glyphs( canvas, get_text(), /*strlen(get_text())*/glyph_size,
							glyphs, glyph_size,
							clusters, cluster_size, cluster_flags );


				// Release
				if ( glyphs ) {
					cairo_glyph_free( glyphs );
					glyphs = NULL;
				}

				if ( clusters ) {
					cairo_text_cluster_free( clusters );
					clusters = NULL;
				}
			}
		}
	}
	cairo_restore( canvas );
}

void CViewAttach::draw_text_pango(canvas_t* canvas, bool simple) {
	//__LOGT__( TAG, "draw_text_pango()" );
	
	double x = 0;
	double y = 0;

	draw_text_pango( canvas, get_text(), x, y );
}

void CViewAttach::draw_text_pango(canvas_t* canvas, const char* text,
		const GdkRectangle rect, bool simple) {
	//__LOGT__( TAG, "draw_text()_pango" );
	
	draw_text_pango( canvas, text, (double)rect.x, (double)rect.y,
						(double)rect.width, (double)rect.height, simple );
}

void CViewAttach::draw_text_pango(canvas_t* canvas, const char* text,
		double x, double y, bool simple) {
	//__LOGT__( TAG, "draw_text_pango()" );
	
	GdkRectangle rect = get_rect();

	draw_text_pango( canvas, text, x, y,
						(double)rect.width, (double)rect.height, simple );
}

void CViewAttach::draw_text_pango(canvas_t* canvas, const char* text,
		double x, double y, double w, double h, bool simple) {
	//__LOGT__( TAG, "draw_text_pango()" );
	
	if ( !canvas ) {
		__LOGT__( TAG, "draw_text_pango(): canvas_t == NULL" );
		return;
	}

	if ( !text ) {
		__LOGT__( TAG, "draw_text_pango(): text == NULL" );
		return;
	}

	if ( !get_text_font_face() ) {
		__LOGT__( TAG, "draw_text_pango(): font family == NULL" );
		return;
	}


	// Background image
	if ( get_has_image() ) {
		draw_image( canvas, x, y, w, h );
	}



	if ( simple ) {
		// Text
		cairo_save( canvas );
		{
			// "Sans Bold Italic 12"
			char font_name[DEFAULT_TEXT_FONT_FAMILY_SIZE_ALL] = { 0, };
			PangoLayout* layout = NULL;
			PangoFontDescription* font_desc = NULL;


			snprintf( font_name, sizeof(font_name), "%s %s %s %f",
						get_text_font_face(),
						(get_text_font_typeface_bold()? "Bold" : ""),
						((get_text_font_typeface() == e_objAttachFontTypeface_NORMAL)?
								"" : get_text_font_typeface_str(false)),
						get_text_font_size() );
			//__LOGT__( TAG, "draw_text_pango(): font name = %s", font_name );


			/*
			// FreeType2
			//
			#include <pango/pangoft2.h>
			#include <fontconfig.h>
			PangoFontMap* font_map = pango_ft2_font_map_new();
			PangoContext* context = pango_font_map_create_context( PANGO_FONT_MAP(font_map) );
			PangoLayout* layout = pango_layout_new( context );
			PangoFontDescription* font_desc = pango_font_description_from_string( "Sans, 12" );
			pango_layout_set_font_description( layout, font_desc );
			pango_font_description_free( font_desc );
			g_object_unref( layout );
			*/


			layout = pango_cairo_create_layout( canvas );
			font_desc = pango_font_description_from_string( (char*)font_name );

			if ( !layout || !font_desc ) {
				if ( font_desc ) {
					pango_font_description_free( font_desc );
					font_desc = NULL;
				}
				else {
					__LOGT__( TAG, "draw_text_pango(): pango font description == NULL" );
				}

				if ( layout ) {
					g_object_unref( layout );
					layout = NULL;
				}
				else {
					__LOGT__( TAG, "draw_text_pango(): pango font layout == NULL" );
				}

				return;
			}


			//font_desc = pango_font_description_from_string( "Sans, 12" );
			pango_layout_set_font_description( layout, font_desc );
			pango_font_description_free( font_desc );

			// Text
			pango_layout_set_text( layout, get_text(), -1 );

			// Word-Wrap, Ellipsize
			//  - PANGO_WRAP_WORD, PANGO_WRAP_CHAR, PANGO_WRAP_WORD_CHAR
			//  - PANGO_ELLIPSIZE_NONE, PANGO_ELLIPSIZE_START, PANGO_ELLIPSIZE_MIDDLE, PANGO_ELLIPSIZE_END
			//pango_layout_set_width( layout, ((int)w * PANGO_SCALE) );
			//pango_layout_set_height( layout, ((int)h * PANGO_SCALE) );
			pango_layout_set_width( layout, ((int)w << DEFAULT_TEXT_PANGO_SCALE_BIT) );
			pango_layout_set_height( layout, ((int)h << DEFAULT_TEXT_PANGO_SCALE_BIT) );
			pango_layout_set_wrap( layout, PANGO_WRAP_CHAR );
			pango_layout_set_ellipsize( layout, PANGO_ELLIPSIZE_END );
			//pango_extents_to_pixels( NULL, NULL );

			// Font Color
			draw_paint_color( canvas, true, get_text_font_color() );

			// pango-cairo drawing
			//cairo_text_extents_t exts;
			//cairo_text_extents( canvas, text, &exts );
			//cairo_move_to( canvas, x, (y - exts.y_bearing) );
			cairo_move_to( canvas, x, y );
			// ...
			pango_cairo_update_layout( canvas, layout );
			pango_cairo_show_layout( canvas, layout );


			// Release
			//
			if ( layout ) {
				g_object_unref( layout );
				layout = NULL;
			}
		}
		cairo_restore( canvas );

		return;
	}



	//! Glyph handling
	// TODO&FIXME: {
	//		- adjust text position(x, y):
	//		+---- add a text when selected another one
	//		+---- it's will be okay when redraw the view
	//		- Unicode
	//		- Word-spacing
	// }
	// ----------------------------------------
	
	// Text
	cairo_save( canvas );
	{
		// "Sans Bold Italic 12"
		char font_name[DEFAULT_TEXT_FONT_FAMILY_SIZE_ALL] = { 0, };
		PangoLayout* layout = NULL;
		PangoFontDescription* font_desc = NULL;


		snprintf( font_name, sizeof(font_name), "%s %s %s %f",
					get_text_font_face(),
					(get_text_font_typeface_bold()? "Bold" : ""),
					((get_text_font_typeface() == e_objAttachFontTypeface_NORMAL)?
					 		"" : get_text_font_typeface_str(false)),
					get_text_font_size() );
		//__LOGT__( TAG, "draw_text_pango(): font name = %s", font_name );


		/*
		// FreeType2
		//
		#include <pango/pangoft2.h>
		#include <fontconfig.h>
		PangoFontMap* font_map = pango_ft2_font_map_new();
		PangoContext* context = pango_font_map_create_context( PANGO_FONT_MAP(font_map) );
		PangoLayout* layout = pango_layout_new( context );
		PangoFontDescription* font_desc = pango_font_description_from_string( "Sans, 12" );
		pango_layout_set_font_description( layout, font_desc );
		pango_font_description_free( font_desc );
		g_object_unref( layout );
		*/


		layout = pango_cairo_create_layout( canvas );
		font_desc = pango_font_description_from_string( (char*)font_name );

		if ( !layout || !font_desc ) {
			if ( font_desc ) {
				pango_font_description_free( font_desc );
				font_desc = NULL;
			}
			else {
				__LOGT__( TAG, "draw_text_pango(): pango font description == NULL" );
			}

			if ( layout ) {
				g_object_unref( layout );
				layout = NULL;
			}
			else {
				__LOGT__( TAG, "draw_text_pango(): pango font layout == NULL" );
			}

			return;
		}


		//font_desc = pango_font_description_from_string( "Sans, 12" );
		pango_layout_set_font_description( layout, font_desc );
		pango_font_description_free( font_desc );

		// Text
		pango_layout_set_text( layout, get_text(), -1 );

		// Word-Wrap, Ellipsize
		//  - PANGO_WRAP_WORD, PANGO_WRAP_CHAR, PANGO_WRAP_WORD_CHAR
		//  - PANGO_ELLIPSIZE_NONE, PANGO_ELLIPSIZE_START, PANGO_ELLIPSIZE_MIDDLE, PANGO_ELLIPSIZE_END
		//pango_layout_set_width( layout, ((int)w * PANGO_SCALE) );
		//pango_layout_set_height( layout, ((int)h * PANGO_SCALE) );
		pango_layout_set_width( layout, ((int)w << DEFAULT_TEXT_PANGO_SCALE_BIT) );
		pango_layout_set_height( layout, ((int)h << DEFAULT_TEXT_PANGO_SCALE_BIT) );
		//pango_layout_set_wrap( layout, PANGO_WRAP_CHAR );
		//pango_layout_set_ellipsize( layout, PANGO_ELLIPSIZE_END );
		//pango_extents_to_pixels( NULL, NULL );

		// Font Color
		draw_paint_color( canvas, true, get_text_font_color() );

		// pango-cairo drawing
		//cairo_text_extents_t exts;
		//cairo_text_extents( canvas, text, &exts );
		//cairo_move_to( canvas, x, (y - exts.y_bearing) );
		//cairo_move_to( canvas, x, y );
		// ...
		pango_cairo_update_layout( canvas, layout );
		//pango_cairo_show_layout( canvas, layout );
		// Release
		//
		//if ( layout ) {
		//	g_object_unref( layout );
		//	layout = NULL;
		//}
		//cairo_restore( canvas );
		//return;



		// Auto CR/LF
		{
			int glyph_size = 0;
			int cluster_size = 0;

			PangoLayoutIter* iter = pango_layout_get_iter( layout );
			PangoLayoutLine* line = NULL;


			do {
				GSList* cur = NULL;
				//int line_offset = 0;


				if ( !iter )
					break;

				line = pango_layout_iter_get_line( iter );
				if ( !line ) continue;

				cur = line->runs;
				//line_offset = line->start_index;

				while ( cur ) {
					PangoLayoutRun* run = (PangoLayoutRun*)cur->data;

					if ( run ) {
						//const char* lang = pango_language_to_string( run->item->analysis.language );
						//char* font_filename = NULL;
						//PangoFontDescription* desc = NULL;
						PangoFont* font = NULL;
						//int run_offset = run->item->offset;

						int glyph_w = 0;
						int glyph_h = 0;
						int next_one_glyph_w = 0;
						int glyph_x = (int)x;
						int glyph_y = (int)y;
						int glyph_offset = 0;


						// Font
						font = run->item->analysis.font;
						if ( !font ) {
							__LOGT__( TAG, "draw_text_pango(): font == NULL\n" );
							break;
						}

						/*
						// Description
						desc = pango_font_describe( font );
						if ( !desc ) {
							__LOGT__( TAG, "draw_text_pango(): description == NULL\n" );
							break;
						}
						*/

						/*
						// Font filename
						//font_filename = pango_font_description_to_filename( desc );
						//__LOGT__( TAG, "draw_text_pango(): Language = %s", lang );
						//__LOGT__( TAG, "draw_text_pango(): Font filename = %s", font_filename );


						__LOGT__( TAG, "draw_text_pango(): -------------------------------" );
						__LOGT__( TAG, "draw_text_pango(): glyphs = %d", run->glyphs->num_glyphs );
						__LOGT__( TAG, "draw_text_pango():   -> rectangle x = %f, y = %f, w = %f(%f), h = %f(%f)",
								x, y, w, (x + w), h, (y + h) );
						//__LOGT__( TAG, "draw_text_pango(): glyphs = %d, clusters = %d", glyph_size, cluster_size );
						*/



						glyph_size = run->glyphs->num_glyphs;
						for ( int i = 0, cr = 0; i < run->glyphs->num_glyphs; i++ ) {
							/*
							__LOGT__( TAG, "draw_text_pango(): [%d] = glyphs (%c, %d), x = %d, y = %d, width = %d",
										i, *(get_text() + i), run->glyphs->glyphs[i].glyph,
										run->glyphs->glyphs[i].geometry.x_offset,
										run->glyphs->glyphs[i].geometry.y_offset,
										run->glyphs->glyphs[i].geometry.width );
										//run->glyphs->glyphs[i].geometry.width / PANGO_SCALE );
										//PANGO_PIXELS(run->glyphs->glyphs[i].geometry.width) );
							*/

							{
								bool new_line = false;

								{
									//__LOGT__( TAG, "draw_text_pango():   -> width (%d >= %f)",
									//			(glyph_x + glyph_w + next_one_glyph_w) - glyph_offset, (x + w/2) );

									// Pango font glyph width doesn't match in Cairo space
									// therefore do (width / 2) here
									if ( (glyph_x + glyph_w + next_one_glyph_w) >= (int)(x + (w/2)) ) {
										new_line = true;
										++cr;

										// sum of the each glyph's offset
										glyph_offset += ((glyph_x - x) + glyph_w);
										glyph_x = x;
										glyph_y += glyph_h;
									}

									// glyphs within height of the rectangle,
									// remainders doesn't drawn
									if ( glyph_y >= (int)(y + h) ) {
										glyph_size = cluster_size = i;
										break;
									}
								}

								// Adds glyph's width
								if ( (i > 0) && !new_line ) {
									glyph_x += glyph_w;
								}


								//! affected next glyph one
								{
									// run->glyphs->glyphs[i]
									// - .geometry.x_offset;
									// - .geometry.y_offset;
									//
									// (offset / PANGO_SCALE), PANGO_PIXELS(offset)
									// - .geometry.width;

									int glyph_h_tmp = 0;
									PangoRectangle ink_rect;
									PangoRectangle logical_rect;
									pango_font_get_glyph_extents( font, run->glyphs->glyphs[i].glyph,
																	&ink_rect,
																	&logical_rect );

									//glyph_w = (run->glyphs->glyphs[i].geometry.width / PANGO_SCALE);
									//glyph_h_tmp = (ink_rect.height / PANGO_SCALE);
									glyph_w = (run->glyphs->glyphs[i].geometry.width >> DEFAULT_TEXT_PANGO_SCALE_BIT);
									glyph_h_tmp = (ink_rect.height >> DEFAULT_TEXT_PANGO_SCALE_BIT);
									glyph_h = (glyph_h > glyph_h_tmp)? glyph_h : glyph_h_tmp;
									/*
									glyph_w = PANGO_PIXELS( run->glyphs->glyphs[i].geometry.width );
									glyph_h = (glyph_h > PANGO_DESCENT(ink_rect))?
												glyph_h : PANGO_DESCENT(ink_rect);
									glyph_h = (glyph_h > PANGO_PIXELS(ink_rect.height))?
												glyph_h : PANGO_PIXELS(ink_rect.height);
									*/
									if ( i == 0 ) {
										//! FIXME: MAX(glyphs height)
										//
										// Adds first glyph's ascent (height here)
										glyph_y += glyph_h;
									}

									/*
									__LOGT__( TAG, "draw_text_pango():   -> glyphs (%c, %d) -> x = %d, y = %d,"
												" w = %d(%d), h = %d(%d)",
												*(get_text() + i), run->glyphs->glyphs[i].glyph,
												ink_rect.x, ink_rect.y, ink_rect.width, glyph_w,
												ink_rect.height, glyph_h );
									*/
									/*
									__LOGT__( TAG, "draw_text_pango():   -> ink x = %d, y = %d, w = %d, h = %d",
												ink_rect.x, ink_rect.y, ink_rect.width, ink_rect.height );
									//__LOGT__( TAG, "draw_text_pango():   -> logical x = %d, y = %d, w = %d, h = %d",
									//			logical_rect.x, logical_rect.y, logical_rect.width, logical_rect.height );
									*/

									// Whitespace
									/*
									if ( (run->glyphs->glyphs[i].glyph == DEFAULT_TEXT_GLYPH_WHITESPACE_INDEX) ||
										 (glyph_w <= 0) ) {
										glyph_w = DEFAULT_TEXT_GLYPH_WHITESPACE_WIDTH_NEW( get_text_font_size() );
										__LOGT__( TAG, "draw_text_pango(): [%d] glyphs (%c, %d) -> whitespace (%d)",
												i, *(get_text() + i), run->glyphs->glyphs[i].glyph, glyph_w );
									}
									*/


									// Updates glyph's offset
									//run->glyphs->glyphs[i].geometry.x_offset = ((glyph_x - glyph_offset) * PANGO_SCALE);
									//run->glyphs->glyphs[i].geometry.y_offset = (glyph_y * PANGO_SCALE);
									run->glyphs->glyphs[i].geometry.x_offset =
										((glyph_x - glyph_offset) << DEFAULT_TEXT_PANGO_SCALE_BIT);
									run->glyphs->glyphs[i].geometry.y_offset = (glyph_y << DEFAULT_TEXT_PANGO_SCALE_BIT);


									// ------------------------------


									// get the next one glyph's extents
									if ( (i + 1) < glyph_size ) {
										//int glyph_h_tmp = 0;
										PangoRectangle ink_rect;
										PangoRectangle logical_rect;
										pango_font_get_glyph_extents( font, run->glyphs->glyphs[i+1].glyph,
																		&ink_rect,
																		&logical_rect );

										//next_one_glyph_w = (run->glyphs->glyphs[i+1].geometry.width / PANGO_SCALE);
										next_one_glyph_w = (run->glyphs->glyphs[i+1].geometry.width >> DEFAULT_TEXT_PANGO_SCALE_BIT);
										/*
										next_one_glyph_w = PANGO_PIXELS( run->glyphs->glyphs[i+1].geometry.width );
										glyph_h = (glyph_h > PANGO_DESCENT(ink_rect))?
													glyph_h : PANGO_DESCENT(ink_rect);
										glyph_h = (glyph_h > (ink_rect.height / PANGO_SCALE))?
													glyph_h : (ink_rect.height / PANGO_SCALE);
										glyph_h = (glyph_h > PANGO_PIXELS(ink_rect.height))?
													glyph_h : PANGO_PIXELS(ink_rect.height);
										*/

										/*
										__LOGT__( TAG, "draw_text_pango():   -> glyphs(+1) (%c, %d) -> x = %d, y = %d,"
													" w = %d(%d), h = %d(%d)",
													*(get_text() + (i+1)), run->glyphs->glyphs[i+1].glyph,
													ink_rect.x, ink_rect.y, ink_rect.width, glyph_w,
													ink_rect.height, glyph_h );
										*/

										// Whitespace
										/*
										if ( (run->glyphs->glyphs[i+1].glyph == DEFAULT_TEXT_GLYPH_WHITESPACE_INDEX) ||
											 (next_one_glyph_w <= 0) ) {
											next_one_glyph_w = DEFAULT_TEXT_GLYPH_WHITESPACE_WIDTH_NEW( get_text_font_size() );
											__LOGT__( TAG, "draw_text_pango(): [%d] glyphs (%c, %d) -> whitespace (%d)",
													i, *(get_text() + (i+1)), run->glyphs->glyphs[i+1].glyph, next_one_glyph_w );
										}
										*/
									}

									//__LOGT__( TAG, "draw_text_pango(): [%d] glyphs (%c), w = %d, h = %d",
									//			i, *(get_text() + i), glyph_w, glyph_h );
								}


								/*
								//run->glyphs->glyphs[i].geometry.x_offset = ((int)glyph_x * PANGO_SCALE);
								//run->glyphs->glyphs[i].geometry.y_offset = ((int)glyph_y * PANGO_SCALE);
								//
								__LOGT__( TAG, "draw_text_pango():   -> RESULT: [%d] x = %d, y = %d, w = %d, h = %d"
											", cluster = %d",
											i, glyph_x, glyph_y, glyph_w, glyph_h, *run->glyphs->log_clusters );
								*/
							}
						} // for()


						// Draw
						run->glyphs->num_glyphs = glyph_size;
						pango_cairo_show_glyph_string( canvas, font, run->glyphs );
					}

					cur = cur->next;
				} // while()
			} while ( pango_layout_iter_next_line(iter) );

			if ( iter ) {
				pango_layout_iter_free( iter );
				iter = NULL;
			}
		}


		// Release
		if ( layout ) {
			g_object_unref( layout );
			layout = NULL;
		}
	}
	cairo_restore( canvas );
}


// --------------------


void CViewAttach::draw_obj(canvas_t* canvas) {
	//__LOGT__( TAG, "draw_obj()" );
	
	if ( !canvas ) {
		__LOGT__( TAG, "draw_obj(): canvas_t == NULL" );
		return;
	}

	{
		int x = 0;
		int y = 0;
		const int radius = DEFAULT_DRAW_CIRCLE_RADIUS;


		// Checks boundary
		if ( get_selected() ) {
			//__LOGT__( TAG, "draw_obj(): x = %d, y = %d, w = %d, h = %d",
			//			m_rect.x, m_rect.y, m_rect.width, m_rect.height );

			if ( m_rect.width <= 1 ) m_rect.width = 1;
			if ( m_rect.height <= 1) m_rect.height = 1;
		}


		// Draw an objects
		if ( get_obj_type() == e_objAttachType_TEXT ) {
			// Draw a text
			//draw_text( canvas, get_text(), m_rect );

			// Pango test
			draw_text_pango( canvas, get_text(), m_rect );
		}
		else if ( get_obj_type() == e_objAttachType_IMAGE ) {
			// Draw an image
			draw_image( canvas, m_rect );
		}
		else {
			return;
		}



		// Clear previous path
		//cairo_new_sub_path( canvas );


		// Paint
		draw_paint_color( canvas, true, e_objAttachPaintColor_RED );


		if ( get_selected() ) {
			// Draw rectangle
			//gdk_cairo_rectangle( canvas, &m_rect );
			{
				// dotted(dashed) rectangle
				const double dashed[] = { 3.0f, 3.0f };
				const int dashed_len = sizeof(dashed) / sizeof(dashed[0]);

				cairo_save( canvas );
				{
					cairo_set_line_width( canvas, 1.0f );
					cairo_set_dash( canvas, dashed, dashed_len, 0 );
					cairo_rectangle( canvas, m_rect.x, m_rect.y, m_rect.width, m_rect.height );
					cairo_stroke( canvas );
				}
				cairo_restore( canvas );
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

			x = m_rect.x;
			y = m_rect.y;
			// move the drawing origin to the x and y
			//cairo_translate( canvas, x, y );
			// add a new circular path to the cairo drawing context
			//cairo_arc( canvas, x, y, radius, 0, (2 * M_PI) );
			// the existing path is not affected before calling cairo_arc()
			//cairo_new_sub_path( canvas );

			// Left-Top
			draw_circle( canvas, x, y, radius );
			// Right-Top
			x = ( m_rect.x + m_rect.width );
			draw_circle( canvas, x, y, radius );
			// Center-Top
			x = ( m_rect.x + (m_rect.width >> 1) );
			draw_circle( canvas, x, y, radius );

			x = m_rect.x;
			y = ( m_rect.y + m_rect.height );

			// Left-Bottom
			draw_circle( canvas, x, y, radius );
			// Right-Bottom
			x = ( m_rect.x + m_rect.width );
			draw_circle( canvas, x, y, radius );
			// Center-Bottom
			x = ( m_rect.x + (m_rect.width >> 1) );
			draw_circle( canvas, x, y, radius );

			x = m_rect.x;
			y = ( m_rect.y + (m_rect.height >> 1) );

			// Left,Right-Center
			draw_circle( canvas, x, y, radius );
			x = ( m_rect.x + m_rect.width );
			draw_circle( canvas, x, y, radius );



			// draws the outline of the circle
			cairo_stroke_preserve( canvas );
		}
	}

/*
	{
		const double row_width = 50.0f;
		const double row_height = 20.0f;
		//GdkRectangle rect;

		//rect.x = 0;
		//rect.y = 30;
		//rect.width = 100;
		//rect.height = 100;
		//gdk_cairo_rectangle( canvas, &rect );


		cairo_set_source_rgb( canvas, 255, 0, 0 );
		cairo_set_line_width( canvas, 3.0f );
		cairo_set_line_cap( canvas, CAIRO_LINE_CAP_SQUARE );
		cairo_move_to( canvas, 0.0f, row_height );
		cairo_line_to( canvas, (double)get_width(), row_height );
		cairo_stroke( canvas );
	}
*/
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

