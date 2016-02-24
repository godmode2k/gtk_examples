/* --------------------------------------------------------------
Project:	ATP(A Tentative Plan) 3
Purpose:	
Author:		Ho-Jung Kim (godmode2k@hotmail.com)
Date:		Since Dec 2, 2014
Filename:	incl_g_func_signal_handler.cxx

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
 - DO NOT INCLUDE "incl_g_func_signal_handler.hxx" here
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

namespace g_FuncSignalHandler {
	//GtkFixed Resizing
	void on_event_gtkfixed_size_allocate(GtkWidget* widget, GdkRectangle* allocation, gpointer user_data) {
		//__LOGT__( TAG__g_FuncSignalHandler, "on_event_gtkfixed_size_allocate()" );

		//__LOGT__( TAG__g_FuncSignalHandler, "on_event_gtkfixed_size_allocate(): width = %d, height = %d",
		//			allocation->width, allocation->height );

		if ( !widget || !allocation ) return;

		if ( g_pCAtp3 ) {
			Widgets_st* wgt = g_pCAtp3->get_widget_st();

			if ( !wgt )
				return;

			gtk_widget_set_size_request( wgt->pDrawingArea,
										allocation->width, allocation->height );
			//GdkColor bgColor = { 0, 0x00, 0x00, 0x00 };	// black
			//gtk_widget_modify_bg( g_pCAtp3->get_widget_st()->pDrawingArea, GTK_STATE_NORMAL, &bgColor );


			{
				CViewMain* view = g_pCAtp3->get_view_main();

				if ( view ) {
					view->set_width_height( allocation->width, allocation->height );
				}
			}
		}
	}

	// MenuItem: Button: Open
	void on_menu_button_open_clicked(GtkWidget* widget, gpointer user_data) {
		//__LOGT__( TAG__g_FuncSignalHandler, "on_menu_button_open_clicked()" );

		if ( !widget || !user_data ) return;

		{
			// TEST
			/*
			{
				CDialog dlg;
				dlg.fileChooser( widget, user_data );
				return;
			}
			*/

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
				__LOGT__( TAG__g_FuncSignalHandler, "on_menu_button_open_clicked(): "
							"File Chooser Dialog == NULL" );
				return;
			}

			if ( gtk_dialog_run(GTK_DIALOG(dlg)) == GTK_RESPONSE_ACCEPT ) {
				char* filename;

				filename = gtk_file_chooser_get_filename( GTK_FILE_CHOOSER(dlg) );

				if ( filename ) {
					//open_file( filename );

					/*
					// Display Image
					__LOGT__( TAG__g_FuncSignalHandler, "on_menu_button_open_clicked(): filename = %s\n", filename );
					g_pEvtData->setImgSrcFilename( filename );
					default_display( widget, NULL );
					*/

					g_free( filename );
				}
			}

			gtk_widget_destroy( dlg );
		}
	}

	// Toolbar Button: Update
	void on_toolbar_button_update_clicked(GtkWidget* widget, gpointer user_data) {
		//__LOGT__( TAG__g_FuncSignalHandler, "on_toolbar_button_update_clicked()" );

		if ( !widget || !user_data ) return;

		{
			const char msg[] = "hehe\nhahaha, progress message test...\n     test1, test2, test3, test4, test5";
			const e_ShowProgress_t show_type = e_showProgress_ALL;
			// perform the task: SEE 'CProgressDlg::task_in_thread()'
			g_progressDlg.show( (GtkWindow*)((Widgets_st*)user_data)->pWindow, NULL, msg, show_type );
			//
			// Doesn't matter the following instance because CProgress_dlg used gtk_dialog_run().
			//CProgress_dlg dlg;
			//dlg.show( (GtkWindow*)((Widgets_st*)user_data)->pWindow, NULL );
		}
	}

	// Toolbar Button: ToastMsg
	void on_toolbar_button_toastmsg_clicked(GtkWidget* widget, gpointer user_data) {
		//__LOGT__( TAG__g_FuncSignalHandler, "on_toolbar_button_toastmsg_clicked()" );

		if ( !widget || !user_data ) return;

		{
			const char msg[] = "hehe\nhahaha, hello toast message dialog test...";
			// perform the task: SEE 'CToastMessageDlg::task_in_thread()'
			g_toastMsgDlg.show( (GtkWindow*)((Widgets_st*)user_data)->pWindow, NULL,
					msg, e_toastMsgShowDelay_SHORT, e_toastMsgShowDelay_CENTER );
			//
			// Don't use the following instance for local scope.
			// This isn't used gtk_dialog_run(), use this for global scope.
			//CToastMsg_dlg toastMsgDlg;
			//toastMsgDlg.show( (GtkWindow*)((Widgets_st*)user_data)->pWindow, NULL, "hello~" );
		}
	}

	// Toolbar Button: Add an Image
	void on_toolbar_button_add_img_clicked(GtkWidget* widget, gpointer user_data) {
		//__LOGT__( TAG__g_FuncSignalHandler, "on_toolbar_button_add_img_clicked()" );

		if ( !widget || !user_data ) return;

		if ( g_pCAtp3 ) {
			CViewMain* view = g_pCAtp3->get_view_main();
			if ( view ) {
				view->attach_set_action( e_attachActionType_ADD );
			}
		}

		{
			char* filename = g_Func::file_open_dialog( widget, user_data );

			if ( !filename ) {
				//__LOGT__( TAG__g_FuncSignalHandler,
				//			"on_toolbar_button_add_img_clicked(): Filename == NULL" );
				return;
			}

			//__LOGT__( TAG__g_FuncSignalHandler,
			//		"on_toolbar_button_add_img_clicked(): Filename = %s", filename );

			if ( g_pCAtp3 ) {
				CViewMain* view = g_pCAtp3->get_view_main();
				if ( view ) {
					//view->attach_add_image( "./res/img1.png", "toolbar_button: #1" );
					//view->attach_add_image( "./res/img2.png" );
					//view->attach_add_image( "./res/img00.jpg", "toolbar_button: #1" );

					view->attach_add_image( filename, "toolbar_button: #1" );

					view->invalidate();
				}
			}


			// Release
			if ( filename ) {
				g_free( filename );
				filename = NULL;
			}
		}
	}

	// Toolbar Button: Add a Text with Background Image
	void on_toolbar_button_add_text_with_bg_img_clicked(GtkWidget* widget, gpointer user_data) {
		//__LOGT__( TAG__g_FuncSignalHandler, "on_toolbar_button_add_text_bg_img_clicked()" );

		if ( !widget || !user_data ) return;

		if ( g_pCAtp3 ) {
			CViewMain* view = g_pCAtp3->get_view_main();
			if ( view ) {
				view->attach_set_action( e_attachActionType_ADD );
				view->set_tmp_font_info_select( false );
				view->set_tmp_bg_img_info_select( false );
			}
		}

		{
			char* text = NULL;
			//char* img_filename = NULL;
			const char* title = "Input a Text";
			const char* desc = "";


			// Text input dialog
			text = g_Func::simple_text_input_dialog( widget, user_data, title, desc, NULL );
			if ( !text ) {
				//__LOGT__( TAG__g_FuncSignalHandler,
				//			"on_toolbar_button_add_text_bg_img_clicked(): Text == NULL" );
				return;
			}

			/*
			img_filename = g_Func::file_open_dialog( widget, user_data );
			if ( !img_filename ) {
				__LOGT__( TAG__g_FuncSignalHandler,
							"on_toolbar_button_add_text_bg_img_clicked(): Image filename == NULL [FALSE]" );
				return;
			}
			*/

			if ( g_pCAtp3 ) {
				CViewMain* view = g_pCAtp3->get_view_main();
				if ( view ) {
					//view->attach_add_text( text, img_filename, "#2" );
					view->attach_add_text( text, NULL, "#2" );

					view->invalidate();
				}
			}


			// Release
			if ( text ) {
				delete text;
				text = NULL;
			}
			/*
			if ( img_filename ) {
				g_free( img_filename );
				img_filename = NULL;
			}
			*/
		}
	}

	// Toolbar Button: Delete an attached object
	void on_toolbar_button_delete_attached_obj_clicked(GtkWidget* widget, gpointer user_data) {
		//__LOGT__( TAG__g_FuncSignalHandler, "on_toolbar_button_delete_attached_obj_clicked()" );

		if ( !widget || !user_data ) return;

		{
			if ( g_pCAtp3 ) {
				CViewMain* view = g_pCAtp3->get_view_main();
				if ( view ) {
					view->attach_delete();
					view->invalidate();
				}
			}
		}
	}

	// Toolbar Button: Clear all
	void on_toolbar_button_clear_all(GtkWidget* widget, gpointer user_data) {
		//__LOGT__( TAG__g_FuncSignalHandler, "on_toolbar_button_clear_all()" );

		if ( !widget || !user_data ) return;

		{
			if ( g_pCAtp3 ) {
				CViewMain* view = g_pCAtp3->get_view_main();
				if ( view ) {
					view->attach_delete_all();
					view->invalidate();
				}
			}
		}
	}

	// Toolbar Button: Take a screenshot; region
	void on_toolbar_button_take_screenshot_region_clicked(GtkWidget* widget, gpointer user_data) {
		//__LOGT__( TAG__g_FuncSignalHandler, "on_toolbar_button_take_screenshot_region_clicked()" );

		if ( !widget || !user_data ) return;

		if ( g_pCAtp3 ) {
			CViewMain* view = g_pCAtp3->get_view_main();

			if ( view ) {
				if ( view->take_screenshot_get_region() ) {
					view->take_screenshot_set_region( false );

					// disabled 'save' button
					// ...
				}
				else {
					view->take_screenshot_set_region( true );

					// enabled 'save' button
					// ...
				}

				view->invalidate();
			}
		}
	}

	// Toolbar Button: Take a screenshot: save
	void on_toolbar_button_take_screenshot_save_clicked(GtkWidget* widget, gpointer user_data) {
		//__LOGT__( TAG__g_FuncSignalHandler, "on_toolbar_button_take_screenshot_save_clicked()" );

		if ( !widget || !user_data ) return;

		if ( g_pCAtp3 ) {
			CViewMain* view = g_pCAtp3->get_view_main();
			bool ret = false;

			if ( view ) {
				if ( view->take_screenshot_get_region() ) {
					view->take_screenshot_set_region( false, false );
					view->invalidate();

					ret = view->take_screenshot_save();
					if ( ret ) {
						const char* pathname = view->take_screenshot_get_pathname();
						const char msg_success[] = "Saved";
						char* msg_pathname = NULL;

						if ( pathname ) {
							const int len = 32 + strlen( pathname ) + 1;

							msg_pathname = new char[len];
							if ( msg_pathname ) {
								snprintf( msg_pathname, len, "Saved: %s", pathname );
								g_toastMsgDlg.show( (GtkWindow*)((Widgets_st*)user_data)->pWindow, NULL,
										msg_pathname, e_toastMsgShowDelay_SHORT, e_toastMsgShowDelay_CENTER );
							}
							else {
								g_toastMsgDlg.show( (GtkWindow*)((Widgets_st*)user_data)->pWindow, NULL,
										msg_success, e_toastMsgShowDelay_SHORT, e_toastMsgShowDelay_CENTER );
							}
						}
						else {
							g_toastMsgDlg.show( (GtkWindow*)((Widgets_st*)user_data)->pWindow, NULL,
									msg_success, e_toastMsgShowDelay_SHORT, e_toastMsgShowDelay_CENTER );
						}
					}
					else {
							const char msg_fail[] = "Saved [FALSE]";
							g_toastMsgDlg.show( (GtkWindow*)((Widgets_st*)user_data)->pWindow, NULL,
									msg_fail, e_toastMsgShowDelay_SHORT, e_toastMsgShowDelay_CENTER );
					}
				}
			}
		}
	}


	// ---------------------------------------------------------------
	

	// Popup Menu
	// ---------------------------------------------------------------
	// Popup Menu: Create a Popup Menu
	bool on_event_create_popup_menu(GtkWidget* widget, gpointer user_data, e_popupMenu_t type) {
		//__LOGT__( TAG__g_FuncSignalHandler, "on_event_create_popup_menu()" );

		if ( !widget || !user_data ) return false;

		bool ret = false;

		switch ( type ) {
			case e_popupMenu_UNKNOWN:
				{
					__LOGT__( TAG__g_FuncSignalHandler, "on_event_create_popup_menu(): "
							"e_popupMenu_UNKNOWN" );
				} break;
			case e_popupMenu_ATTACHMENT_OBJ:
				{
					//__LOGT__( TAG__g_FuncSignalHandler, "on_event_create_popup_menu(): "
					//		"e_popupMenu_ATTACHMENT_OBJ" );

					if ( g_pCAtp3 ) {
						CViewMain* view = g_pCAtp3->get_view_main();
						bool selected = false;
						e_ObjAttachType_t type = e_objAttachType_UNKNOWN;

						if ( view ) {
							CViewAttach* attach = view->attach_selected();

							if ( attach ) {
								selected = true;
								type = attach->get_obj_type();
							}

							view->set_tmp_font_info_select( false );
							view->set_tmp_bg_img_info_select( false );
						}

						ret = on_event_popup_menu_attachment( widget, user_data, selected, type );
					}
				} break;
			default:
				{
					__LOGT__( TAG__g_FuncSignalHandler, "on_event_create_popup_menu(): "
							"e_popupMenu_UNKNOWN");
				} break;
		}

		return ret;
	}

	bool on_event_popup_menu_attachment(GtkWidget* widget, gpointer user_data,
			bool selected, e_ObjAttachType_t type) {
		//__LOGT__( TAG__g_FuncSignalHandler, "on_event_popup_menu_attachment()" );

		if ( !widget || !user_data ) return false;

		{
			//const char title[] = "Object";
			GtkWidget* menu_item_add_image = NULL;
			GtkWidget* menu_item_add_text = NULL;
			GtkWidget* menu_item_edit_text = NULL;
			GtkWidget* menu_item_rotate_obj = NULL;
			GtkWidget* menu_item_del_obj = NULL;
			GtkWidget* menu_item_separator = NULL;

			//gtk_menu_set_title( (GtkMenu*)widget, title );
			menu_item_separator = gtk_separator_menu_item_new();
			menu_item_add_image = gtk_menu_item_new_with_label( "+Image" );
			menu_item_add_text = gtk_menu_item_new_with_label( "+Text" );
			menu_item_edit_text = gtk_menu_item_new_with_label( "Edit the Text" );
			menu_item_rotate_obj = gtk_menu_item_new_with_label( "Rotate" );
			menu_item_del_obj = gtk_menu_item_new_with_label( "-Image/Text" );


			if ( !menu_item_add_image || !menu_item_add_text || !menu_item_edit_text ||
					!menu_item_rotate_obj || !menu_item_del_obj || !menu_item_separator ) {
				__LOGT__( TAG__g_FuncSignalHandler, "on_event_popup_menu_attachment(): items == NULL" );
				return false;
			}


			// Indexing
			gtk_widget_set_name( menu_item_add_image, "0" );
			gtk_widget_set_name( menu_item_add_text, "1" );
			gtk_widget_set_name( menu_item_edit_text, "2" );
			gtk_widget_set_name( menu_item_rotate_obj, "3" );
			gtk_widget_set_name( menu_item_del_obj, "4" );


			g_signal_connect( G_OBJECT(menu_item_add_image), "activate",
							G_CALLBACK(g_FuncSignalHandler::on_event_popup_menu_clicked), user_data );
			g_signal_connect( G_OBJECT(menu_item_add_text), "activate",
							G_CALLBACK(g_FuncSignalHandler::on_event_popup_menu_clicked), user_data );
			g_signal_connect( G_OBJECT(menu_item_edit_text), "activate",
							G_CALLBACK(g_FuncSignalHandler::on_event_popup_menu_clicked), user_data );
			g_signal_connect( G_OBJECT(menu_item_rotate_obj), "activate",
							G_CALLBACK(g_FuncSignalHandler::on_event_popup_menu_clicked), user_data );
			g_signal_connect( G_OBJECT(menu_item_del_obj), "activate",
							G_CALLBACK(g_FuncSignalHandler::on_event_popup_menu_clicked), user_data );


			gtk_menu_shell_append( GTK_MENU_SHELL(widget), menu_item_add_image );
			gtk_menu_shell_append( GTK_MENU_SHELL(widget), menu_item_add_text );
			gtk_menu_shell_append( GTK_MENU_SHELL(widget), menu_item_separator );
			gtk_menu_shell_append( GTK_MENU_SHELL(widget), menu_item_edit_text );
			gtk_menu_shell_append( GTK_MENU_SHELL(widget), menu_item_rotate_obj );
			gtk_menu_shell_append( GTK_MENU_SHELL(widget), menu_item_del_obj );


			// Item Disabled
			{
				if ( type == e_objAttachType_IMAGE ) {
					gtk_widget_set_sensitive( menu_item_edit_text, false );
				}

				if ( !selected ) {
					gtk_widget_set_sensitive( menu_item_edit_text, selected );
					gtk_widget_set_sensitive( menu_item_rotate_obj, selected );
					gtk_widget_set_sensitive( menu_item_del_obj, selected );
				}
			}


			//gtk_menu_attach_to_widget( GTK_MENU(widget), ..., NULL );
		}


		return true;
	}

	// Popup Menu: Popup Menu Clicked
	bool on_event_popup_menu_clicked(GtkMenuItem* item, gpointer user_data) {
		//__LOGT__( TAG__g_FuncSignalHandler, "on_event_popup_menu_clicked()" );

		//if ( !item || !widget || !user_data ) return false;
		if ( !item || !user_data ) return false;

		{
			//const char* label = gtk_menu_item_get_label( item );
			//__LOGT__( TAG__g_FuncSignalHandler, "on_event_popup_menu_clicked(): label = %s", label );

			if ( g_pCAtp3 ) {
				CViewMain* view = g_pCAtp3->get_view_main();
				const char* tag = gtk_widget_get_name( GTK_WIDGET(item) );

				if ( view && tag ) {
					int index = atoi( tag );

					//__LOGT__( TAG__g_FuncSignalHandler, "on_event_popup_menu_clicked(): %d, label = %s",
					//			index, label );

					switch ( index ) {
						case 0:
							// Add an Image
							{
								// Add an Image
								//view->attach_add_image( "./res/img1.png", "toolbar_button: #1" );
								//view->attach_add_image( "./res/img2.png" );


								{
									view->attach_set_action( e_attachActionType_ADD );
								}


								//GtkWindow* window = (GtkWindow*)((Widgets_st*)user_data)->pWindow;
								GtkWidget* window = ((Widgets_st*)user_data)->pWindow;
								char* filename = g_Func::file_open_dialog( window, user_data );

								if ( !filename ) {
									//__LOGT__( TAG__g_FuncSignalHandler,
									//			"on_event_popup_menu_clicked(): Filename == NULL" );
									return false;
								}


								view->attach_add_image( filename, "toolbar_button: #1" );


								// Release
								g_free( filename );
								filename = NULL;
							} break;
						case 1:
							// Add a Text
							{
								// Add a Text
								//view->attach_add_text( "toolbar_button: 0test...1test...2test...", NULL, "#2" );
								//view->attach_add_text( "0test...", "./res/img2.png", "#3" );
								//view->attach_add_text( "1test...", "./res/attach_balloon_1.9.png", "#4" );


								{
									view->attach_set_action( e_attachActionType_ADD );
								}


								char* text = NULL;
								const char* title = "Input a Text";
								const char* desc = "";
								//GtkWindow* window = (GtkWindow*)((Widgets_st*)user_data)->pWindow;
								GtkWidget* window = ((Widgets_st*)user_data)->pWindow;


								// Text input dialog
								text = g_Func::simple_text_input_dialog( window, user_data, title, desc, NULL );
								if ( !text ) {
									//__LOGT__( TAG__g_FuncSignalHandler,
									//			"on_event_popup_menu_clicked(): Text == NULL" );
									return false;
								}


								view->attach_add_text( text, NULL, "#2" );


								// Release
								delete text;
								text = NULL;
							} break;
						case 2:
							// Edit a Text and an Image
							{
								// Edit a Text
								//view->attach_edit_text( "toolbar_button: 0test...1test...2test...", NULL, "#2" );
								//view->attach_edit_text( "0test...", "./res/img2.png", "#3" );
								//view->attach_edit_text( "1test...", "./res/attach_balloon_1.9.png", "#4" );


								{
									view->attach_set_action( e_attachActionType_EDIT );
								}


								char* text = NULL;
								//char* img_filename = NULL;
								const char* title = "Input a Text";
								const char* desc = "";
								char* old_text = NULL;
								//GtkWindow* window = (GtkWindow*)((Widgets_st*)user_data)->pWindow;
								GtkWidget* window = ((Widgets_st*)user_data)->pWindow;

								{
									CViewAttach* attach = view->attach_selected();
									if ( !attach ) {
										__LOGT__( TAG__g_FuncSignalHandler,
													"on_event_popup_menu_clicked(): Attach object == NULL" );
										return false;
									}

									old_text = (char*)attach->get_text();
								}

								// Text input dialog
								text = g_Func::simple_text_input_dialog( window, user_data, title, desc, old_text );
								if ( !text ) {
									//__LOGT__( TAG__g_FuncSignalHandler,
									//			"on_event_popup_menu_clicked(): Text == NULL" );
									return false;
								}

								/*
								// Background image file open dialog
								img_filename = g_Func::file_open_dialog( window, user_data );
								if ( !img_filename ) {
									__LOGT__( TAG__g_FuncSignalHandler,
												"on_event_popup_menu_clicked(): Image filename == NULL [FALSE]; SKIP" );
								}
								*/


								//view->attach_edit_text( text, img_filename, "#3" );
								view->attach_edit_text( text, NULL, "#3" );


								// Release
								if ( text ) {
									delete text;
									text = NULL;
								}
								/*
								if ( img_filename ) {
									g_free( img_filename );
									img_filename = NULL;
								}
								*/
							} break;
						case 3:
							// Rotate the object
							{
								view->attach_rotate();
							} break;
						case 4:
							// Delete the object
							{
								// Delete an attached object
								view->attach_delete();
							} break;
						default:
							{
								index = -1;
								__LOGT__( TAG__g_FuncSignalHandler, "on_event_popup_menu_clicked(): index = %d", index );
							} break;
					}

					if ( index >= 0 ) {
						view->invalidate();
					}
				}
			}

		}


		return false;
	}




	//! TEST: GtkTextView
	/*
	{
		GtkWidget* pMainWindow = NULL;
		GtkWidget* pToastMsg = NULL;
		GtkTextBuffer* pToastMsgBuf = NULL;


		pMainWindow = ((Widgets_st*)user_data)->pWindow;
		if ( !pMainWindow )
			return;

		((Widgets_st*)user_data)->pToastMsg = gtk_text_view_new();
		pToastMsg = ((Widgets_st*)user_data)->pToastMsg;
		if ( !pToastMsg )
			return;

		//((Widgets_st*)user_data)->pToastMsgBuf =
		pToastMsgBuf =
			gtk_text_view_get_buffer( GTK_TEXT_VIEW(pToastMsg) );
		//pToastMsgBuf = ((Widgets_st*)user_data)->pToastMsgBuf;
		if ( !pToastMsgBuf )
			return;

		gtk_text_buffer_set_text( pToastMsgBuf, "test message...", -1 );

		gtk_container_add( GTK_CONTAINER(pMainWindow), pToastMsg );
		gtk_widget_show_all( pToastMsg );


		__usleep( 3000 );

		gtk_widget_hide( pToastMsg );
		gtk_widget_destroy( pToastMsg );
	}
	*/


	// ---------------------------------------------------------------
	

	// Font
	// ---------------------------------------------------------------
	void on_text_font_dialog_open(GtkWidget* widget, gpointer user_data) {
		//__LOGT__( TAG__g_FuncSignalHandler, "on_text_font_dialog_open()" );

		if ( !widget || !user_data ) return;

		{
			const char* title = "Font";
			GtkWidget* dlg = NULL;
			char font_name[DEFAULT_TEXT_FONT_FAMILY_SIZE_ALL] = { 0, };

			if ( g_pCAtp3 ) {
				CViewMain* view = g_pCAtp3->get_view_main();

				if ( view ) {
					CViewAttach* attach = view->attach_selected();

					if ( attach && (view->attach_get_action() == e_attachActionType_EDIT) ) {
						const char* family_name = attach->get_text_font_face();
						const e_ObjAttachFontTypeface_t typeface = attach->get_text_font_typeface();
						const char* typeface_str =
							(attach->get_text_font_typeface() == e_objAttachFontTypeface_NORMAL)?
								"Regular" : attach->get_text_font_typeface_str(false);
						const bool bold = attach->get_text_font_typeface_bold();
						const int size = attach->get_text_font_size();

						if ( bold )
							snprintf( font_name, sizeof(font_name), "%s Bold %s %d", family_name, typeface_str, size );
						else
							snprintf( font_name, sizeof(font_name), "%s %s %d", family_name, typeface_str, size );


						//__LOGT__( TAG__g_FuncSignalHandler, "on_text_font_dialog_open(): ===== OLD FONT INFO ===== {" );
						//attach->show_text_font_info_all();
						//__LOGT__( TAG__g_FuncSignalHandler, "on_text_font_dialog_open(): ===== OLD FONT INFO ===== }" );


						{
							// Temporary
							view->set_tmp_font_info( family_name, typeface, bold, size );
						}
					}
				}
			}

#ifdef __GTKv2__
			dlg = gtk_font_selection_dialog_new( title );
			if ( !dlg ) {
				__LOGT__( TAG__g_FuncSignalHandler, "on_text_font_dialog_open(): Dialog == NULL" );
				return;
			}

			//gtk_window_set_modal( GTK_WINDOW(dlg), true );
			gtk_font_selection_dialog_set_font_name( GTK_FONT_SELECTION_DIALOG(dlg),
					//"Sans Regular 12" );		// "Sans Bold Italic 12"
					font_name );

			gtk_font_selection_dialog_set_preview_text( GTK_FONT_SELECTION_DIALOG(dlg),
					"Preview Test" );

			g_signal_connect( G_OBJECT(dlg), "response",
							G_CALLBACK(g_FuncSignalHandler::on_event_text_font_dialog_open), user_data );
#elif __GTKv3__
#if 0
			//GtkWindow* window = (GtkWindow*)((Widgets_st*)user_data)->pWindow;
			GtkWidget* window = ((Widgets_st*)user_data)->pWindow;

			if ( window ) {
				dlg = gtk_font_chooser_dialog_new( title, window );
				if ( !dlg ) {
					__LOGT__( TAG__g_FuncSignalHandler, "on_text_font_dialog_open(): Dialog == NULL" );
					return;
				}

				//gtk_window_set_modal( GTK_WINDOW(dlg), true );
				gtk_font_chooser_set_font_name( GTK_FONT_CHOOSER_DIALOG(dlg),
						//"Sans Regular 12" );		// "Sans Bold Italic 12"
						font_name );
				gtk_font_chooser_set_preview_text( GTK_FONT_CHOOSER_DIALOG(dlg),
						"Preview Test" );

				g_signal_connect( G_OBJECT(dlg), "response",
								G_CALLBACK(g_FuncSignalHandler::on_event_text_font_dialog_open), user_data );
			}
#endif
#else
#endif

			gtk_widget_show_all( dlg );
		}
	}

	void on_text_font_color_dialog_open(GtkWidget* widget, gpointer user_data) {
		//__LOGT__( TAG__g_FuncSignalHandler, "on_text_font_color_dialog_open()" );

		if ( !widget || !user_data ) return;

		{
			const char* title = "Color";
			bool domodal = true;
			GtkWidget* dlg = NULL;
			GtkWidget* colorsel = NULL;
			GdkColor color { 0, };
			guint alpha = 65535;


			if ( g_pCAtp3 ) {
				CViewMain* view = g_pCAtp3->get_view_main();

				if ( view ) {
					CViewAttach* attach = view->attach_selected();

					if ( attach && (view->attach_get_action() == e_attachActionType_EDIT)) {
						guint16 a = attach->get_text_font_color().a;
						guint16 r = attach->get_text_font_color().r;
						guint16 g = attach->get_text_font_color().g;
						guint16 b = attach->get_text_font_color().b;

						alpha = a;
						color.red = r;
						color.green = g;
						color.blue = b;

						//__LOGT__( TAG__g_FuncSignalHandler,
						//			"on_text_font_color_dialog_open(): ===== OLD FONT COLOR INFO ===== {" );
						//__LOGT__( TAG__g_FuncSignalHandler,
						//			"on_text_font_color_dialog_open(): FRACTION VALUE: (0 to 1)" );
						//__LOGT__( TAG__g_FuncSignalHandler,
						//			"on_text_font_color_dialog_open(): A(%d, %X), R(%d, %X), G(%d, %X), B(%d, %X)",
						//			alpha, PAINT_COLOR_UINT16_8(alpha),
						//			color.red, PAINT_COLOR_UINT16_8(color.red),
						//			color.green, PAINT_COLOR_UINT16_8(color.green),
						//			color.blue, PAINT_COLOR_UINT16_8(color.blue) );
						//attach->show_text_font_info_color();
						//__LOGT__( TAG__g_FuncSignalHandler,
						//			"on_text_font_color_dialog_open(): ===== OLD FONT COLOR INFO ===== }" );


						{
							// Temporary
							ColorARGB_st color;

							color.a = a;
							color.r = r;
							color.g = g;
							color.b = b;

							// Color
							view->set_tmp_font_info_color( color );
						}
					}
				}
			}

#ifdef __GTKv2__
			dlg = gtk_color_selection_dialog_new( title );
			if ( !dlg ) {
				__LOGT__( TAG__g_FuncSignalHandler, "on_text_font_color_dialog_open(): Dialog == NULL" );
				return;
			}

			gtk_window_set_modal( GTK_WINDOW(dlg), domodal );

			colorsel = GTK_COLOR_SELECTION_DIALOG(dlg)->colorsel;
			if ( colorsel ) {
				gtk_color_selection_set_has_opacity_control( GTK_COLOR_SELECTION(colorsel), true );
				gtk_color_selection_set_current_color( GTK_COLOR_SELECTION(colorsel), &color );
				gtk_color_selection_set_current_alpha( GTK_COLOR_SELECTION(colorsel), alpha );

				gtk_widget_show_all( dlg );
			}

			g_signal_connect( G_OBJECT(dlg), "response",
							G_CALLBACK(g_FuncSignalHandler::on_event_text_font_color_dialog_open), user_data );
#elif __GTKv3__
#else
#endif
		}
	}

	void on_text_bg_image_dialog_open(GtkWidget* widget, gpointer user_data) {
		//__LOGT__( TAG__g_FuncSignalHandler, "on_text_bg_image_dialog_open()" );

		if ( !widget || !user_data ) return;

		{
			char* img_filename = NULL;

			// Background image file open dialog
			img_filename = g_Func::file_open_dialog( widget, user_data );
			if ( !img_filename ) {
				//__LOGT__( TAG__g_FuncSignalHandler,
				//			"on_text_bg_image_dialog_open(): Image filename == NULL [FALSE]; SKIP" );
				return;
			}

			if ( g_pCAtp3 ) {
				CViewMain* view = g_pCAtp3->get_view_main();

				if ( view ) {
					view->set_tmp_bg_img_info( img_filename );
				}
			}


			// Release
			if ( img_filename ) {
				g_free( img_filename );
				img_filename = NULL;
			}
		}
	}

	void on_event_text_font_dialog_open(GtkFontSelectionDialog* dlg, gint response,
			gpointer user_data) {
		//__LOGT__( TAG__g_FuncSignalHandler, "on_event_text_font_dialog_open()" );

		if ( !dlg || !user_data ) return;

		{
			char* font_name = NULL;

#ifdef __GTKv2__
			switch ( response ) {
				case GTK_RESPONSE_APPLY:
				case GTK_RESPONSE_OK:
					{
						//font_name = gtk_font_selection_dialog_get_font_name( dlg );
						font_name = gtk_font_selection_get_font_name( GTK_FONT_SELECTION(dlg->fontsel) );
						//__LOGT__( TAG__g_FuncSignalHandler,
						//			"on_event_text_font_dialog_open(): Font name = %s", font_name );

						{
							PangoFontFamily* font_family = gtk_font_selection_get_family( GTK_FONT_SELECTION(dlg->fontsel) );
							PangoFontFace* font_face = gtk_font_selection_get_face( GTK_FONT_SELECTION(dlg->fontsel) );
							// Size
							//  - NOTE: from the GTK+ 2.24 source(gtkfontsel.c:1089 gtk_font_selection_select_size()),
							//  - new size = (selected size * PANGO_SCALE)
							int size = gtk_font_selection_get_size( GTK_FONT_SELECTION(dlg->fontsel) );
							size /= PANGO_SCALE;

							if ( font_family && font_face) {
								// Family name
								const char* family_name = pango_font_family_get_name( font_family );
								// Typefaces (style)
								const char* face_name = pango_font_face_get_face_name( font_face );

								//__LOGT__( TAG__g_FuncSignalHandler,
								//			"on_event_text_font_dialog_open(): family name = %s"
								//			", face name = %s, size = %d",
								//			family_name, face_name, size );

								{
									CViewMain* view = g_pCAtp3->get_view_main();

									if ( view ) {
										//CViewAttach* attach = view->attach_selected();

										e_ObjAttachFontTypeface_t typeface = e_objAttachFontTypeface_NORMAL;
										bool bold = false;

										// Typefaces (style)
										{
											if ( strstr(face_name, "regular") || strstr(face_name, "Regular") ||
												 strstr(face_name, "REGULAR") ) {
												typeface = e_objAttachFontTypeface_NORMAL;
											}
											else {
												if ( strstr(face_name, "italic") || strstr(face_name, "Italic") ||
													 strstr(face_name, "ITALIC") ) {
													typeface = e_objAttachFontTypeface_ITALIC;
													//! ITALIC, OBLIQUE doesn't work in Cairo here
												}

												if ( strstr(face_name, "bold") || strstr(face_name, "Bold") ||
													 strstr(face_name, "BOLD") ) {
													bold = true;
												}
											}
										}

										// Temporary
										view->set_tmp_font_info( family_name, typeface, bold, size );
									}
								}
							}
						}

						if ( font_name ) {
							g_free( font_name );
							font_name = NULL;
						}
					} break;
			}
#elif __GTKv3__
#if 0
			switch ( response ) {
				case GTK_RESPONSE_OK:
					{
						font_name = gtk_font_chooser_get_font( dlg );
						//__LOGT__( TAG__g_FuncSignalHandler,
						//			"on_event_text_font_dialog_open(): Font name = %s", font_name );

						{
							PangoFontFamily* font_family = gtk_font_chooser_get_font_family( dlg );
							PangoFontFace* font_face = gtk_font_chooser_get_font_face( dlg );
							// Size
							int size = gtk_font_chooser_get_font_size( dlg );

							if ( font_family && font_face ) {
								// Family name
								const char* family_name = pango_font_family_get_name( font_family );
								// Typefaces (style)
								const char* face_name = pango_font_face_get_face_name( font_face );

								//__LOGT__( TAG__g_FuncSignalHandler,
								//			"on_event_text_font_dialog_open(): family name = %s"
								//			" face name = %s, size = %d",
								//			family_name, face_name, size );

								{
									CViewMain* view = g_pCAtp3->get_view_main();

									if ( view ) {
										CViewAttach* attach = view->attach_selected();

										if ( attach ) {
											// Font: "Sans Bold Italic 12"
											//
											// Family name
											// Typefaces (style)
											// Size
										}
										else {
											__LOGT__( TAG__g_FuncSignalHandler,
														"on_event_text_font_dialog_open(): Attach object == NULL" );
										}
									}
								}
							}
						}

						if ( font_name ) {
							g_free( font_name );
							font_name = NULL;
						}
					} break;
			}
#endif
#else
#endif

			gtk_widget_destroy( GTK_WIDGET(dlg) );
		}
	}

	void on_event_text_font_color_dialog_open(GtkFontSelectionDialog* dlg, gint response,
			gpointer user_data) {
		//__LOGT__( TAG__g_FuncSignalHandler, "on_event_text_font_color_dialog_open()" );

		if ( !dlg || !user_data ) return;

		{
			GtkWidget* colorsel = NULL;
			GdkColor color { 0, };
			guint16 alpha = 0;

#ifdef __GTKv2__
			switch ( response ) {
				case GTK_RESPONSE_HELP:
					{
					} break;
				case GTK_RESPONSE_OK:
					{
						colorsel = GTK_COLOR_SELECTION_DIALOG(dlg)->colorsel;
						if ( colorsel ) {
							gtk_color_selection_get_current_color( GTK_COLOR_SELECTION(colorsel), &color );
							alpha = gtk_color_selection_get_current_alpha( GTK_COLOR_SELECTION(colorsel) );

							//__LOGT__( TAG__g_FuncSignalHandler,
							//			"on_event_text_font_color_dialog_open(): color(RGBA) = #%X%X%X%X"
							//			", A(%d), R(%d), G(%d), B(%d)",
							//			color.red, color.green, color.blue, alpha,
							//			PAINT_COLOR_UINT16_8(alpha),
							//			PAINT_COLOR_UINT16_8(color.red),
							//			PAINT_COLOR_UINT16_8(color.green),
							//			PAINT_COLOR_UINT16_8(color.blue) );

							{
								CViewMain* view = g_pCAtp3->get_view_main();

								if ( view ) {
									{
										// Temporary
										ColorARGB_st color_argb;

										color_argb.a = alpha;
										color_argb.r = color.red;
										color_argb.g = color.green;
										color_argb.b = color.blue;

										// Color
										view->set_tmp_font_info_color( color_argb );
									}
								}
							}
						}
					} break;
			}
#elif __GTKv3__
#else
#endif

			gtk_widget_destroy( GTK_WIDGET(dlg) );
		}
	}


	// ---------------------------------------------------------------


	// Keyboard & Mouse Event
	// ---------------------------------------------------------------
	void on_event_key_press(GtkWidget* widget, GdkEvent* event, gpointer user_data) {
		//__LOGT__( TAG__g_FuncSignalHandler, "on_event_key_press()" );

		if ( !widget || !event ) return;

		//__LOGT__( TAG__g_FuncSignalHandler, "on_event_key_press(): KEY = %s",
		//		event->key.string );

		if ( g_pCAtp3 ) {
			//g_pCAtp3->get_key_event().key( event, true, NULL, NULL );
			g_pCAtp3->get_key_event().key( event, true, NULL );
		}
	}

	void on_event_key_release(GtkWidget* widget, GdkEvent* event, gpointer user_data) {
		//__LOGT__( TAG__g_FuncSignalHandler, "on_event_key_release()" );
		
		if ( !widget || !event ) return;

		//__LOGT__( TAG__g_FuncSignalHandler, "on_event_key_release(): KEY = %s",
		//		event->key.string );

		if ( g_pCAtp3 ) {
			//g_pCAtp3->get_key_event().key( event, false, NULL, NULL );
			g_pCAtp3->get_key_event().key( event, false, NULL );
		}
	}

	void on_event_mouse(GtkWidget* widget, GdkEvent* event, gpointer user_data) {
		//__LOGT__( TAG__g_FuncSignalHandler, "on_event_mouse()" );

		if ( !widget || !event || !user_data ) return;

		if ( g_pCAtp3 ) {
			//g_pCAtp3->get_key_event().mouse( event, NULL, g_pCAtp3->get_view_main() );
			g_pCAtp3->get_key_event().mouse( event, NULL );


			{
				CViewMain* view = g_pCAtp3->get_view_main();
				if ( view ) {
					view->onTouchEvent( &g_pCAtp3->get_key_event() );
				}
			}

			{
				// Popup Menu
				if ( g_pCAtp3->get_key_event().is_mouse_rbtn() ) {
					//__LOGT__( TAG__g_FuncSignalHandler, "on_event_mouse(): Popup Menu" );

					if ( !g_pCAtp3->create_popup_menu(e_popupMenu_ATTACHMENT_OBJ, true) ) {
						__LOGT__( TAG__g_FuncSignalHandler, "on_event_mouse(): Popup Menu [FALSE]" );
						g_pCAtp3->release_popup_menu();
					}
				}
			}
		}
	}


	// ---------------------------------------------------------------


	// onDraw()
	// ---------------------------------------------------------------
	/*
	void on_event_redraw(void) {
		//__LOGT__( TAG__g_FuncSignalHandler, "on_event_redraw()" );

		if ( g_pCAtp3 ) {
			Widgets_st* wgt = g_pCAtp3->get_widget_st();

			if ( wgt )
				gtk_widget_queue_draw( wgt->pDrawingArea );
		}
	}
	*/

