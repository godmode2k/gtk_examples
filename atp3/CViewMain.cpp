/* --------------------------------------------------------------
Project:	ATP(A Tentative Plan) 3
Purpose:	
Author:		Ho-Jung Kim (godmode2k@hotmail.com)
Date:		Since Dec 2, 2014
Filename:	CViewMain.cpp

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

	// TODO:
	

	// Childs views
	{
		//__LOGT__( TAG, "draw(): attach tag = %s", attach.get_tag() );
		//m_attach.invalidate( view );

		attach_invalidate( view );
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
				attach->load_image( bg_img_filename );
			}
		}
		else {
			if ( m_tmp_obj_bg_img_info.bg_img_filename ) {
				attach->load_image( m_tmp_obj_bg_img_info.bg_img_filename );
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

			return false;
		}

		//! TODO: stat ( img_filename )
		//

		attach->set_obj_type( type );
		attach->set_tag2( tag );
		attach->set_obj_rect( 50.f, 50.f, 100.f, 100.f );
		attach->load_image( img_filename );

		m_pvec_attach->push_back( attach );
	}


	return true;
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

