GTK+ Examples
===============


Summary
----------
> The sources show you how to use a GtkDrawingArea,<br>
> make a custom View and how to use Cairo/Pango font glyph.


Environment
----------
> build all and tested on GNU/Linux X-Window

    GNU/Linux: Ubuntu 11.10_x86, 16.10_x64 (Unity 3D, 2D)
    G++ version: 4.6.1 (Ubuntu 11.10_x86), 6.2.0 (Ubuntu 16.10_x64)
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
     - _CViewSlideWindow.cpp             // Custom Animation test
     - _incl_slidewindow_anim.cxx        // Custom Animation test
     - incl_timer_handler.cxx            // Timer Handler test
	 -
	 - ui_gtk2.glade           // Glade UI(XML) for GTK+2.x
	 - ui_gtk3.glade           // Glade UI(XML) for GTK+3.x
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

> Rotation

![alt tag](https://github.com/godmode2k/gtk_examples/raw/master/screenshots/screenshot_10.png)

> Patchers IO

![alt tag](https://github.com/godmode2k/gtk_examples/raw/master/screenshots/screenshot_11.png)



## Donation
If this project help you reduce time to develop, you can give me a cup of coffee :)

(BitcoinCash) -> bitcoincash:qqls8jsln7w5vzd32g4yrwprstu57aa8rgf4yvsm3m <br>
(Bitcoin) -> 16kC7PUd75rvmwom4oftXRyg3gR9KTPb4m <br>
(Ethereum) -> 0x90B45D2CBBB0367D50590659845C486497F89cBB <br>


