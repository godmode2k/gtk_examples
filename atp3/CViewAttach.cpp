/* --------------------------------------------------------------
Project:	ATP(A Tentative Plan) 3
Purpose:	
Author:		Ho-Jung Kim (godmode2k@hotmail.com)
Date:		Since Dec 2, 2014
Filename:	CViewAttach.cpp

Last modified: Jan 27, 2016
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
	memset( (void*)&m_uuid, 0x00, sizeof(m_uuid) );
	get_uuid( m_uuid, sizeof(m_uuid) );

	// Text
	m_text = NULL;
	m_text_show_boundary = false;
	m_text_font_size = DEFAULT_TEXT_FONT_SIZE;
	//m_text_font_color = (ColorARGB_st) { PAINT_COLOR_UINT8_16(255), 0, 0, 0 };
	m_text_font_color.a = PAINT_COLOR_UINT8_16( 255 );
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
	m_touch_event = NULL;

	// Object rectangle
	m_rect.x = 50;
	m_rect.y = 50;
	m_rect.width = 100;
	m_rect.height = 100;

	// Object rotation
	m_obj_rotate = false;
	m_obj_rotate_degree = 0.f;
	m_obj_rotate_slide_rect.x = 0;
	m_obj_rotate_slide_rect.y = 0;
	m_obj_rotate_slide_rect.width = DEFAULT_ROTATION_UI_SLIDER_WIDTH;
	m_obj_rotate_slide_rect.height = DEFAULT_ROTATION_UI_SLIDER_HEIGHT;
	m_obj_rotate_slidebar_rect.x = 0;
	m_obj_rotate_slidebar_rect.y = 0;
	m_obj_rotate_slidebar_rect.width = DEFAULT_DRAW_CIRCLE_RADIUS;
	m_obj_rotate_slidebar_rect.height = DEFAULT_DRAW_CIRCLE_RADIUS;
	m_obj_rotate_slidebar_pos = 0;


	// Patchers IO
	m_pvec_patchers_io = new std::vector<objAttachPatchersIO_st*>;
	m_show_patchers_io = false;



	// --------------------
	//load_image( "./test.png" );
	__test_patchers_io_add();
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

	// Patchers IO
	if ( m_pvec_patchers_io ) {
		if ( !m_pvec_patchers_io->empty() ) {
			std::vector<objAttachPatchersIO_st*>::iterator iter;

			for ( iter = m_pvec_patchers_io->begin(); iter != m_pvec_patchers_io->end(); ) {
				objAttachPatchersIO_st* io = (*iter);

				if ( io ) {
					{
						// src output -> dst input
						//
						// previously linked
						if ( io->ln_pio_input && io->ln_pio_input->ln_pio_output ) {
							if ( io->ln_pio_input->ln_pio_output[io->ln_pio_input_registered_idx] == io ) {
								io->ln_pio_input->ln_pio_output[io->ln_pio_input_registered_idx] = NULL;
								io->ln_pio_input_registered_idx = -1;
							}
						}
					}

					{
						// dst output -> src input
						//
						for ( int i = 0; i < DEFAULT_PATCHERS_IO_NUM_OF_INOUT_LINKAGE; i++ ) {
							if ( !io->ln_pio_output[i] )
								continue;

							// previously linked
							io->ln_pio_output[i]->ln_pio_input = NULL;
							io->ln_pio_output[i]->ln_pio_input_registered_idx = -1;
							memset( io->ln_pio_output[i]->ln_pio_input_uuid, 0x00,
									sizeof(io->ln_pio_output[i]->ln_pio_input_uuid) );
							io->ln_pio_output[i] = NULL;
						}
					}

					io->fn = NULL;

					delete io;
					io = NULL;

					iter = m_pvec_patchers_io->erase( iter );
				}
			}

			m_pvec_patchers_io->clear();
		}

		delete m_pvec_patchers_io;
		m_pvec_patchers_io = NULL;
	}
}



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


		{
			if ( onTouchEvent_get_event() ) {
				if ( !onTouchEvent_get_event()->is_mouse_lbtn() ) {
					m_direction = e_objAttachDirection_UNKNOWN;
				}
			}
		}
	}
}



// Touch Event
// ---------------------------------------------------------------
bool CViewAttach::onTouchEvent(CKeyEvent* event) {
	//__LOGT__( TAG, "onTouchEvent()" );
	
	onTouchEvent_set_event( event );

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
				//if ( !(event->is_mouse_lbtn() || event->is_mouse_rbtn() ||
				//		event->is_mouse_wbtn()) ) {
				//	return false;
				//}
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
	m_obj_rotate_slide_touchX = x;
	m_obj_rotate_slide_touchY = y;

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

	if ( event->is_mouse_lbtn() ) {
		if ( get_selected() ) {
			update_position( m_direction, x, y );

			// Rotation
			if ( get_obj_rotate() ) {
				set_obj_rotate_update_position( m_direction, x,  y );
			}
		}
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
		case e_objAttachDirection_PATCHERS_IO_INPUT:
			{
				__LOGT__( TAG, "get_str_direction(): PATCHERS_IO_INPUT" );
			} break;
		case e_objAttachDirection_PATCHERS_IO_OUTPUT:
			{
				__LOGT__( TAG, "get_str_direction(): PATCHERS_IO_OUTPUT" );
			} break;
		default:
			{
				__LOGT__( TAG, "get_str_direction(): UNKNOWN" );
			} break;
	}
}

e_ObjAttachDirection_t CViewAttach::is_obj_selected_direction(GdkRectangle rect, float x, float y) {
	//__LOGT__( TAG, "is_obj_selected_direction()" );
	
	const int radius = DEFAULT_DRAW_CIRCLE_RADIUS;
	e_ObjAttachDirection_t ret = e_objAttachDirection_UNKNOWN;


	// LEFT-TOP
	if ( ((int)x >= (rect.x - radius)) &&
			((int)x <= (rect.x + radius)) &&
			((int)y >= (rect.y - radius)) &&
			((int)y <= (rect.y + radius)) ) {
		ret = e_objAttachDirection_LEFT_TOP;
	}
	// RIGHT-TOP
	else if ( ((int)x >= ((rect.x + rect.width) - radius)) &&
				((int)x <= ((rect.x + rect.width) + radius)) &&
				((int)y >= (rect.y - radius)) &&
				((int)y <= (rect.y + radius)) ) {
		ret = e_objAttachDirection_RIGHT_TOP;
	}
	// LEFT-BOTTOM
	else if ( ((int)x >= (rect.x - radius)) &&
				((int)x <= (rect.x + radius)) &&
				((int)y >= ((rect.y + rect.height) - radius)) &&
				((int)y <= ((rect.y + rect.height) + radius)) ) {
		ret = e_objAttachDirection_LEFT_BOTTOM;
	}
	// RIGHT-BOTTOM
	else if ( ((int)x >= ((rect.x + rect.width) - radius)) &&
				((int)x <= ((rect.x + rect.width) + radius)) &&
				((int)y >= ((rect.y + rect.height) - radius)) &&
				((int)y <= ((rect.y + rect.height) + radius)) ) {
		ret = e_objAttachDirection_RIGHT_BOTTOM;
	}
	// LEFT-CENTER
	else if ( ((int)x >= (rect.x - radius)) &&
				((int)x <= (rect.x + radius)) &&
				((int)y >= ((rect.y + (rect.height >> 1)) - radius)) &&
				((int)y <= ((rect.y + (rect.height >> 1)) + radius)) ) {
		ret = e_objAttachDirection_LEFT_CENTER;
	}
	// RIGHT-CENTER
	else if ( ((int)x >= ((rect.x + rect.width) - radius)) &&
				((int)x <= ((rect.x + rect.width) + radius)) &&
				((int)y >= ((rect.y + (rect.height >> 1)) - radius)) &&
				((int)y <= ((rect.y + (rect.height >> 1)) + radius)) ) {
		ret = e_objAttachDirection_RIGHT_CENTER;
	}
	// TOP-CENTER
	else if ( ((int)x >= ((rect.x + (rect.width >> 1)) - radius)) &&
				((int)x <= ((rect.x + (rect.width >> 1)) + radius)) &&
				((int)y >= (rect.y - radius)) &&
				((int)y <= (rect.y + radius)) ) {
		ret = e_objAttachDirection_TOP_CENTER;
	}
	// BOTTOM-CENTER
	else if ( ((int)x >= ((rect.x + (rect.width >> 1)) - radius)) &&
				((int)x <= ((rect.x + (rect.width >> 1)) + radius)) &&
				((int)y >= ((rect.y + rect.height) - radius)) &&
				((int)y <= ((rect.y + rect.height) + radius)) ) {
		ret = e_objAttachDirection_BOTTOM_CENTER;
	}
	// CENTER
	else if ( ((int)x >= rect.x) && ((int)x <= (rect.x + rect.width)) &&
				((int)y >= rect.y) && ((int)y <= (rect.y + rect.height)) ) {
		ret = e_objAttachDirection_CENTER;
	}
	else {
		// ROTATE: SLIDEBAR REGION
		if ( get_obj_rotate() ) {
			if ( ((int)x >= (m_obj_rotate_slide_rect.x - radius)) &&
					((int)x <= (m_obj_rotate_slide_rect.x + m_obj_rotate_slide_rect.width + radius)) &&
					((int)y >= (m_obj_rotate_slide_rect.y - radius)) &&
					((int)y <= (m_obj_rotate_slide_rect.y + m_obj_rotate_slide_rect.height + radius)) ) {
				ret = e_objAttachDirection_ROTATE_SLIDEBAR_REGION;
			}
		}
	}


	// Selected
	if ( (ret == e_objAttachDirection_UNKNOWN) || (ret == e_objAttachDirection_RESERVED) ) {
		set_select( false );
		set_obj_rotate( false );

		// Patchers IO
		if ( patchers_io_is_selected(rect, x, y) ) {
			ret = get_selected_direction();
			set_select( true );
		}
	}
	else {
		set_select( true );
	}

	//__LOGT__( TAG, "is_obj_selected_direction(): selected = %s", (get_selected()? "TRUE" : "FALSE") );

	// Patchers IO
	if ( patchers_io_get_show() ) {
		patchers_io_info();
	}


	return ret;
}

e_ObjAttachDirection_t CViewAttach::get_selected_direction(void) {
	//__LOGT__( TAG, "get_selected_direction()" );
	
	return m_direction;
}

void CViewAttach::update_position(e_ObjAttachDirection_t direction, float x, float y) {
	//__LOGT__( TAG, "update_position()" );
	
	//__LOGT__( TAG, "update_position(): x = %f, y = %f", x, y );
	//get_str_direction( m_direction );

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
					//if ( ((m_rect.y + m_rect.height) - y) < radius ) {
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
					//if ( ((m_rect.x + m_rect.width) - x) < radius ) {
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
					//if ( ((m_rect.y + m_rect.height) - y) < radius ) {
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
					//if ( (x - m_rect.x) < radius ) {
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
					//if ( (y - m_rect.y) < radius ) {
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
					//if ( ((m_rect.x + m_rect.width) - x) < radius ) {
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
					//if ( (y - m_rect.y) < radius ) {
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
					//if ( (x - m_rect.x) < radius ) {
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
					//if ( ((m_rect.x + m_rect.width) - x) < radius ) {
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
					//if ( ((m_rect.y + m_rect.height) - y) < radius ) {
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
					//if ( ((x - m_rect.x) < radius ) {
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
					//if ( (y - m_rect.y) < radius ) {
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
			// Patchers IO
			case e_objAttachDirection_PATCHERS_IO_INPUT:
			case e_objAttachDirection_PATCHERS_IO_OUTPUT:
				{
					if ( !m_pvec_patchers_io->empty() ) {
						std::vector<objAttachPatchersIO_st*>::iterator iter;

						for ( iter = m_pvec_patchers_io->begin(); iter != m_pvec_patchers_io->end(); ++iter ) {
							objAttachPatchersIO_st* io = (*iter);

							if ( io && io->selected ) {
								if ( direction == e_objAttachDirection_PATCHERS_IO_INPUT ) {
									//__LOGT__( TAG, "update_position(): PATCHERS_IO_INPUT: x = %f, y = %f", x, y );

									// Input
									// link lines: src input -> dst output
									//io->output_point.x = x;
									//io->output_point.y = y;
									io->output_x = x;
									io->output_y = y;
								}
								else if ( direction == e_objAttachDirection_PATCHERS_IO_OUTPUT ) {
									//__LOGT__( TAG, "update_position(): PATCHERS_IO_OUTPUT: x = %f, y = %f", x, y );

									// Output
									// link lines: src output -> dst input
									//io->input_point.x = x;
									//io->input_point.y = y;
									io->input_x = x;
									io->input_y = y;
								}

								break;
							}
						} // for()
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

void CViewAttach::draw_triangle(canvas_t* canvas, float x, float y, float size, bool fill) {
	//__LOGT__( TAG, "draw_triangle()" );
	
	if ( !canvas ) {
		__LOGT__( TAG, "draw_triangle(): canvas_t == NULL" );
		return;
	}

	{
		cairo_new_sub_path( canvas );

		// ▲
		cairo_move_to( canvas, x, y );
		cairo_line_to( canvas, (x - size), (y + size) );
		cairo_move_to( canvas, x, y );
		cairo_line_to( canvas, (x + size), (y + size) );
		cairo_move_to( canvas, (x - size), (y + size) );
		cairo_line_to( canvas, (x + size), (y + size) );

		/*
		// ►
		cairo_move_to( canvas, x, y );
		cairo_line_to( canvas, (x - size), (y - size) );
		cairo_move_to( canvas, x, y );
		cairo_line_to( canvas, (x - size), (y + size) );
		cairo_move_to( canvas, (x - size), (y - size) );
		cairo_line_to( canvas, (x - size), (y + size) );
		*/


		// ◄
		// ▼


		if ( fill )
			cairo_fill( canvas );


		// draws the outline
		//cairo_stroke_preserve( canvas );
	}
}

