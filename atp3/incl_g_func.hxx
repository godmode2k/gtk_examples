#ifndef __INCL_G_FUNC_HXX__
#define __INCL_G_FUNC_HXX__

/* --------------------------------------------------------------
Project:	ATP(A Tentative Plan) 3
Purpose:	
Author:		Ho-Jung Kim (godmode2k@hotmail.com)
Date:		Since Dec 2, 2014
Filename:	incl_g_func.hxx

Last modified: Jan 21, 2015
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
// "incl_g_func.cxx"
namespace g_Func {
	#define TAG__g_Func		"g_Func"

	// Gtk Window Quit
	void main_window_quit(GtkWidget* widget, gpointer user_data);

	// File Open Dialog
	//  - MUST release the return value with g_free() or free() or 'delete'
	//	- devolve release to caller
	char* file_open_dialog(GtkWidget* widget, gpointer user_data);

	// Input Text Dialog
	//  - MUST release the return value with 'delete'
	//	- devolve release to caller
	char* simple_text_input_dialog(GtkWidget* widget, gpointer user_data,
			const char* title = NULL, const char* desc = NULL, const char* old_text = NULL);
} // namespace g_Func
// ---------------------------------------------------------------



//! Class
// ---------------------------------------------------------------
// ---------------------------------------------------------------



#endif	// __INCL_G_FUNC_HXX__
