#ifndef __INCL_G_FUNC_SIGNAL_HANDLER_HXX__
#define __INCL_G_FUNC_SIGNAL_HANDLER_HXX__

/* --------------------------------------------------------------
Project:	ATP(A Tentative Plan) 3
Purpose:	
Author:		Ho-Jung Kim (godmode2k@hotmail.com)
Date:		Since Dec 2, 2014
Filename:	incl_g_func_signal_handler.hxx

Last modified: Jun 29, 2015
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
// ---------------------------------------------------------------



//! Definition
// ---------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif
// ---------------------------------------------------------------



//! Prototype
// ---------------------------------------------------------------
// incl_g_func_signal_handler.cxx"
namespace g_FuncSignalHandler {
	#define TAG__g_FuncSignalHandler		"g_FuncSignalHandler"

	// Gtk Window Quit
	void main_window_quit(GtkWidget* widget, gpointer user_data);

	//GtkFixed Resizing
	void on_event_gtkfixed_size_allocate(GtkWidget* widget, GdkRectangle* allocation, gpointer user_data);


	// Menu Item: Button
	// ---------------------------------------------------------------
	// Menu button
	void on_menu_button_open_clicked(GtkWidget* widget, gpointer user_data);
	void on_menu_button_test_anim_clicked(GtkWidget* widget, gpointer user_data);
	void on_menu_button_test_show_patchers_io_clicked(GtkWidget* widget, gpointer user_data);
	// Toolbar
	void on_toolbar_button_update_clicked(GtkWidget* widget, gpointer user_data);
	void on_toolbar_button_toastmsg_clicked(GtkWidget* widget, gpointer user_data);
	void on_toolbar_button_add_img_clicked(GtkWidget* widget, gpointer user_data);
	void on_toolbar_button_add_text_with_bg_img_clicked(GtkWidget* widget, gpointer user_data);
	void on_toolbar_button_delete_attached_obj_clicked(GtkWidget* widget, gpointer user_data);
	void on_toolbar_button_clear_all(GtkWidget* widget, gpointer user_data);
	void on_toolbar_button_take_screenshot_region_clicked(GtkWidget* widget, gpointer user_data);
	void on_toolbar_button_take_screenshot_save_clicked(GtkWidget* widget, gpointer user_data);


	// Popup Menu
	// ---------------------------------------------------------------
	bool on_event_create_popup_menu(GtkWidget* widget, gpointer user_data, e_popupMenu_t type);
	bool on_event_popup_menu_clicked(GtkMenuItem* item, gpointer user_data);
	bool on_event_popup_menu_attachment(GtkWidget* widget, gpointer user_data,
			bool selected, e_ObjAttachType_t type);


	// Font Dialog: Font, Color
	// ---------------------------------------------------------------
	void on_text_font_dialog_open(GtkWidget* widget, gpointer user_data);
	void on_text_font_color_dialog_open(GtkWidget* widget, gpointer user_data);
	void on_text_bg_image_dialog_open(GtkWidget* widget, gpointer user_data);
	void on_event_text_font_dialog_open(GtkFontSelectionDialog* dlg, gint response,
			gpointer user_data);
	void on_event_text_font_color_dialog_open(GtkFontSelectionDialog* dlg, gint response,
			gpointer user_data);


	// Keyboard & Mouse Event
	// ---------------------------------------------------------------
	void on_event_key_press(GtkWidget* widget, GdkEvent* event, gpointer user_data);
	void on_event_key_release(GtkWidget* widget, GdkEvent* event, gpointer user_data);
	void on_event_mouse(GtkWidget* widget, GdkEvent* event, gpointer user_data);


	///*
	// Timer for redraw
	// ---------------------------------------------------------------
	bool on_event_timer_redraw_handler(GtkWidget* widget);
	//*/


	// onDraw()
	// ---------------------------------------------------------------
	//void on_event_redraw(void);
	//bool on_event_draw_top_window(GtkWidget* widget, GdkEventExpose* event, gpointer user_data);
#ifdef __GTKv2__
	// v2.x: "expose-event"
	bool on_event_draw_main(GtkWidget* widget, GdkEventExpose* event, gpointer user_data);
#elif __GTKv3__
	// v3.x: "draw"
	bool on_event_draw_main(GtkWidget* widget, cairo_t* cr, gpointer user_data);
	//bool on_event_draw_...(GtkWidget* widget, cairo_t* cr, gpointer user_data);
#else
#endif

} // namespace g_FuncSignalHandler
// ---------------------------------------------------------------



//! Class
// ---------------------------------------------------------------
// ---------------------------------------------------------------



#endif	// __INCL_G_FUNC_SIGNAL_HANDLER_HXX__