#if 0
	bool on_event_draw_top_window(GtkWidget* widget, GdkEventExpose* event, gpointer user_data) {
		//__LOGT__( TAG__g_FuncSignalHandler, "on_event_draw_top_window()" );

		if ( !widget || !event || !user_data ) return;

		/*
		if ( !gtk_widget_get_app_paintable(widget) ) {
			gtk_paint_flat_box( widget->style, widget->window, GTK_STATE_NORMAL,
									GTK_SHADOW_NONE, (GdkRectangle*)&event->area, widget,
									"base", 0, 0, 100, 100 );

			__LOGT__( TAG__g_FuncSignalHandler, "on_event_draw_top_window(): "
						"rect x = %d, y = %d, w = %d, h = %d",
							event->area.x, event->area.y, event->area.width, event->area.height );

			//if ( GTK_WIDGET_CLASS(gtk_window_parent_class)->expose_event )
			//	return GTK_WIDGET_CLASS(gtk_window_parent_clas)->expose_event( widget, event );
		}
		*/

		{
			//! NOTE:
			/*  The GTK+ Drawing Model
				Source: https://developer.gnome.org/gtk2/stable/chap-drawing-model.html

				Summary of app-paintable widgets.
				Turn on the GTK_APP_PAINTABLE flag if you intend to draw your own content directly on
				a GtkWindow and GtkEventBox. You seldom need to draw on top of other widgets, and
				GtkDrawingArea ignores this flag, as it is intended to be drawn on.
			*/

			/*
			// for GtkWindow, GtkEventBox
			{
				GdkRectangle rect;
				//GtkWidget* pWindow = ((Widgets_st*)user_data)->pWindow;
				//if ( !pWindow ) {
				//	__LOGT__( TAG__g_FuncSignalHandler, "on_event_draw_top_window(): Top Window == NULL" );
				//	return false;
				//}

				rect.x = 0;
				rect.y = 0;
				rect.width = 640;
				rect.height = 700;

				gtk_paint_flat_box( widget->style, widget->window, GTK_STATE_NORMAL,
										GTK_SHADOW_NONE, (GdkRectangle*)&rect, widget,
										"base", 0, 0, 300, 600 );
			}

			// for GtkDrawingArea
			{
				// is GtkDrawingArea on the top?
				// ...
			}
			*/
		}



		return false;
	}