//! TODO
// - rotates trangle follow the x and y
//
// diff_x: distances between x and target x
// diff_y: distances between y and target y
void CViewAttach::draw_triangle(canvas_t* canvas, double x, double y, double diff_x, double diff_y,
		float size, bool fill) {
	//__LOGT__( TAG, "draw_triangle()" );
	
	if ( !canvas ) {
		__LOGT__( TAG, "draw_triangle(): canvas_t == NULL" );
		return;
	}

	//
	//    +---+
	//    o   o-.  <--- start (x, y)
	//    +---+  .
	//            .
	//             .
	//        +---> * <--- (x, y)
	//  diff_ |      .
	//  x, y  |       .
	//        |        .  +---+
	//        +-------> .-o   o
	//                  ^ +---+
	//                  |
	//                  |
	//                  +--- end (x, y)

	{
		const int pad = 4;
		//const double rel_x = (diff_x / 2) / pad;
		//const double rel_y = (diff_y / 2) / pad;
		const double rel_x = diff_x;
		const double rel_y = diff_y;

		cairo_new_sub_path( canvas );


		__LOGT__( TAG, "draw_triangle(): x, y = (%f, %f)", x, y );
		__LOGT__( TAG, "draw_triangle(): rel (x, y) distance = (%f, %f)", rel_x, rel_y );


		/*
		// +: Crossline
		cairo_move_to( canvas, x + rel_y, (y - size) + rel_y );
		cairo_line_to( canvas, x - rel_y, (y + size) - rel_y );
		cairo_move_to( canvas, (x - size) + rel_y, y + rel_y );
		cairo_line_to( canvas, (x + size) - rel_y, y - rel_y );
		*/

		// ►
		// |
		//cairo_move_to( canvas, x + rel_y, (y - size) + rel_y );
		//cairo_line_to( canvas, x - rel_y, (y + size) - rel_y );
		cairo_move_to( canvas, x + rel_y, (y - size) );
		cairo_line_to( canvas, x - rel_y, (y + size) );

		//cairo_move_to( canvas, x + rel_y, (y - size) + rel_y );
		//cairo_line_to( canvas, (x + size) - rel_y, y - rel_y );

		//cairo_move_to( canvas, (x - size), y - rel_y );
		//cairo_line_to( canvas, (x + size), y + rel_y );

		// --
		//cairo_move_to( canvas, x + rel_y, (y + size) - rel_y );
		//cairo_line_to( canvas, x - rel_y, (y - size) + rel_y );



		if ( fill )
			cairo_fill( canvas );


		// draws the outline
		//cairo_stroke_preserve( canvas );
	}
}

void CViewAttach::draw_crossline(canvas_t* canvas, float x, float y, float size) {
	//__LOGT__( TAG, "draw_crossline()" );
	
	if ( !canvas ) {
		__LOGT__( TAG, "draw_crossline(): canvas_t == NULL" );
		return;
	}

	{
		cairo_new_sub_path( canvas );

		// +
		cairo_move_to( canvas, x, (y - size) );
		cairo_line_to( canvas, x, (y + size) );
		cairo_move_to( canvas, (x - size), y );
		cairo_line_to( canvas, (x + size), y );



		// draws the outline
		//cairo_stroke_preserve( canvas );
	}
}

void CViewAttach::draw_rectangle(canvas_t* canvas, float x, float y, double radius, bool fill) {
	//__LOGT__( TAG, "draw_rectangle()" );
	
	draw_rectangle( canvas, x - ((int)radius >> 1), y - ((int)radius >> 1), radius, radius, fill );
}

void CViewAttach::draw_rectangle(canvas_t* canvas, float x, float y, float w, float h, bool fill) {
	//__LOGT__( TAG, "draw_rectangle()" );

	if ( !canvas ) {
		__LOGT__( TAG, "draw_rectangle(): canvas_t == NULL" );
		return;
	}

	{
		cairo_rectangle( canvas, x, y, w, h );

		if ( fill )
			cairo_fill( canvas );
	}
}

void CViewAttach::draw_drag_area(canvas_t* canvas, float x, float y, double radius, bool circle, bool fill) {
	//__LOGT__( TAG, "draw_drag_area()" );

	if ( circle )
		draw_circle( canvas, x, y, radius, fill );
	else
		draw_rectangle( canvas, x, y, radius, fill );
}

void CViewAttach::draw_rectangle_boundary(canvas_t* canvas, const GdkRectangle rect, bool fill) {
	//__LOGT__( TAG, "draw_rectangle_boundary()" );
	
	draw_rectangle_boundary( canvas, (float)rect.x, (float)rect.y, (float)rect.width, (float)rect.height );
}

void CViewAttach::draw_rectangle_boundary(canvas_t* canvas, float x, float y, float w, float h, bool fill) {
	//__LOGT__( TAG, "draw_rectangle_boundary()" );

	draw_rectangle( canvas, x, y, w, h, fill );
}

