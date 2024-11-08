GTK+ Examples
===============


Summary
----------
> The sources show you how to use a GtkDrawingArea,<br>
> make a custom View and how to use Cairo/Pango font glyph.


Environment
----------
> build all and tested on GNU/Linux X-Window

    GNU/Linux: Ubuntu 20.04 x64 (Unity 3D, 2D)
    GCC(G++) version: 9.4.0
    GTK+ version: 2.x, 3.x
    GDK pixbuf: 2.x
    Cairo version: 2.x
    Pango version: 1.x


Build-tools
----------
```sh
$ sudo apt-get install build-essential
$ sudo apt-get install libgtk2.0-0 libgtk2.0-dev libgtk2.0-doc libgtk-3-0 libgtk-3-dev libgtk-3-doc
$ sudo apt-get install libgdk-pixbuf2.0-0 libgdk-pixbuf2.0-dev libpango-1.0-0 libpango1.0-dev libpangocairo-1.0-0
$ sudo apt-get install libcairo2 libcairo2-dev
$ sudo apt-get install glade libglade2-0 libglade2-dev xxd
```


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
     - _CViewSlideWindow.cpp             // Custom Animation test
     - _incl_slidewindow_anim.cxx        // Custom Animation test
     - incl_timer_handler.cxx            // Timer Handler test
	 -
	 - ui_gtk2.glade           // Glade UI(XML) for GTK+2.x
	 - ui_gtk3.glade           // Glade UI(XML) for GTK+3.x
	 - ui_gtk3_new.glade       // Glade UI(XML) for GTK+3.20
	 - ui_glade.h              // Glade UI(XML) to C-Style include file, AUTO-GENERATED at build


	./libs/util
	  - CMutexSemaphore.h      // Mutex, Semaphore Class (Abstract)
	  - CNetSockLib.cpp        // Network Class
	  -
	  - CDialog.cpp            // Gtk+ Widget Dialog Class (Custom)
	  - CThreadTask.cpp        // POSIX Threads (Pthreads) Class, like AsyncTask method in Android
	  - CProgressDlg.cpp       // Progress Dialog Class
	  - CToastMsgDlg.cpp       // Message Dialog Class, like Toast method in Android
      - CAnimTask.cpp          // Custom Animation Class
      - CTimerHandlerTask.cpp  // Timer Handler Class
	  -
	  - CUtil.cpp              // Useful system functions (utility)


    // GTK+ v2.x
    $ sh build.sh atp3 atp3.cpp v2

    // GTK+ v3.x
    $ sh build.sh atp3 atp3.cpp v3

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

> Rotation

![alt tag](https://github.com/godmode2k/gtk_examples/raw/master/screenshots/screenshot_10.png)

> Patchers IO

![alt tag](https://github.com/godmode2k/gtk_examples/raw/master/screenshots/screenshot_11.png)


