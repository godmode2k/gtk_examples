GTK+ Examples
===============


Summary
----------
> The sources show you how to use a GtkDrawingArea,<br>
> make a custom View and how to use Cairo/Pango font glyph.


Environment
----------
> build all and tested on GNU/Linux X-Window

    GNU/Linux: Ubuntu 11.10 (Unity 3D, 2D)
    GTK+ version: 2.x, 3.x
    Cairo version: 1.xx
    Pango version: 1.0


Build
----------
> as following or you should make whatever you want

    ./atp3/
     - build.sh                // for build all
	 -
     - atp3.cpp                // Main Class
	 -
	 - CBaseView.cpp           // Abstract Base View Class
	 - CKeyEvent.cpp           // Key, Mouse Event Base Class
	 -
	 - CViewMain.cpp           // Main View Class
	 - CViewAttach.cpp         // Attachment (Text, Image) Class
	 -
	 - incl_g_func.cxx                   // Global functions
	 - incl_g_func_signal_handler.cxx    // Gtk+ Widget Signal Handler
	 - incl_progress_dlg.cxx             // Progress Dialog Implements
	 - incl_toastmsg_dlg.cxx             // Toast Message Dialog Implements
	 -
	 - ui_gtk2.glade           // Glade UI(XML) for GTK+2.x
	 - ui_gtk3.glade           // Glade UI(XML) for GTK+3.x
	 - ui_glade.h              // Glade UI(XML) to C-Style include file, AUTO-GENERATED at build


	./libs/util
	  - CMutexSemaphore.h      // Mutex, Semaphore Class (Abstract)
	  - CNetSockLib.cpp        // Network Class
	  -
	  - CDialog.cpp            // Gtk+ Widget Dialog Class (Custom)
	  - CThreadTask.cpp        // PThread Class, 
	  - CProgressDlg.cpp       // Progress Dialog Class
	  - CToastMsgDlg.cpp       // Message Dialog Class, like Toast method in Android
	  -
	  - CUtil.cpp              // Useful system functions (utility)


    $ sh build.sh atp3 atp3.cpp
    $ ./atp3


Screenshots
----------

> Image & Text

![alt tag](https://github.com/godmode2k/gtk_examples/raw/master/screenshots/screenshot_01.png)

> Popup menu

![alt tag](https://github.com/godmode2k/gtk_examples/raw/master/screenshots/screenshot_02.png)

> Text Dialog

![alt tag](https://github.com/godmode2k/gtk_examples/raw/master/screenshots/screenshot_03.png)

> Text Dialog: Font

![alt tag](https://github.com/godmode2k/gtk_examples/raw/master/screenshots/screenshot_04.png)

> Text Dialog: Color

![alt tag](https://github.com/godmode2k/gtk_examples/raw/master/screenshots/screenshot_05.png)

> Toast Message Dialog (Ubuntu Unity 2D, No Alpha effect)

![alt tag](https://github.com/godmode2k/gtk_examples/raw/master/screenshots/screenshot_06.png)

> Toast Message Dialog (Ubuntu Unity 3D, Alpha effect applied)

![alt tag](https://github.com/godmode2k/gtk_examples/raw/master/screenshots/screenshot_07.png)

> Progress Dialog

![alt tag](https://github.com/godmode2k/gtk_examples/raw/master/screenshots/screenshot_08.png)

> Capture region

![alt tag](https://github.com/godmode2k/gtk_examples/raw/master/screenshots/screenshot_09.png)


