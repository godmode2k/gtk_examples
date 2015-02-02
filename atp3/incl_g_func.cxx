/* --------------------------------------------------------------
Project:	ATP(A Tentative Plan) 3
Purpose:	
Author:		Ho-Jung Kim (godmode2k@hotmail.com)
Date:		Since Dec 2, 2014
Filename:	incl_g_func.cxx

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
 - DO NOT INCLUDE "incl_g_func.hxx" here
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
// ---------------------------------------------------------------



//! Definition
// ---------------------------------------------------------------
// ---------------------------------------------------------------



//! Prototype
// ---------------------------------------------------------------
// ---------------------------------------------------------------



//! Class
// ---------------------------------------------------------------
// ---------------------------------------------------------------



//! Implementation
// ---------------------------------------------------------------

namespace g_Func {
	// Callback
	

	// Gtk Window Quit
	void main_window_quit(GtkWidget* widget, gpointer user_data) {
		__LOGT__( TAG__g_Func, "main_window_quit()" );

		// Global instance
		if ( g_pCAtp3 ) {
			delete g_pCAtp3;
			g_pCAtp3 = NULL;
		}


		// Finish Application
		gtk_main_quit();
	}

	// File Open Dialog
	char* file_open_dialog(GtkWidget* widget, gpointer user_data) {
		//__LOGT__( TAG__g_Func, "file_open_dialog()" );

		if ( !widget || !user_data ) return NULL;

		{
			char* filename = NULL;
			GtkWidget* dlg = NULL;


			dlg = gtk_file_chooser_dialog_new( "Open File",
					//(GtkWindow*)widget,

					// Parent Window
					// Gtk-CRITICAL: IA__gtk_window_set_transient_for
					(GtkWindow*)((Widgets_st*)user_data)->pWindow,

					GTK_FILE_CHOOSER_ACTION_OPEN,
					GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
					GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
					NULL );

			if ( !dlg ) {
				__LOGT__( TAG__g_Func, "file_open_dialog(): File Chooser Dialog == NULL" );
				return NULL;
			}

			if ( gtk_dialog_run(GTK_DIALOG(dlg)) == GTK_RESPONSE_ACCEPT ) {
				filename = gtk_file_chooser_get_filename( GTK_FILE_CHOOSER(dlg) );

				/*
				if ( filename ) {
					__LOGT__( TAG__g_Func, "file_open_dialog(): filename = %s\n", filename );

					// leave this, devolve release to caller
					g_free( filename );
				}
				*/
			}

			gtk_widget_destroy( dlg );

			return filename;
		}
	}

	// Input Text Dialog
	char* simple_text_input_dialog(GtkWidget* widget, gpointer user_data,
			const char* title, const char* desc, const char* old_text) {
		//__LOGT__( TAG__g_Func, "simple_text_input_dialog()" );

		if ( !widget || !user_data ) return NULL;

		{
			char* text = NULL;
			GtkWidget* dlg = NULL;
			GtkWidget* label_desc = NULL;
			GtkWidget* edit_text = NULL;
			GtkWidget* btn_font = NULL;
			GtkWidget* btn_font_color = NULL;
			GtkWidget* btn_bg_img = NULL;
			GtkWidget* hBox_font_and_color = NULL;
			GtkWidget* vBox = NULL;
			gint ret_dlg = GTK_RESPONSE_NONE;

			dlg = gtk_dialog_new_with_buttons( (title)? title : "",
							// use a transient parent for set layout to center-align onto parent
							(GtkWindow*)((Widgets_st*)user_data)->pWindow,

							GTK_DIALOG_MODAL,
							GTK_STOCK_OK, GTK_RESPONSE_OK,
							GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
							NULL );
			if ( !dlg ) {
				__LOGT__( TAG__g_Func, "simple_text_input_dialog(): Dialog == NULL" );
				return NULL;
			}

			//gtk_widget_set_size_request( dlg, 220, 100 );
			gtk_window_set_resizable( GTK_WINDOW(dlg), true );


			vBox = gtk_vbox_new( false, 0 );
			label_desc = gtk_label_new( (desc)? desc : "" );
			edit_text = gtk_entry_new();
			hBox_font_and_color = gtk_hbox_new( false, 0 );
			btn_font = gtk_button_new_with_label( "Font" );
			btn_font_color = gtk_button_new_with_label( "Color" );
			btn_bg_img = gtk_button_new_with_label( "Bg Image" );

			if ( !vBox || !label_desc || !edit_text ||
				 !hBox_font_and_color || !btn_font || !btn_font_color ) {
				if ( !vBox )
					__LOGT__( TAG__g_Func, "simple_text_input_dialog(): vbox == NULL" );

				if ( !label_desc )
					__LOGT__( TAG__g_Func, "simple_text_input_dialog(): Label(desc) == NULL" );

				if ( !edit_text )
					__LOGT__( TAG__g_Func, "simple_text_input_dialog(): Text Input == NULL" );

				if ( !hBox_font_and_color )
					__LOGT__( TAG__g_Func, "simple_text_input_dialog(): hbox(font and color) == NULL" );

				if ( !btn_font )
					__LOGT__( TAG__g_Func, "simple_text_input_dialog(): Button(font) == NULL" );

				if ( !btn_font_color )
					__LOGT__( TAG__g_Func, "simple_text_input_dialog(): Button(font color) == NULL" );

				if ( !btn_bg_img )
					__LOGT__( TAG__g_Func, "simple_text_input_dialog(): Button(bg image) == NULL" );



				return NULL;
			}

			if ( old_text ) {
				gtk_entry_set_text( GTK_ENTRY(edit_text), old_text );
			}

			gtk_label_set_justify( GTK_LABEL(label_desc), GTK_JUSTIFY_LEFT );

			// Layout
			{
				// Description, Text Input
				{
					gtk_box_pack_start( GTK_BOX(vBox), label_desc, false, false, 1 );
					gtk_box_pack_start( GTK_BOX(vBox), edit_text, false, false, 1 );
				}

				// Font, Color, Bg Image
				{
					gtk_box_pack_start( GTK_BOX(hBox_font_and_color), btn_font, false, false, 1 );
					gtk_box_pack_start( GTK_BOX(hBox_font_and_color), btn_font_color, false, false, 1 );
					gtk_box_pack_start( GTK_BOX(hBox_font_and_color), btn_bg_img, false, false, 1 );

					gtk_box_pack_start( GTK_BOX(vBox), hBox_font_and_color, false, false, 1 );
				}

				// Signal
				{
					//std::function<void (GtkWidget*, gpointer)> func = [](GtkWidget* widget, gpointer user_data) { };

					g_signal_connect( G_OBJECT(btn_font), "clicked",
									G_CALLBACK(g_FuncSignalHandler::on_text_font_dialog_open), user_data );
					g_signal_connect( G_OBJECT(btn_font_color), "clicked",
									G_CALLBACK(g_FuncSignalHandler::on_text_font_color_dialog_open), user_data );
					g_signal_connect( G_OBJECT(btn_bg_img), "clicked",
									G_CALLBACK(g_FuncSignalHandler::on_text_bg_image_dialog_open), user_data );
				}


				gtk_container_add( GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(dlg))), vBox );
			}

			gtk_widget_show_all( dlg );


			// for catch an interrupts
			//  - send a signal: gtk_dialog_response( GTK_DIALOG(__DIALOG__), GTK_RESPONSE_NONE );
			ret_dlg = gtk_dialog_run( GTK_DIALOG(dlg) );
			switch ( ret_dlg ) {
				case GTK_RESPONSE_OK:
					{
						//__LOGT__( TAG__g_Func, "simple_text_input_dialog(): OK Button clicked" );

						// Retrieve the text
						{
							const char* entry_text = (char*)gtk_entry_get_text( GTK_ENTRY(edit_text) );
							int entry_text_len = 0;

							if ( entry_text ) {
								entry_text_len = strlen( entry_text );

								if ( entry_text_len <= 0 ) {
									text = NULL;
								}
								else {
									//__LOGT__( TAG__g_Func, "simple_text_input_dialog(): text = %s, size = %d",
									//				entry_text, entry_text_len );

									entry_text_len += 1;	// '\0'
									text = new char[entry_text_len];
									if ( text ) {
										memset( (void*)text, 0x00, entry_text_len );
										snprintf( text, entry_text_len, "%s", entry_text );
									}
								}
							}
						}
					} break;
				case GTK_RESPONSE_CANCEL:
					{
						//__LOGT__( TAG__g_Func, "simple_text_input_dialog(): Cancel Button clicked" );
					} break;
				case GTK_RESPONSE_NONE:
					{
						//__LOGT__( TAG__g_Func, "simple_text_input_dialog(): Destroyed" );
					} break;
				default:
					{
						__LOGT__( TAG__g_Func, "simple_text_input_dialog(): Destroyed: Response code = %d", ret_dlg );
					} break;
			}


			// Release
			{
				if ( label_desc ) {
					gtk_widget_hide( label_desc );
					gtk_widget_destroy( label_desc );
					label_desc = NULL;
				}

				if ( edit_text ) {
					gtk_widget_hide( edit_text );
					gtk_widget_destroy( edit_text );
					edit_text = NULL;
				}

				if ( dlg ) {
					gtk_widget_hide( dlg );
					gtk_widget_destroy( dlg );
					dlg = NULL;
				}
			}


			return text;
		}
	}
} // namespace g_Func

// ---------------------------------------------------------------