#endif


#ifdef __GTKv2__
	// Main Drawing View
	//
	// v2.x: "expose-event"
	/*
	bool on_event_draw_main(GtkWidget* widget, GdkEventExpose* event, gpointer user_data) {
		//__LOGT__( TAG__g_FuncSignalHandler, "on_event_draw_main()" );

		if ( !widget || !event || !user_data ) return;

		GtkAllocation timelineWindowSize;
		//cairo_t** cr = NULL;
		cairo_t* cr = NULL;
		const double rowHeight = 20.0f;

		//cairo_t* pTimelineCairo;
		if ( ((Widgets_st*)user_data)->pTimelineCairo ) {
			cairo_destroy( ((Widgets_st*)user_data)->pTimelineCairo );
			((Widgets_st*)user_data)->pTimelineCairo = NULL;
		}

		((Widgets_st*)user_data)->pTimelineCairo = gdk_cairo_create( widget->window );
		//cr = &((Widgets_st*)user_data)->pTimelineCairo;
		cr = ((Widgets_st*)user_data)->pTimelineCairo;
		//printf( "local = %p, user_data = %p\n", cr, ((Widgets_st*)user_data)->pTimelineCairo );

		if ( cr == NULL ) {
			return false;
		}

		timelineWindowSize.width = 0;
		timelineWindowSize.height = 0;
		gtk_widget_get_allocation( widget, &timelineWindowSize );

		// ...

		return false;
	}
	*/
	bool on_event_draw_main(GtkWidget* widget, GdkEventExpose* event, gpointer user_data) {
		//__LOGT__( TAG__g_FuncSignalHandler, "on_event_draw_main()" );

		if ( !widget || !event || !user_data ) return false;

		GtkAllocation windowSize;
		cairo_t* cr = NULL;
		cairo_surface_t* cr_surface = NULL;
		bool rgba = true;

		// OpenGL
		//GdkGLWindow* gl_window = gdk_window_get_gl_window( widget->window );
		//GdkGLDrawable* gl_drawable = NULL;
		GdkGLContext* gl_context = gtk_widget_get_gl_context( widget );
		GdkGLDrawable* gl_drawable = gtk_widget_get_gl_drawable( widget );
		bool gl_begin = false;


		windowSize.width = 0;
		windowSize.height = 0;
		gtk_widget_get_allocation( widget, &windowSize );


//#if 0
		// OpenGL
		{
			if ( !gl_context ) {
				__LOGT__( TAG__g_FuncSignalHandler, "on_event_draw_main(): GdkGLContext == NULL" );
				return false;
			}

			/*
			if ( gl_window ) {
				gl_drawable = GDK_GL_DRAWABLE( gl_window );
			}
			else {
				__LOGT__( TAG__g_FuncSignalHandler, "on_event_draw_main(): GdkGLWindow == NULL" );
			}
			*/

			{
				if ( gl_drawable ) {
					gl_begin = gdk_gl_drawable_gl_begin( gl_drawable, gl_context );
					if ( !gl_begin ) {
						__LOGT__( TAG__g_FuncSignalHandler, "on_event_draw_main(): gl begin [FALSE]" );
						return false;
					}

					/*
					if ( gdk_gl_drawable_is_double_buffered(gl_drawable) ) {
						gdk_gl_drawable_swap_buffers( gl_drawable );
					}
					else {
						__LOGT__( TAG__g_FuncSignalHandler, "on_event_draw_main(): double-buffered [FALSE]" );
						glFlush();
					}

					gdk_gl_drawable_gl_end( gl_drawable );
					*/

					{
						glClearColor( 255.0f, 255.0f, 255.0f, 0.0f );
						glClear( GL_COLOR_BUFFER_BIT );
						glPushMatrix();
						glShadeModel( GL_FLAT );

						glBegin( GL_LINES );
						glColor3f( 1.0f, 0.0f, 0.0f );
						glVertex3f( 0.0f, 0.0f, 0.0f );
						glVertex3f( 10.0f, 0.0f, 0.0f );
						glEnd();

						glPopMatrix();
						//glXSwapBuffers( g_pCAtp3->x_display, g_pCAtp3->x_window );
					}
				}
				else {
					__LOGT__( TAG__g_FuncSignalHandler, "on_event_draw_main(): GdkGLDrawable == NULL" );
					return false;
				}
			}
			//glXSwapBuffers( g_pCAtp3->x_display, g_pCAtp3->x_window );
		}
		/*
		{
		//	__LOGT__( TAG__g_FuncSignalHandler, "on_event_draw_main(): gl clear" );
		//	//glClearColor( 255.0f, 255.0f, 255.0f, 0.0f );
		//	//glClear( GL_COLOR_BUFFER_BIT );
			glXSwapBuffers( g_pCAtp3->x_display, g_pCAtp3->x_window );
		}
		*/
//#endif


		// Cairo
		{
			if ( ((Widgets_st*)user_data)->pCanvas ) {
				cairo_destroy( ((Widgets_st*)user_data)->pCanvas );
				((Widgets_st*)user_data)->pCanvas = NULL;
			}
			//((Widgets_st*)user_data)->pCanvas = gdk_cairo_create( widget->window );
			//
			cr_surface = cairo_image_surface_create( CAIRO_FORMAT_ARGB32, windowSize.width, windowSize.height );
			if ( !cr_surface ) {
				__LOGT__( TAG__g_FuncSignalHandler, "on_event_draw_main(): Canvas surface == NULL" );
				return false;
			}
			((Widgets_st*)user_data)->pCanvas = cairo_create( cr_surface );

			cr = ((Widgets_st*)user_data)->pCanvas;
			if ( cr == NULL ) {
				return false;
			}
		}


		// RGBA
		{
			GdkScreen* screen = gtk_widget_get_screen( widget );
			GdkColormap* colormap = gdk_screen_get_rgba_colormap( screen );

			if ( !colormap ) {
				__LOGT__( TAG__g_FuncSignalHandler, "on_event_draw_main(): Support RGBA [FALSE]" );

				//colormap = gdk_screen_get_rgb_colormap( screen );
				rgba = false;
			}

			//gtk_widget_set_colormap( widget, colormap );
		}


		//windowSize.width = 0;
		//windowSize.height = 0;
		//gtk_widget_get_allocation( widget, &windowSize );

		if ( g_pCAtp3 ) {
			CViewMain* view = g_pCAtp3->get_view_main();

			if ( view ) {
				view->set_top_view( widget );
				//view->set_top_view( ((Widgets_st*)user_data)->pDrawingArea );
				//view->set_top_view_ref( ((Widgets_st*)user_data)->pDrawingArea );
				view->set_rgba( rgba );
				view->set_canvas( ((Widgets_st*)user_data)->pCanvas );
				//view->set_canvas_ref( ((Widgets_st*)user_data)->pCanvas );
				view->set_display_width_height( windowSize.width, windowSize.height );

				//__LOGT__( TAG__g_FuncSignalHandler, "on_event_draw_main(): w = %d, h = %d",
				//			windowSize.width, windowSize.height );


				view->_draw();


//#if 0
				// OpenGL
				{
					if ( gl_drawable ) {
						{
							const int channel = 4;
							//const int texture_size  = (channel * windowSize.width * windowSize.height);

							//if ( g_pCAtp3->m_texture_data ) {
							//	delete g_pCAtp3->m_texture_data;
							//	g_pCAtp3->m_texture_data = NULL;
							//}

							//g_pCAtp3->m_texture_data = new unsigned char[texture_size];
							//if ( g_pCAtp3->m_texture_data ) {
							{
								printf( "cairo_get_target()\n" );
								//cairo_surface_t* cr_surface = cairo_get_target( ((Widgets_st*)user_data)->pCanvas );

								if ( cr_surface ) {
									/*
									cairo_surface_t* surface = 
									cairo_image_surface_create_for_data( g_pCAtp3->m_texture_data,
																			CAIRO_FORMAT_ARGB32,
																			windowSize.width, windowSize.height,
																			(channel * windowSize.width) );
									*/
									g_pCAtp3->m_texture_data = cairo_image_surface_get_data( cr_surface );
									if ( g_pCAtp3->m_texture_data ) {
										//glBindTexture( GL_TEXTURE_RECTANGLE_ARB, g_pCAtp3->m_texture_id );
										glBindTexture( GL_TEXTURE_2D, g_pCAtp3->m_texture_id );
										glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
										glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
										//glTexImage2D( GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGBA,
										glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA,
														windowSize.width, windowSize.height, 0,
														GL_BGRA, GL_UNSIGNED_BYTE, g_pCAtp3->m_texture_data );

										/*
										glMatrixMode( GL_MODELVIEW );
										glLoadIdentity();
										glClear( GL_COLOR_BUFFER_BIT );
										glPushMatrix();

										glBindTexture( GL_TEXTURE_2D, g_pCAtp3->m_texture_id );
										glBegin( GL_QUADS );
										// ...
										glEnd();

										glPopMatrix();
										*/
									}
								}
							}
						}

						if ( gdk_gl_drawable_is_double_buffered(gl_drawable) ) {
							gdk_gl_drawable_swap_buffers( gl_drawable );
						}
						else {
							__LOGT__( TAG__g_FuncSignalHandler, "on_event_draw_main(): double-buffered [FALSE]" );
							glFlush();
						}
						//glXSwapBuffers( g_pCAtp3->x_display, g_pCAtp3->x_window );

						gdk_gl_drawable_gl_end( gl_drawable );
						__LOGT__( TAG__g_FuncSignalHandler, "on_event_draw_main(): updated" );
					}
					else {
						__LOGT__( TAG__g_FuncSignalHandler, "on_event_draw_main(): GdkGLDrawable == NULL" );
					}

					glXSwapBuffers( g_pCAtp3->x_display, g_pCAtp3->x_window );

					if ( g_pCAtp3->m_texture_data ) {
						delete g_pCAtp3->m_texture_data;
						g_pCAtp3->m_texture_data = NULL;
					}
				} // OpenGL
//#endif
			}
		}


		//cairo_destroy( cr );


		return false;
	}