void CViewAttach::draw_curved_line(canvas_t* canvas,
		double start_x, double start_y, double end_x, double end_y) {
	//__LOGT__( TAG, "draw_curved_line()" );

	if ( !canvas ) {
		__LOGT__( TAG, "draw_curved_line(): canvas_t == NULL" );
		return;
	}

	{
		// Pmid = (x1 + x3) / 2, (y1 + y3) / 2
		/*
		 *
		 *                                  rotate 90 degree CW
		 *               * (x4, y4; after) <---------------------------+
		 *                .                                            |
		 *                 .                                           |
		 *                  .                                          |
		 *                   .  END                                    |
		 *                    * (x3, y3)                               |
		 *                   .                                         |
		 *                  .                                          |
		 *                 .                                           |
		 *                .                                            |
		 *               * <--- split point; pivot (x2, y2; before), pivot (x4, y4; before)
		 *              .                     |
		 *             .                      |
		 *            .                       |
		 * START     .                        |
		 * (x1, y1) *                         |
		 *           .          +-------------+
		 *            .         |
		 *             .        | rotate 90 degree CW
		 *              .       v
		 *               * (x2, y2; after)
		 *
		 *
		 *
		 *	const double xm = ( x1 + x3 ) / 2;
		 *	const double ym = ( y1 + y3 ) / 2;
		 *
		 *	// rotate Pmid by 90 degrees around p1 to get p2
		 *	x2 = -(ym - y1) + y1;
		 *	y2 = (xm - x1) + x1;
		 *
		 *	// rotate Pmid by 90 degrees around p3 to get p4
		 *	x4 = -(ym - y3) + y3;
		 *	y4 = (xm - x3) + x3;
		 *
		 *
		 *	cairo_move_to( canvas, x1, y1 );
		 *	cairo_curve_to( canvas, x2, y2, x4, y4, x3, y3 );
		 *
		 *	cairo_set_line_width( canvas, 10.0f );
		 *	cairo_stroke( canvas );
		 *
		 *	cairo_set_source_rgba( canvas, 1, 0.2f, 0.2f, 0.6f );
		 *	cairo_set_line_width( canvas, 10.0f );
		 *	cairo_move_to( canvas, x1, y1 ); cairo_line_to( canvas, x2, y2 );
		 *	cairo_move_to( canvas, x3, y3 ); cairo_line_to( canvas, x4, y4 );
		 *	cairo_stroke( canvas );
		 *
		 *
		*/
		const double x1 = start_x, y1 = start_y;
		const float curv_pivot_x = 15.f, curv_pivot_y = 20.f;
		const float line_width = 3.0f;
		const double x3 = end_x, y3 = end_y;
		double x2 = 0.f, y2 = 0.f, x4 = 0.f, y4 = 0.f;

		// x2, y2
		x2 = start_x + curv_pivot_x;
		y2 = start_y + curv_pivot_y;

		// x4, y4
		x4 = end_x - curv_pivot_x;
		y4 = end_y - curv_pivot_y;


		cairo_move_to( canvas, x1, y1 );
		cairo_curve_to( canvas, x2, y2, x4, y4, x3, y3 );

		cairo_set_line_width( canvas, line_width );
		cairo_stroke( canvas );

		
/*
		// direction
		{
			const double new_start_x = start_x + ((end_x - start_x) / 2);
			const double new_start_y = start_y + ((end_y - start_y) / 2);
			//
			// set start position as following the curved line
			//double dir_pad_x = (x4 - new_start_x) / 2;
			//double dir_pad_y = (y4 - new_start_y) / 2;
			//
			//const double mid_pos_x = -(new_start_y - start_y) + start_y;
			//const double mid_pos_y = -(new_start_x - start_x) + start_x;


			//__LOGT__( TAG, "draw_curved_line(): (s_x = %f, s_y = %f, e_x = %f, e_y = %f),\n"
			//				"    x = %f, y = %f",
			//				start_x, start_y, end_x, end_y,
			//				dir_pad_x, dir_pad_y );

			{
				// show the vertex
				cairo_save( canvas );
					draw_paint_color( canvas, true, e_objAttachPaintColor_RED );
					draw_circle( canvas, (new_start_x), (new_start_y), 3, true );
					//draw_triangle( canvas, (new_start_x), (new_start_y), 3.f );
					cairo_stroke( canvas );
				cairo_restore( canvas );

				//__LOGT__( TAG, "draw_curved_line(): ---> (%f, %f) (%f, %f)", start_x, start_y,
				//				(new_start_x - start_x), (new_start_y - start_y) );
				//draw_triangle( canvas, new_start_x, new_start_y,
				//				(new_start_x - start_x), (new_start_y - start_y), 10.f );

				__LOGT__( TAG, "draw_curved_line(): ---> (%f, %f) (%f, %f)", end_x, end_y,
								(end_x - new_start_x), (end_y - new_start_y) );
				draw_triangle( canvas, new_start_x, new_start_y,
								(end_x - new_start_x), (end_y - new_start_y), 10.f );
			}

			cairo_stroke( canvas );
		}
*/
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
				(double)rect.width, (double)rect.height );
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

		if ( get_obj_rotate() || (get_obj_rotate_degree() > 0.f) ) {
			// Rotation
			draw_obj_rotate( canvas, x, y, w, h, img_w, img_h );
		}
		else {
			cairo_translate( canvas, x, y );
			cairo_scale( canvas, w/img_w, h/img_h );
		}


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
		cairo_select_font_face( canvas, get_text_font_face(), typeface, typeface_bold );
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

void CViewAttach::draw_text_only(canvas_t* canvas, const char* text,
		double x, double y, double w, double h) {
	//__LOGT__( TAG, "draw_text_only()" );
	
	if ( !canvas ) {
		__LOGT__( TAG, "draw_text_only(): canvas_t == NULL" );
		return;
	}

	if ( !text ) {
		__LOGT__( TAG, "draw_text_only(): text == NULL" );
		return;
	}


	// Text
	cairo_save( canvas );
	{
		cairo_font_slant_t typeface =
				get_text_font_typeface_cairo( e_objAttachFontTypeface_NORMAL );
		cairo_font_weight_t typeface_bold = CAIRO_FONT_WEIGHT_NORMAL;
		cairo_text_extents_t exts;
		ColorARGB_st font_color;
		const int font_size = 10;
		const char* font_face = "Sans";

		font_color.a = PAINT_COLOR_UINT8_16( 255 );
		font_color.r = 0;
		font_color.g = 0;
		font_color.b = 0;


		//! ITALIC, OBLIQUE doesn't work in Cairo here
		// - Should be considered change to 'PANGO Text APIs'
		// - (replace 'Cairo Text APIs' by 'PANGO Text APIs')
		cairo_select_font_face( canvas, font_face, typeface, typeface_bold );
		cairo_set_font_size( canvas, (double)font_size );
		cairo_text_extents( canvas, text, &exts );

		// Font Color
		draw_paint_color( canvas, true, font_color );

		cairo_move_to( canvas, x, (y - exts.y_bearing) );
		cairo_show_text( canvas, text );
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

			{
				// Rotation
				if ( get_obj_rotate() || (get_obj_rotate_degree() > 0.f) ) {
					const double obj_w = w;
					const double obj_h = h;

					draw_obj_rotate( canvas, x, y, w, h, obj_w, obj_h );
				}
			}

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
			{
				// Rotation
				if ( get_obj_rotate() || (get_obj_rotate_degree() > 0.f) ) {
					// None
				}
				else {
					cairo_move_to( canvas, x, y );
				}
			}
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

void CViewAttach::set_obj_rotate(bool rotate) {
	//__LOGT__( TAG, "set_obj_rotate()" );
	
	m_obj_rotate = rotate;
}

void CViewAttach::set_obj_rotate(bool rotate, double degree) {
	//__LOGT__( TAG, "set_obj_rotate()" );
	
	m_obj_rotate = rotate;
	m_obj_rotate_degree = degree;
}

bool CViewAttach::get_obj_rotate(void) {
	//__LOGT__( TAG, "get_obj_rotate()" );
	
	return m_obj_rotate;
}

void CViewAttach::set_obj_rotate_degree(double degree) {
	//__LOGT__( TAG, "set_obj_rotate_degree()" );
	
	m_obj_rotate_degree = degree;
}

double CViewAttach::get_obj_rotate_degree(void) {
	//__LOGT__( TAG, "get_obj_rotate_degree()" );
	
	return m_obj_rotate_degree;
}

void CViewAttach::set_obj_rotate_degree_positive(bool positive) {
	//__LOGT__( TAG, "set_obj_rotate_degree_positive()" );
	
	m_obj_rotate_degree_positive = positive;
}

bool CViewAttach::get_obj_rotate_degree_positive(void) {
	//__LOGT__( TAG, "get_obj_rotate_degree_positive()" );
	
	return m_obj_rotate_degree_positive;
}

void CViewAttach::draw_obj_rotate(canvas_t* canvas, const GdkRectangle rect,
		double obj_w, double obj_h) {
	//__LOGT__( TAG, "draw_obj_rotate()" );
	
	draw_obj_rotate( canvas, (double)rect.x, (double)rect.y,
			(double)rect.width, (double)rect.height, obj_w, obj_h );
}

void CViewAttach::draw_obj_rotate(canvas_t* canvas, double x, double y, double w, double h,
		double obj_w, double obj_h) {
	//__LOGT__( TAG, "draw_obj_rotate()" );
	
	draw_obj_rotate( canvas, get_obj_rotate_degree(), x, y, w, h, obj_w, obj_h );
}

void CViewAttach::draw_obj_rotate(canvas_t* canvas, double degree, double x, double y,
		double w, double h, double obj_w, double obj_h) {
	//__LOGT__( TAG, "draw_obj_rotate()" );
	
	if ( !canvas ) {
		__LOGT__( TAG, "draw_obj_rotate(): canvas_t == NULL" );
		return;
	}


	{
		// Angle in radians
		//  = angle in degrees * (PI / 180)
		if ( degree >= 0.f ) {
			const double new_x = x + (w / 2);
			const double new_y = y + (h / 2);
			const double adjust = (-0.5f);

			cairo_translate( canvas, new_x, new_y );
			cairo_rotate( canvas, degree * (M_PI / 180) );
			cairo_scale( canvas, w/obj_w, h/obj_h );

			// adjust the rotated position
			cairo_translate( canvas, (obj_w * adjust), (obj_h * adjust) );
		}
	}
}

void CViewAttach::draw_obj_rotate_ui(canvas_t* canvas, double x, double y) {
	//__LOGT__( TAG, "draw_obj_rotate_ui()" );
	
	if ( !canvas ) {
		__LOGT__( TAG, "draw_obj_rotate(): canvas_t == NULL" );
		return;
	}

	{
		const double w = (double)m_obj_rotate_slide_rect.width;
		const double h = (double)m_obj_rotate_slide_rect.height;

		m_obj_rotate_slide_rect.x = x;
		m_obj_rotate_slide_rect.y = (y + DEFAULT_ROTATION_UI_SLIDER_HEIGHT_PADDING);

		draw_obj_rotate_ui( canvas, x, (y + DEFAULT_ROTATION_UI_SLIDER_HEIGHT_PADDING),
							w, h );
	}
}

void CViewAttach::draw_obj_rotate_ui(canvas_t* canvas, double x, double y, double w, double h) {
	//__LOGT__( TAG, "draw_obj_rotate_ui()" );
	
	if ( !canvas ) {
		__LOGT__( TAG, "draw_obj_rotate(): canvas_t == NULL" );
		return;
	}


	cairo_save( canvas );
	{
		const int radius = DEFAULT_DRAW_CIRCLE_RADIUS;

		// ----------0----------	// Current degree
		// ------<---*--->------	// SLIDEBAR
		// -180  ..  0  ..  +180	// DEGREES
		// +-------------------+


		// Paint
		draw_paint_color( canvas, true, e_objAttachPaintColor_RED );

		//cairo_rectangle( canvas, x, y, w, h );
		cairo_set_line_width( canvas, h );
		cairo_move_to( canvas, x, y );
		cairo_line_to( canvas, (x + w), y );

		{
			m_obj_rotate_slidebar_rect.y = (int)y;

			if ( m_obj_rotate_slidebar_rect.x == 0 ) {
				m_obj_rotate_slidebar_rect.x = ( (int)x + ((int)w >> 1) );
				//( (int)m_obj_rotate_slide_rect.x + ((int)m_obj_rotate_slide_rect.width >> 1) );
				m_obj_rotate_slidebar_pos = ((int)w >> 1);
			}
			else {
				if ( x >= 0 )
					m_obj_rotate_slidebar_rect.x = ((int)x + m_obj_rotate_slidebar_pos);
				else
					m_obj_rotate_slidebar_rect.x = (m_obj_rotate_slidebar_pos + (int)x);
			}


			draw_circle( canvas, m_obj_rotate_slidebar_rect.x, y, radius, false );


			// Text
			{
				// -180 ~ +180
				int degree = 0;
				char cur_degree[DEFAULT_ROTATION_UI_SLIDER_STR_SIZE + 1] = { 0, };

				if ( get_obj_rotate_degree() >= 180.f ) {
					// -180 ~ 0
					degree = (int)(get_obj_rotate_degree() - 360.f);

					// -180 or +180
					if ( (get_obj_rotate_degree() == 180) &&
							get_obj_rotate_degree_positive() ) {
						degree = abs(degree);
					}
				}
				else {
					// 0 ~ 179
					degree = (int)get_obj_rotate_degree();
				}
				

				// -180
				draw_text_only( canvas, DEFAULT_ROTATION_UI_SLIDER_STR_N180,
								(x - (double)radius), (y + (double)radius), 5, 4 );
				// +180
				draw_text_only( canvas, DEFAULT_ROTATION_UI_SLIDER_STR_P180,
								((x + w) - (double)(radius << 1)), (y + (double)radius), 5, 4 );
				// 0
				draw_text_only( canvas, DEFAULT_ROTATION_UI_SLIDER_STR_0,
								((x + (double)((int)w >> 1)) - (double)(radius >> 1)),
								(y + (double)radius), 5, 4 );

				// Current value
				snprintf( cur_degree, sizeof(cur_degree), "%d", degree );
				draw_text_only( canvas, cur_degree,
								((x + (double)((int)w >> 1)) - (double)(radius >> 1)),
								(y - (double)(radius << 1)), 5, 4 );
			}
		}

		cairo_stroke( canvas );
		cairo_stroke_preserve( canvas );
	}
	cairo_restore( canvas );
}

void CViewAttach::set_obj_rotate_update_position(e_ObjAttachDirection_t direction, float x, float y) {
	//__LOGT__( TAG, "set_obj_rotate_update_position()" );
	
	{
		double pos_x = 0.f;
		double fv = 0.f;
		double degree = 0.f;


		if ( (x < 0 ) || (direction != e_objAttachDirection_ROTATE_SLIDEBAR_REGION) ) {
			return;
		}


		if ( (int)x < m_obj_rotate_slide_rect.x ) {
			m_obj_rotate_slidebar_rect.x = m_obj_rotate_slide_rect.x;
			//pos_x = 0.f;
			m_obj_rotate_slidebar_pos = 0;
		}
		else if ( (int)x > (m_obj_rotate_slide_rect.x + m_obj_rotate_slide_rect.width) ) {
			m_obj_rotate_slidebar_rect.x =
				(m_obj_rotate_slide_rect.x + m_obj_rotate_slide_rect.width);
			//pos_x = (double)m_obj_rotate_slide_rect.width;
			m_obj_rotate_slidebar_pos = m_obj_rotate_slide_rect.width;
		}

		{
			if ( m_obj_rotate_slide_touchX > x ) {
				pos_x = (double)(m_obj_rotate_slide_touchX - x);
				//m_obj_rotate_slidebar_rect.x -= (int)pos_x;
				m_obj_rotate_slide_touchX -= (int)pos_x;

				m_obj_rotate_slidebar_pos -= pos_x;
			}
			else if ( m_obj_rotate_slide_touchX < x ) {
				pos_x = (double)(x - m_obj_rotate_slide_touchX);
				//m_obj_rotate_slidebar_rect.x += (int)pos_x;
				m_obj_rotate_slide_touchX += (int)pos_x;

				m_obj_rotate_slidebar_pos += pos_x;
			}

			if ( m_obj_rotate_slidebar_pos < 0 ) {
				m_obj_rotate_slidebar_pos = 0;
			}
			else if ( m_obj_rotate_slidebar_pos > m_obj_rotate_slide_rect.width ) {
				m_obj_rotate_slidebar_pos = m_obj_rotate_slide_rect.width;
			}
		}


		// fraction(0 to 1) value for the range 0 to (DEFAULT_ROTATION_UI_SLIDER_WIDTH - 1)
		fv = (double)(m_obj_rotate_slidebar_rect.x - m_obj_rotate_slide_rect.x);
		if ( fv < 0 ) { fv = 0.f; }
		if ( fv > DEFAULT_ROTATION_UI_SLIDER_WIDTH ) {
			fv = (double)DEFAULT_ROTATION_UI_SLIDER_WIDTH;
		}
		//__LOGT__( TAG, "set_obj_rotate_update_position(): x = %f, fv = %f", x, fv );
		fv = (fv / 180.f);		// interval 2
		//fv = (fv / 360.f);
		//__LOGT__( TAG, "set_obj_rotate_update_position(): x = %f, new fv = %f", x, fv );

		// width: 180 (0 to 179)
		// degrees: 180 (0 to 180, interval 2)
		//
		// --------------------
		// 0       100      179
		// --------------------
		// 0  ...  90  ...  180
		// 360     0        180
		// -180    0       +180

		//degree = (fv * 180.f);
		degree = (fv * 360.f);		// 0 to 360 degrees
		//__LOGT__( TAG, "set_obj_rotate_update_position(): degree = %f", degree );
		if ( fv == 0.5f ) {
			degree = 0.f;
		}
		else {
			//degree = (fv < 0.5f)? (270.f + degree) : (degree - 90.f);
			//degree = (fv < 0.5f)? (180.f + degree) : (degree - 180.f);

			if ( fv < 0.5f ) {
				degree = (180.f + degree);
				set_obj_rotate_degree_positive( false );
			}
			else {
				degree = (degree - 180.f);
				set_obj_rotate_degree_positive( true );
			}
		}


		//__LOGT__( TAG, "set_obj_rotate_update_position(): new degree = %f", degree );
		set_obj_rotate_degree( degree );
	}
}

// Patchers IO
bool CViewAttach::patchers_io_add(objAttachPatchersIO_st* io) {
	//__LOGT__( TAG, "patchers_io_add()" );
	
	if ( !m_pvec_patchers_io ) {
		__LOGT__( TAG, "patchers_io_add(): Patchers IO Container == NULL" );
		return false;
	}

	if ( !io ) return false;


	m_pvec_patchers_io->push_back( io );


	return true;
}

bool CViewAttach::patchers_io_set(objAttachPatchersIO_st* io, const unsigned short idx) {
	//__LOGT__( TAG, "patchers_io_set()" );
	
	if ( !m_pvec_patchers_io ) {
		__LOGT__( TAG, "patchers_io_set(): Patchers IO Container == NULL" );
		return false;
	}

	if ( !io ) return false;


	bool ret = false;

	if ( !m_pvec_patchers_io->empty() ) {
		std::vector<objAttachPatchersIO_st*>::iterator iter;

		for ( iter = m_pvec_patchers_io->begin(); iter != m_pvec_patchers_io->end(); ++iter ) {
			objAttachPatchersIO_st* _io = (*iter);

			if ( _io ) {
				if ( _io->idx == idx ) {
					//_io->rect = io->rect;
					//_io->x = io->x;
					//_io->y = io->y;
					_io->idx = io->idx;
					_io->selected = io->selected;
					_io->linked_input = io->linked_input;
					_io->linked_output = io->linked_output;
					_io->drawn_input = io->drawn_input;
					_io->drawn_output = io->drawn_output;
					_io->input_point.x = io->input_point.x;
					_io->input_point.y = io->input_point.y;
					_io->output_point.x = io->output_point.x;
					_io->output_point.y = io->output_point.y;
					_io->input_val1 = io->input_val1;
					_io->input_val2 = io->input_val2;
					_io->input_val3 = io->input_val3;
					_io->output_val1 = io->output_val1;
					_io->output_val2 = io->output_val2;
					_io->output_val3 = io->output_val3;
					//_io->input_x = io->input_x;
					//_io->input_y = io->input_y;
					//_io->output_x = io->output_x;
					//_io->output_y = io->output_y;
					//_io->ln_pio_input = io->ln_pio_input;
					//_io->ln_pio_output = io->ln_pio_output;
					_io->fn = io->fn;

					ret = true;
					break;
				}
			}
		} // for()
	}


	return ret;
}

bool CViewAttach::patchers_io_delete(const unsigned short idx) {
	//__LOGT__( TAG, "patchers_io_delete()" );
	
	if ( !m_pvec_patchers_io ) {
		__LOGT__( TAG, "patchers_io_delete(): Patchers IO Container == NULL" );
		return false;
	}


	bool ret = false;

	if ( !m_pvec_patchers_io->empty() ) {
		std::vector<objAttachPatchersIO_st*>::iterator iter;

		for ( iter = m_pvec_patchers_io->begin(); iter != m_pvec_patchers_io->end(); ++iter ) {
			objAttachPatchersIO_st* io = (*iter);

			if ( io ) {
				if ( io->idx == idx ) {
					//! TODO
					// ...

					io->fn = NULL;

					delete io;
					io = NULL;

					m_pvec_patchers_io->erase( iter );

					ret = true;
					break;
				}
			}
		} // for()
	}


	return ret;
}

bool CViewAttach::patchers_io_delete_all(void) {
	//__LOGT__( TAG, "patchers_io_delete_all()" );
	
	if ( !m_pvec_patchers_io ) {
		__LOGT__( TAG, "patchers_io_delete_all(): Patchers IO Container == NULL" );
		return false;
	}


	if ( !m_pvec_patchers_io->empty() ) {
		std::vector<objAttachPatchersIO_st*>::iterator iter;

		for ( iter = m_pvec_patchers_io->begin(); iter != m_pvec_patchers_io->end(); ++iter ) {
			objAttachPatchersIO_st* io = (*iter);

			if ( io ) {
				{
					// src output -> dst input
					//
					// previously linked
					if ( io->ln_pio_input && io->ln_pio_input->ln_pio_output ) {
						if ( io->ln_pio_input->ln_pio_output[io->ln_pio_input_registered_idx] == io ) {
							io->ln_pio_input->ln_pio_output[io->ln_pio_input_registered_idx] = NULL;
							io->ln_pio_input_registered_idx = -1;
						}
					}
				}

				{
					// dst output -> src input
					//
					for ( int i = 0; i < DEFAULT_PATCHERS_IO_NUM_OF_INOUT_LINKAGE; i++ ) {
						if ( !io->ln_pio_output[i] )
							continue;

						// previously linked
						io->ln_pio_output[i]->ln_pio_input = NULL;
						io->ln_pio_output[i]->ln_pio_input_registered_idx = -1;
						memset( io->ln_pio_output[i]->ln_pio_input_uuid, 0x00,
								sizeof(io->ln_pio_output[i]->ln_pio_input_uuid) );
						io->ln_pio_output[i] = NULL;
					}
				}

				io->fn = NULL;

				delete io;
				io = NULL;

				iter = m_pvec_patchers_io->erase( iter );
			}
		}

		m_pvec_patchers_io->clear();
	}

	delete m_pvec_patchers_io;
	m_pvec_patchers_io = NULL;


	return true;
}

objAttachPatchersIO_st* CViewAttach::patchers_io_get(const unsigned short idx) {
	//__LOGT__( TAG, "patchers_io_get()" );
	
	if ( !m_pvec_patchers_io ) {
		__LOGT__( TAG, "patchers_io_get(): Patchers IO Container == NULL" );
		return false;
	}


	objAttachPatchersIO_st* io = NULL;

	if ( !m_pvec_patchers_io->empty() ) {
		std::vector<objAttachPatchersIO_st*>::iterator iter;

		for ( iter = m_pvec_patchers_io->begin(); iter != m_pvec_patchers_io->end(); ++iter ) {
			objAttachPatchersIO_st* _io = (*iter);

			if ( _io ) {
				if ( _io->selected ) {
					io = _io;
					break;
				}
			}
		} // for()
	}


	return io;
}

bool CViewAttach::patchers_io_is_selected(GdkRectangle rect, float x, float y) {
	//__LOGT__( TAG, "patchers_io_is_selected()" );
	
	if ( !m_pvec_patchers_io ) {
		__LOGT__( TAG, "patchers_io_is_selected(): Patchers IO Container == NULL" );
		return false;
	}


	bool ret = false;
	const int radius = DEFAULT_PATCHERS_IO_CIRCLE_RADIUS;

	if ( !m_pvec_patchers_io->empty() ) {
		std::vector<objAttachPatchersIO_st*>::iterator iter;

		//__LOGT__( TAG, "patchers_io_is_selected(): Patchers IO size = %d", m_pvec_patchers_io->size() );

		for ( iter = m_pvec_patchers_io->begin(); iter != m_pvec_patchers_io->end(); ++iter ) {
			objAttachPatchersIO_st* io = (*iter);

			if ( io ) {
				io->selected = false;

				//__LOGT__( TAG, "patchers_io_is_selected(): idx = %d", io->idx );

				if ( !ret ) {
					// Left/Right-side, one patcher respectively
					// Input
					if ( ((int)x >= (io->input_point.x - radius)) &&
							((int)x <= (io->input_point.x + radius)) &&
							((int)y >= (io->input_point.y - radius)) &&
							((int)y <= (io->input_point.y + radius)) ) {
						io->selected = true;
						m_direction = e_objAttachDirection_PATCHERS_IO_INPUT;
						ret = true;
						break;
					}
					// Output
					else if ( ((int)x >= (io->output_point.x - radius)) &&
							((int)x <= (io->output_point.x + radius)) &&
							((int)y >= (io->output_point.y - radius)) &&
							((int)y <= (io->output_point.y + radius)) ) {
						io->selected = true;
						m_direction = e_objAttachDirection_PATCHERS_IO_OUTPUT;
						ret = true;
						break;
					}
				}
			}
		} // for()

		//get_str_direction( get_selected_direction() );
	}


	return ret;
}

bool CViewAttach::patchers_io_set_select(const unsigned short idx) {
	//__LOGT__( TAG, "patchers_io_set_select()" );
	
	if ( !m_pvec_patchers_io ) {
		__LOGT__( TAG, "patchers_io_set_select(): Patchers IO Container == NULL" );
		return false;
	}


	bool ret = false;

	if ( !m_pvec_patchers_io->empty() ) {
		std::vector<objAttachPatchersIO_st*>::iterator iter;

		for ( iter = m_pvec_patchers_io->begin(); iter != m_pvec_patchers_io->end(); ++iter ) {
			objAttachPatchersIO_st* io = (*iter);

			if ( io ) {
				if ( io->idx == idx ) {
					io->selected = true;

					ret = true;
					break;
				}
			}
		} // for()
	}


	return ret;
}

bool CViewAttach::patchers_io_get_selected(const unsigned short idx) {
	//__LOGT__( TAG, "patchers_io_get_selected()" );
	
	if ( !m_pvec_patchers_io ) {
		__LOGT__( TAG, "patchers_io_get_selected(): Patchers IO Container == NULL" );
		return false;
	}


	bool ret = false;

	if ( !m_pvec_patchers_io->empty() ) {
		std::vector<objAttachPatchersIO_st*>::iterator iter;

		for ( iter = m_pvec_patchers_io->begin(); iter != m_pvec_patchers_io->end(); ++iter ) {
			objAttachPatchersIO_st* io = (*iter);

			if ( io ) {
				if ( io->selected ) {
					ret = true;
					break;
				}
			}
		} // for()
	}


	return ret;
}

bool CViewAttach::patchers_io_set_inout_xy(const unsigned short idx, float x, float y, bool in) {
	//__LOGT__( TAG, "patchers_io_set_inout_xy()" );
	
	if ( !m_pvec_patchers_io ) {
		__LOGT__( TAG, "patchers_io_set_inout_xy(): Patchers IO Container == NULL" );
		return false;
	}


	bool ret = false;

	if ( !m_pvec_patchers_io->empty() ) {
		std::vector<objAttachPatchersIO_st*>::iterator iter;

		for ( iter = m_pvec_patchers_io->begin(); iter != m_pvec_patchers_io->end(); ++iter ) {
			objAttachPatchersIO_st* io = (*iter);

			if ( io ) {
				if ( io->idx == idx ) {
					if ( in ) {
						io->input_x = x;
						io->input_y = y;
						//io->input_point.x = x;
						//io->input_point.y = y;
					}
					else {
						io->output_x = x;
						io->output_y = y;
						//io->output_point.x = x;
						//io->output_point.y = y;
					}

					ret = true;
					break;
				}
			}
		} // for()
	}


	return ret;
}

bool CViewAttach::patchers_io_set_inout_xy(const unsigned short idx,
		float in_x, float in_y, float out_x, float out_y) {
	//__LOGT__( TAG, "patchers_io_set_inout_xy()" );
	
	if ( !m_pvec_patchers_io ) {
		__LOGT__( TAG, "patchers_io_set_inout_xy(): Patchers IO Container == NULL" );
		return false;
	}


	bool ret = false;

	if ( !m_pvec_patchers_io->empty() ) {
		std::vector<objAttachPatchersIO_st*>::iterator iter;

		for ( iter = m_pvec_patchers_io->begin(); iter != m_pvec_patchers_io->end(); ++iter ) {
			objAttachPatchersIO_st* io = (*iter);

			if ( io ) {
				if ( io->idx == idx ) {
					io->input_x = in_x;
					io->input_y = in_y;
					io->output_x = out_x;
					io->output_y = out_y;

					//io->input_point.x = in_x;
					//io->input_point.y = in_y;
					//io->output_point.x = out_x;
					//io->output_point.y = out_y;

					ret = true;
					break;
				}
			}
		} // for()
	}


	return ret;
}

bool CViewAttach::patchers_io_linking_detection(float* _ret_x, float* _ret_y) {
	//__LOGT__( TAG, "patchers_io_linking_detection()" );
	
	return patchers_io_linking_detection( patchers_io_get_attached_list(),
			0.f, 0.f, false, _ret_x, _ret_y );
}

bool CViewAttach::patchers_io_linking_detection(float x, float y, float* _ret_x, float* _ret_y) {
	//__LOGT__( TAG, "patchers_io_linking_detection()" );
	
	return patchers_io_linking_detection( patchers_io_get_attached_list(),
			x, y, true, _ret_x, _ret_y );
}

bool CViewAttach::patchers_io_linking_detection(std::vector<CViewAttach*>* attach,
		float _x, float _y, bool use_xy, float* _ret_x, float* _ret_y) {
	//__LOGT__( TAG, "patchers_io_linking_detection()" );
	
	if ( !m_pvec_patchers_io ) {
		__LOGT__( TAG, "patchers_io_linking_detection(): Patchers IO Container == NULL" );
		return false;
	}

	if ( !attach || attach->empty() ) {
		__LOGT__( TAG, "patchers_io_linking_detection(): Attach Container == NULL or EMPTY" );
		return false;
	}

	if ( attach->size() <= 1 ) {
		__LOGT__( TAG, "patchers_io_linking_detection(): Attach Container size <= 1" );
		return false;
	}


	bool ret = false;
	float x = 0.f, y = 0.f;
	objAttachPatchersIO_st* src_io = NULL;

	if ( !m_pvec_patchers_io->empty() ) {
		std::vector<objAttachPatchersIO_st*>::iterator iter;

		for ( iter = m_pvec_patchers_io->begin(); iter != m_pvec_patchers_io->end(); ++iter ) {
			objAttachPatchersIO_st* io = (*iter);

			if ( io && io->selected ) {
				if ( get_selected_direction() == e_objAttachDirection_PATCHERS_IO_OUTPUT ) {

					src_io = io;

					if ( !use_xy ) {
						//x = io->input_point.x;
						//y = io->input_point.y;
						x = io->input_x;
						y = io->input_y;
					}
					else {
						x = _x;
						y = _y;
					}
				}
				//else if ( get_selected_direction() == e_objAttachDirection_PATCHERS_IO_INPUT ) {
				//	// NOPE
				//}

				break;
			}
		} // for()
	}


	// Attached-list
	{
		std::vector<CViewAttach*>::iterator iter;
		for ( iter = attach->begin(); iter != attach->end(); ++iter ) {
			CViewAttach* _attach = (*iter);

			if ( _attach ) {
				std::vector<objAttachPatchersIO_st*>* patchers_io = _attach->patchers_io_get_all();

				if ( patchers_io && !patchers_io->empty() ) {
					std::vector<objAttachPatchersIO_st*>::iterator iter_pio;
					const int radius = DEFAULT_PATCHERS_IO_CIRCLE_RADIUS;

					for ( iter_pio = patchers_io->begin(); iter_pio != patchers_io->end(); ++iter_pio ) {
						objAttachPatchersIO_st* io = (*iter_pio);

						if ( io && !io->selected ) {
							// detect the linking point

							//__LOGT__( TAG, "patchers_io_linking_detection(): src output x = %f, y = %f"
							//			" | dst input x = %f, y = %f",
							//			x, y, io->input_point.x, io->input_point.y );

							// src output -> dst input
							if ( get_selected_direction() == e_objAttachDirection_PATCHERS_IO_OUTPUT ) {
								if ( (x >= (io->input_point.x - radius)) &&
									(x <= (io->input_point.x + radius)) &&
									(y >= (io->input_point.y - radius)) &&
									(y <= (io->input_point.y + radius)) ) {
									if ( !src_io ) {
										__LOGT__( TAG, "patchers_io_linking_detection(): src input == NULL" );
										ret = false;
										break;
									}

									{
										if ( !onTouchEvent_get_event() ) {
											__LOGT__( TAG, "patchers_io_linking_detection(): Touch Event == NULL" );
											return false;
										}

										// skip while in draggin
										if ( onTouchEvent_get_event()->is_mouse_lbtn() ) {
											return false;
										}
									}

									__LOGT__( TAG, "patchers_io_linking_detection(): DETECTED: src output -> dst input" );

									{
										bool pio_ret = PATCHERS_IO_ADD_OUT_TO_IN( io, src_io );
										//__LOGT__( TAG, "patchers_io_linking_detection(): added pos = %d, %s",
										//			src_io->ln_pio_input_registered_idx, (pio_ret? "TRUE" : "FALSE") );

										if ( !pio_ret ) {
											ret = false;
											break;
										}
									}
									src_io->ln_pio_input = io;
									src_io->linked_input = true;
									//io->ln_pio_output = src_io;
									io->linked_input = true;

									if ( _ret_x ) *_ret_x = io->input_point.x;
									if ( _ret_y ) *_ret_y = io->input_point.y;

									ret = true;
									break;
								}
								else {
									{
										bool pio_ret = PATCHERS_IO_DEL_OUT_FROM_IN( io, src_io );
										//__LOGT__( TAG, "patchers_io_linking_detection(): deleted pos = %d, %s",
										//			src_io->ln_pio_input_registered_idx, (pio_ret? "TRUE" : "FALSE") );

										if ( !pio_ret ) {
											ret = false;
											break;
										}
									}
									src_io->ln_pio_input = NULL;
									src_io->linked_input = false;
									//io->ln_pio_output = NULL;
									src_io->ln_pio_input_registered_idx = -1;
									io->linked_input = false;
								}
							}
							/*
							// src input -> dst output
							else if ( get_selected_direction() == e_objAttachDirection_PATCHERS_IO_INPUT ) {
								// NOPE
								if ( (x >= (io->output_point.x - radius)) &&
									(x <= (io->output_point.x + radius)) &&
									(y >= (io->output_point.y - radius)) &&
									(y <= (io->output_point.y + radius)) ) {
									if ( !src_io ) {
										__LOGT__( TAG, "patchers_io_linking_detection(): src out == NULL" );
										ret = false;
										break;
									}

									{
										if ( !onTouchEvent_get_event() ) {
											__LOGT__( TAG, "patchers_io_linking_detection(): Touch Event == NULL" );
											return false;
										}

										// skip while in draggin
										if ( onTouchEvent_get_event()->is_mouse_lbtn() ) {
											return false;
										}
									}

									__LOGT__( TAG, "patchers_io_linking_detection(): DETECTED: src input -> dst output" );

									src_io->ln_pio_output = io;
									src_io->linked_output = true;
									io->ln_pio_input = src_io;
									io->linked_output = true;

									if ( _ret_x ) *_ret_x = io->output_point.x;
									if ( _ret_y ) *_ret_y = io->output_point.y;

									ret = true;
									break;
								}
								else {
									src_io->ln_pio_output = NULL;
									src_io->linked_output = false;
									io->ln_pio_input = NULL;
									io->linked_output = false;
								}
							}
							*/
						}
					}
				}

				if ( ret ) break;
			}
		} // for()
	} // attached-list


	return ret;
}

void CViewAttach::patchers_io_info(void) {
	//__LOGT__( TAG, "patchers_io_info()" );
	
	if ( !get_selected() )
		return;

	if ( !m_pvec_patchers_io->empty() ) {
		std::vector<objAttachPatchersIO_st*>::iterator iter;

		//__LOGT__( TAG, "patchers_io_info(): size = %d", m_pvec_patchers_io->size() );
		for ( iter = m_pvec_patchers_io->begin(); iter != m_pvec_patchers_io->end(); ++iter ) {
			objAttachPatchersIO_st* io = (*iter);

			if ( io ) {
				__LOGT__( TAG, "patchers_io_info(): {" );
				__LOGT__( TAG, "patchers_io_info():    uuid = %s, text = %s", get_uuid_str(), get_text() );
				__LOGT__( TAG, "patchers_io_info():    src output -> dst input uuid = %s", io->ln_pio_input->uuid );

				if ( io->ln_pio_output ) {
					__LOGT__( TAG, "patchers_io_info():    dst output -> src input {" );
					for ( int i = 0; i < DEFAULT_PATCHERS_IO_NUM_OF_INOUT_LINKAGE; i++ ) {
						if ( io->ln_pio_output[i] ) {
							__LOGT__( TAG, "patchers_io_info():       idx = %d, uuid = %s",
										io->ln_pio_output[i]->ln_pio_input_registered_idx,
										io->ln_pio_output[i]->uuid );
						}
					}
					__LOGT__( TAG, "patchers_io_info():    }" );
					__LOGT__( TAG, "patchers_io_info(): }" );
				}
			}
		} // for()
	}
}

void CViewAttach::draw_patchers_io(canvas_t* canvas) {
	//__LOGT__( TAG, "draw_patchers_io()" );
	
	if ( !canvas ) {
		__LOGT__( TAG, "draw_patchers_io(): canvas_t == NULL" );
		return;
	}

	if ( !m_pvec_patchers_io ) {
		__LOGT__( TAG, "draw_patchers_io(): Patchers IO Container == NULL" );
		return;
	}


	//
	// Draw a circle for respective direction as following:
	//  - Right-side
	//
	//	                    *
	//		+-------------+ *
	//		| .         . | *
	//		|   .     .   | *
	//		|      .      | *
	//		|    .   .    | *
	//		| .         . | *
	//		+-------------+ *
	//	                    *
	//

	{
		int x = 0;
		int y = m_rect.y;
		const int radius = DEFAULT_PATCHERS_IO_CIRCLE_RADIUS;

		{
			if ( !patchers_io_get_show() ) {
				x = ( m_rect.x + m_rect.width );

				// Left/Right-side, one patcher respectively
				// Updates link(input/output) hilum position
				if ( !m_pvec_patchers_io->empty() ) {
					std::vector<objAttachPatchersIO_st*>::iterator iter;

					for ( iter = m_pvec_patchers_io->begin(); iter != m_pvec_patchers_io->end(); ++iter ) {
						objAttachPatchersIO_st* io = (*iter);

						if ( io ) {
							// Padding
							y += (DEFAULT_PATCHERS_IO_PADDING_HEIGHT << 1);

							// Input
							x = m_rect.x;
							io->input_point.x = (float)x;
							io->input_point.y = (float)y;


							// Output
							x = ( m_rect.x + m_rect.width );
							io->output_point.x = (float)x;
							io->output_point.y = (float)y;
						}
					} // for()
				}

				return;
			}
		}


		x = 0;
		y = m_rect.y;

		cairo_save( canvas );
		{
			// Paint
			draw_paint_color( canvas, true, e_objAttachPaintColor_BLUE );

			x = ( m_rect.x + m_rect.width );

			// Left/Right-side, one patcher respectively
			// Updates link(input/output) hilum position
			if ( !m_pvec_patchers_io->empty() ) {
				std::vector<objAttachPatchersIO_st*>::iterator iter;

				for ( iter = m_pvec_patchers_io->begin(); iter != m_pvec_patchers_io->end(); ++iter ) {
					objAttachPatchersIO_st* io = (*iter);

					if ( io ) {
						// Padding
						y += (DEFAULT_PATCHERS_IO_PADDING_HEIGHT << 1);

						// Input
						x = m_rect.x;
						draw_circle( canvas, x, y, radius );
						io->input_point.x = (float)x;
						io->input_point.y = (float)y;


						// Output
						x = ( m_rect.x + m_rect.width );
						draw_circle( canvas, x, y, radius );
						io->output_point.x = (float)x;
						io->output_point.y = (float)y;

						cairo_stroke( canvas );
						// draws the outline of the circle
						//cairo_stroke_preserve( canvas );
					}
				} // for()
			}


			{
				draw_paint_color( canvas, true, e_objAttachPaintColor_GREEN );
				cairo_set_line_width( canvas, 3.0f );
				cairo_set_line_cap( canvas, CAIRO_LINE_CAP_SQUARE );

				if ( !m_pvec_patchers_io->empty() ) {
					std::vector<objAttachPatchersIO_st*>::iterator iter;

					for ( iter = m_pvec_patchers_io->begin(); iter != m_pvec_patchers_io->end(); ++iter ) {
						objAttachPatchersIO_st* io = (*iter);

						if ( io ) {
							// Draw a linking line
							{
								if ( get_selected_direction() == e_objAttachDirection_PATCHERS_IO_OUTPUT ) {
									// link lines: src output -> dst input
									//

									{
										float _ret_x = 0.f, _ret_y = 0.f;
										if ( patchers_io_linking_detection(
													//io->input_point.x, io->input_point.y,
													io->input_x, io->input_y,
													&_ret_x, &_ret_y) ) {
											io->linked_output = true;
											//io->input_point.x = _ret_x;
											//io->input_point.y = _ret_y;
										}
										else {
											io->linked_output = false;
											io->ln_pio_input = NULL;
										}
									}
								}
								else if ( get_selected_direction() == e_objAttachDirection_PATCHERS_IO_INPUT ) {
									// link lines: src input -> dst output
									// NOPE

									/*
									{
										if ( ... ) {
											io->linked_input = true;
										}
										else {
											io->linked_input = false;
											io->ln_pio_output = NULL;
										}
									}
									*/
								}
								else {
									// src output -> dst input
									io->input_x = io->output_point.x;
									io->input_y = io->output_point.y;

									// src input -> dst output
									io->output_x = io->input_point.x;
									io->output_y = io->input_point.y;
								}


								// Output
								//cairo_move_to( canvas, io->output_point.x, io->output_point.y );
								//cairo_line_to( canvas, io->input_point.x, io->input_point.y );
								//cairo_stroke( canvas );


								if ( get_selected() ) {
									io->drawn_input = false;
									io->drawn_output = false;
								}

								// draw a linking lines
								// src output -> dst input
								if ( (get_selected_direction() == e_objAttachDirection_PATCHERS_IO_OUTPUT) ) {
									//|| (get_selected_direction() == e_objAttachDirection_PATCHERS_IO_INPUT) ) {
									cairo_move_to( canvas, io->output_point.x, io->output_point.y );
									cairo_line_to( canvas, io->input_x, io->input_y );
								}
								else {
									for ( int i = 0; i < DEFAULT_PATCHERS_IO_NUM_OF_INOUT_LINKAGE; i++ ) {
										if ( io->ln_pio_input && io->ln_pio_output[i] ) {
											if ( !io->drawn_input ) {
												// Output
												//cairo_move_to( canvas, io->output_point.x, io->output_point.y );
												//cairo_line_to( canvas,
												//				io->ln_pio_input->input_point.x,
												//				io->ln_pio_input->input_point.y );
												//cairo_stroke( canvas );
												draw_curved_line( canvas, io->output_point.x, io->output_point.y,
																io->ln_pio_input->input_point.x,
																io->ln_pio_input->input_point.y );

												io->ln_pio_input->drawn_output = true;
											}

//! ---DELETE--- [
											/*
											if ( !io->ln_pio_output[i]->drawn_output ) {
												// Input
												//cairo_move_to( canvas, io->input_point.x, io->input_point.y );
												//cairo_line_to( canvas,
												//				io->ln_pio_output[i]->output_point.x,
												//				io->ln_pio_output[i]->output_point.y );
												//cairo_stroke( canvas );
												draw_curved_line( canvas, io->input_point.x, io->input_point.y,
																io->ln_pio_output[i]->output_point.x,
																io->ln_pio_output[i]->output_point.y );

												io->ln_pio_output[i]->drawn_input = true;
											}
											*/
//! ---DELETE--- ]
										}
										else {
											// src output -> dst input
											if ( io->ln_pio_input ) {
												if ( !io->drawn_input ) {
													// Output
													cairo_move_to( canvas, io->output_point.x, io->output_point.y );
													cairo_line_to( canvas,
																	io->ln_pio_input->input_point.x,
																	io->ln_pio_input->input_point.y );
													//draw_curved_line( canvas, io->output_point.x, io->output_point.y,
													//				io->ln_pio_input->input_point.x,
													//				io->ln_pio_input->input_point.y );

													io->ln_pio_input->drawn_output = true;
												}
											}
//! ---DELETE--- [
											/*
											else {
												// Output
												cairo_move_to( canvas, io->output_point.x, io->output_point.y );
												cairo_line_to( canvas, io->input_x, io->input_y );
												//cairo_line_to( canvas, io->input_point.x, io->input_point.y );
												////cairo_line_to( canvas,
												////					io->ln_pio_input->input_point.x,
												////					io->ln_pio_input->input_point.y );
											}
											*/
											/*
											// src input -> dst output
											else if ( io->ln_pio_output[i] ) {
												if ( !io->ln_pio_output[i]->drawn_output ) {
													// Input
													//cairo_move_to( canvas, io->input_point.x, io->input_point.y );
													//cairo_line_to( canvas,
													//				io->ln_pio_output[i]->output_point.x,
													//				io->ln_pio_output[i]->output_point.y );
													draw_curved_line( canvas, io->input_point.x, io->input_point.y,
																	io->ln_pio_output[i]->output_point.x,
																	io->ln_pio_output[i]->output_point.y );

													io->ln_pio_output[i]->drawn_input = true;
												}
											}
											*/
											//else {
											//	cairo_move_to( canvas, io->output_point.x, io->output_point.y );
											//	cairo_line_to( canvas, io->input_x, io->input_y );
											//}
//! ---DELETE--- ]
										}
									} // for()
								}

								io->drawn_input = false;
								io->drawn_output = false;

								cairo_stroke( canvas );
								//cairo_stroke_preserve( canvas );
							}
						}
					} // for()
				}
			}


			// draws the outline of the circle
			//cairo_stroke_preserve( canvas );
		}
		cairo_restore( canvas );
	}
}

bool CViewAttach::__test_patchers_io_add(unsigned short patchers) {
	//__LOGT__( TAG, "__test_patchers_io_add()" );

	for ( unsigned short i = 0; i < patchers; i++ ) {
		objAttachPatchersIO_st* io = new objAttachPatchersIO_st;

		if ( !io ) {
			return false;
		}

		io->idx = i;
		memset( (void*)&io->uuid, 0x00, sizeof(io->uuid) );
		memset( (void*)&io->ln_pio_input_uuid, 0x00, sizeof(io->ln_pio_input_uuid) );
		snprintf( io->uuid, sizeof(io->uuid), "%s", get_uuid_str() );
		io->selected = false;
		io->linked_input = false;
		io->linked_output = false;
		io->drawn_input = false;
		io->drawn_output = false;
		{
			/*
			int x = 0;
			int y = m_rect.y;

			// Left/Right-side, one patcher respectively
			// Padding
			y += (DEFAULT_PATCHERS_IO_PADDING_HEIGHT << 1);

			// Left-Top
			x = ( m_rect.x - DEFAULT_PATCHERS_IO_PADDING_WIDTH );
			io->input_point.x = x;
			io->input_point.y = y;

			// Right-Top
			x = ( m_rect.x + m_rect.width ) + DEFAULT_PATCHERS_IO_PADDING_WIDTH;
			io->output_point.x = x;
			io->output_point.y = y;
			*/

			io->input_point.x = 0.f;
			io->input_point.y = 0.f;
			io->output_point.x = 0.f;
			io->output_point.y = 0.f;
		}
		io->input_val1 = 0;
		io->input_val2 = 0;
		io->input_val3 = 0;
		io->output_val1 = 0;
		io->output_val2 = 0;
		io->output_val3 = 0;
		io->input_x = 0.f;
		io->input_y = 0.f;
		io->output_x = 0.f;
		io->output_y = 0.f;
		io->ln_pio_input = NULL;
		//io->ln_pio_output = NULL;
		{
			for ( int i = 0; i < DEFAULT_PATCHERS_IO_NUM_OF_INOUT_LINKAGE; i++ ) {
				io->ln_pio_output[i] = NULL;
			}
		}
		io->ln_pio_input_registered_idx = -1;
		//io->ln_pio_output_registered_idx = -1;
		io->fn = NULL;

		if ( !patchers_io_add(io) )
			return false;
	}


	if ( !m_pvec_patchers_io ) {
		__LOGT__( TAG, "__test_patchers_io_add(): Patchers IO Container == NULL" );
		return false;
	}


	/*
	if ( !m_pvec_patchers_io->empty() ) {
		std::vector<objAttachPatchersIO_st*>::iterator iter;

		for ( iter = m_pvec_patchers_io->begin(); iter != m_pvec_patchers_io->end(); ++iter ) {
			objAttachPatchersIO_st* io = (*iter);

			if ( io ) {
				__LOGT__( TAG, "__test_patchers_io_add(): uuid = %s", io->uuid );
				__LOGT__( TAG, "__test_patchers_io_add(): idx = %d", io->idx );
				__LOGT__( TAG, "__test_patchers_io_add(): selected = %s",
							(io->selected? "TRUE" : "FALSE") );
				__LOGT__( TAG, "__test_patchers_io_add(): linked input = %s",
							(io->linked_input? "TRUE" : "FALSE") );
				__LOGT__( TAG, "__test_patchers_io_add(): linked output = %s",
							(io->linked_output? "TRUE" : "FALSE") );
				__LOGT__( TAG, "__test_patchers_io_add(): input_point x = %f, y = %f",
							io->input_point.x, io->input_point.y );
				__LOGT__( TAG, "__test_patchers_io_add(): output_point x = %f, y = %f",
							io->output_point.x, io->output_point.y );
				__LOGT__( TAG, "__test_patchers_io_add(): input_val1 = %d", io->input_val1 );
				__LOGT__( TAG, "__test_patchers_io_add(): input_val2 = %d", io->input_val2 );
				__LOGT__( TAG, "__test_patchers_io_add(): input_val3 = %d", io->input_val3 );
				__LOGT__( TAG, "__test_patchers_io_add(): output_val1 = %d", io->output_val1 );
				__LOGT__( TAG, "__test_patchers_io_add(): output_val2 = %d", io->output_val2 );
				__LOGT__( TAG, "__test_patchers_io_add(): output_val3 = %d", io->output_val3 );
				//__LOGT__( TAG, "__test_patchers_io_add(): input_x = %f", io->input_x );
				//__LOGT__( TAG, "__test_patchers_io_add(): input_y = %f", io->input_y );
				//__LOGT__( TAG, "__test_patchers_io_add(): output_x = %f", io->output_x );
				//__LOGT__( TAG, "__test_patchers_io_add(): output_y = %f", io->output_y );
				//io->fn;
			}
		} // for()
	}
	*/


	return true;
}

/* ---DELETE---
bool CViewAttach::__test_patchers_io_link(const unsigned short src_idx,
		CViewAttach* dst, const unsigned short dst_idx, bool dst_in) const {
	//__LOGT__( TAG, "__test_patchers_io_link()" );
	
	if ( !m_pvec_patchers_io ) {
		__LOGT__( TAG, "__test_patchers_io_link(): Patchers IO Container == NULL" );
		return false;
	}

	if ( !dst ) {
		__LOGT__( TAG, "__test_patchers_io_link(): target object == NULL" );
		return false;
	}


	bool ret = false;
	objAttachPatchersIO_st* dst_io = dst->patchers_io_get( dst_idx );

	if ( !dst_io ) {
		__LOGT__( TAG, "__test_patchers_io_link(): target Patchers IO == NULL" );
		return false;
	}

	if ( !m_pvec_patchers_io->empty() ) {
		std::vector<objAttachPatchersIO_st*>::iterator iter;

		for ( iter = m_pvec_patchers_io->begin(); iter != m_pvec_patchers_io->end(); ++iter ) {
			objAttachPatchersIO_st* io = (*iter);

			if ( io ) {
				if ( io->idx == src_idx ) {
					if ( dst_in ) {
						//        +----+                         +----+
						// -(IN)->|obj1|-(OUT)->          -(IN)->|obj2|-(OUT)->
						//        +----+   |                 ^   +----+
						//                 |                 |
						//                 |                 |
						//                 +-----------------+

						//io->output_x = dst_io->input_x;
						//io->output_y = dst_io->input_y;
						io->output_point.x = dst_io->input_point.x;
						io->output_point.y = dst_io->input_point.y;
						io->linked_output = true;
					}
					else {
						// NOPE
						//
						//        +----+                         +----+
						// -(IN)->|obj1|-(OUT)->          -(IN)->|obj2|-(OUT)->
						//        +----+   |                     +----+   ^
						//                 |                              |
						//                 |                              |
						//                 +------------------------------+
						
						//io->output_x = dst_io->output_x;
						//io->output_y = dst_io->output_y;
						//io->output_point.x = dst_io->output_point.x;
						//io->output_point.y = dst_io->output_point.y;
						//io->linked_input = true;
					}


					ret = true;
					break;
				}
			}
		}
	}


	return ret;
}
*/



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

		// Rotation
		{
			if ( get_obj_rotate() ) {
				draw_obj_rotate_ui( canvas, (double)m_rect.x, (double)(m_rect.y + m_rect.height) );
			}
		}

		// Patchers IO
		{
			draw_patchers_io( canvas );
		}


		// Clear previous path
		//cairo_new_sub_path( canvas );


		// Object rectangle
		cairo_save( canvas );
		{
			if ( get_text_show_boundary() ) {
				// Draw boundary
				if ( (get_obj_type() == e_objAttachType_TEXT) && !get_has_image() ) {
					// Paint
					draw_paint_color( canvas, true, e_objAttachPaintColor_DKGRAY );
					draw_rectangle_boundary( canvas, m_rect );
					cairo_stroke( canvas );
				}
			}

			if ( get_selected() ) {
				// Paint
				draw_paint_color( canvas, true, e_objAttachPaintColor_RED );

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
				draw_drag_area( canvas, x, y, radius );
				// Right-Top
				x = ( m_rect.x + m_rect.width );
				draw_drag_area( canvas, x, y, radius );
				// Center-Top
				x = ( m_rect.x + (m_rect.width >> 1) );
				draw_drag_area( canvas, x, y, radius );

				x = m_rect.x;
				y = ( m_rect.y + m_rect.height );

				// Left-Bottom
				draw_drag_area( canvas, x, y, radius );
				// Right-Bottom
				x = ( m_rect.x + m_rect.width );
				draw_drag_area( canvas, x, y, radius );
				// Center-Bottom
				x = ( m_rect.x + (m_rect.width >> 1) );
				draw_drag_area( canvas, x, y, radius );

				x = m_rect.x;
				y = ( m_rect.y + (m_rect.height >> 1) );

				// Left,Right-Center
				draw_drag_area( canvas, x, y, radius );
				x = ( m_rect.x + m_rect.width );
				draw_drag_area( canvas, x, y, radius );



				cairo_stroke( canvas );
				// draws the outline of the circle
				//cairo_stroke_preserve( canvas );
			}
		}
		cairo_restore( canvas );
	}
}
// ---------------------------------------------------------------



//! Implementation
// ---------------------------------------------------------------
// Global variable
//
// ---------------------------------------------------------------