#elif __GTKv3__
	//
	// v3.x "draw"
	bool on_event_draw_main(GtkWidget* widget, cairo_t* cr, gpointer user_data) {
		//__LOGT__( TAG__g_FuncSignalHandler, "on_event_draw_main()" );

		if ( !widget || !cr || !user_data ) return false;

		GtkAllocation windowSize;
		bool rgba = true;


		// Cairo
		{
			/*
			if ( ((Widgets_st*)user_data)->pCanvas ) {
				cairo_destroy( ((Widgets_st*)user_data)->pCanvas );
				((Widgets_st*)user_data)->pCanvas = NULL;
			}
			((Widgets_st*)user_data)->pCanvas = gdk_cairo_create( widget->window );

			cr = ((Widgets_st*)user_data)->pCanvas;
			if ( cr == NULL ) {
				return false;
			}
			*/

			if ( cr == NULL ) {
				return false;
			}
		}


		// RGBA
		{
			GdkScreen* screen = gtk_widget_get_screen( GTK_WIDGET(widget) );
			GdkVisual* visual = gdk_screen_get_rgba_visual( screen );

			if ( !visual ) {
				__LOGT__( TAG__g_FuncSignalHandler, "on_event_draw_main(): Support RGBA [FALSE]" );

				//visual = gdk_screen_get_system_visual( screen );
				rgba = false;
			}

			//gtk_widget_set_visual( widget, visual );
		}


		windowSize.width = 0;
		windowSize.height = 0;
		gtk_widget_get_allocation( widget, &windowSize );

		if ( g_pCAtp3 ) {
			CViewMain* view = g_pCAtp3->get_view_main();

			if ( view ) {
				view->set_top_view( widget );
				//view->set_top_view( ((Widgets_st*)user_data)->pDrawingArea );
				//view->set_top_view_ref( ((Widgets_st*)user_data)->pDrawingArea );
				view->set_rgba( rgba );
				view->set_canvas( cr );
				//view->set_canvas( ((Widgets_st*)user_data)->pCanvas );
				//view->set_canvas_ref( ((Widgets_st*)user_data)->pCanvas );
				view->set_display_width_height( windowSize.width, windowSize.height );

				//__LOGT__( TAG__g_FuncSignalHandler, "on_event_draw_main(): w = %d, h = %d",
				//			windowSize.width, windowSize.height );

				view->_draw();
			}
		}

		//cairo_destroy( cr );


		return false;

	}

	// ... Drawing View
	/*
	bool on_event_draw_...(GtkWidget* widget, cairo_t* cr, gpointer user_data) {
		//__LOGT__( TAG__g_FuncSignalHandler, "on_event_draw_...()" );

		if ( !widget || !cr || !user_data ) return;

		if ( g_pCAtp3 ) {
			CViewMain* view = g_pCAtp3->get_view_...();

			if ( view ) {
				view->set_canvas( event );
				view->invalidate();
			}
		}

		return false;
	}
	*/
#else
#endif
} // namespace g_FuncSignalHandler

// ---------------------------------------------------------------

