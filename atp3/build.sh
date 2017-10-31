#!/bin/sh

#
# ATP3 Project Build Script
#
# Author: Ho-Jung Kim (godmode2k@hotmail.com)
# Date: Since June 29, 2012
# Last modified: May 28, 2015
#



# Binary and build opt. for glade
#
BIN_CC=/usr/bin/g++
#
#
# USE GTK+ 2.x
##gmodule-2.0
OPT_GTK2="-D__GTKv2__"
#OPT_LIB="`pkg-config --cflags --libs gtk+-2.0 libglade-2.0 --libs gthread-2.0` $OPT_GTK2"
OPT_LIB="`pkg-config --cflags --libs gtk+-2.0 libglade-2.0 --libs` $OPT_GTK2"
#
# USE GTK+ 3.x
#OPT_GTK3="-D__GTKv3__"
#OPT_LIB="`pkg-config --cflags --libs gtk+-3.0 libglade-2.0` $OPT_GTK3"
##GTKFLAGS="-Wl,-export-dynamic"
#GTKFLAGS="-DGTK_DISABLE_SINGLE_INCLUDES"
#GTKFLAGS="-DGDK_DISABLE_DEPRECATED -DGTK_DISABLE_DEPRECATED"
#
#
# OpenGL
OPT_USE_OPENGL="false"
#
#
# Glade UI(XML) to C-Style include file
BIN_XXD=/usr/bin/xxd
#
echo "-------------------------"
if [ -z "$OPT_GTK2" ]; then
	echo '=== [GTK+ VERSION 3] ==='
	XXD_GLADE_FILE="ui_gtk3.glade"
else
	echo '=== [GTK+ VERSION 2] ==='
	XXD_GLADE_FILE="ui_gtk2.glade"
fi
echo "-------------------------"
#
#XXD_GLADE_FILE="ui.glade"
XXD_GLADE_OUTPUT="ui_glade.h"
XXD_OPTS="-i $XXD_GLADE_FILE $XXD_GLADE_OUTPUT"
XXD_CMD="$BIN_XXD $XXD_OPTS"
BIN_SED=/bin/sed
SED_CMD_ADD_MESSAGE="// AUTO-GENERATED at build that C-style include file from ui.glade\n//"
SED_CMD_ADD_OPTS="-i '1i $SED_CMD_ADD_MESSAGE'"
SED_CMD_ADD_COMMENT="$BIN_SED $SED_CMD_ADD_OPTS $XXD_GLADE_OUTPUT"


# Include
#

# Library, Dependencies
#
SRC_IN_LIB_CUTIL_PATH="../libs/util"
SRC_IN_LIB="$SRC_IN_LIB_CUTIL_PATH/CUtil.cpp	\
	$SRC_IN_LIB_CUTIL_PATH/CThreadTask.cpp	\
	$SRC_IN_LIB_CUTIL_PATH/CDialog.cpp		\
	$SRC_IN_LIB_CUTIL_PATH/CProgressDlg.cpp	\
	$SRC_IN_LIB_CUTIL_PATH/CToastMsgDlg.cpp	\
	$SRC_IN_LIB_CUTIL_PATH/CAnimTask.cpp	\
	$SRC_IN_LIB_CUTIL_PATH/CTimerHandlerTask.cpp	\
	./CKeyEvent.cpp	\
	./CBaseView.cpp	\
	./CViewMain.cpp	\
	./CViewAttach.cpp	\
	./_CViewSlideWindow.cpp	\
"
SRC_IN_LIB_PATH_ALL="-I../libs	\
"
OPT_LIBS_PTHREAD="-D_REENTRANT -lpthread"
OPT_LIBS_TIME="-lrt"
OPT_LIBS_CUTTER="" #"-lcutter"
OPT_LIBS="$OPT_LIBS_PTHREAD	\
	$OPT_LIBS_TIME		\
	$OPT_LIBS_CUTTER	\
	-lX11
"
#
#
# OpenGL
#OPT_LIB_GL="-lGL -lXxf86vm -lXext -lX11"
#OPT_LIB_GL="-lGL -lXext -lX11"
echo "-------------------------"
if [ "$OPT_USE_OPENGL" = "true" ]; then
	echo '=== [OpenGL] ==='
	echo 'Use [TRUE]'
	OPT_LIB_GL="-lGL -lXext -lX11"
	OPT_LIB_GTKGL="`pkg-config gtkglext-1.0 --cflags --libs`"
	#OPT_LIB_GL_FLAGS="-O3 -ffast-math"
else
	echo '=== [OpenGL] ==='
	echo 'Use [FALSE]'
	OPT_LIB_GL=
	OPT_LIB_GTKGL=
	#OPT_LIB_GL_FLAGS=
fi
echo "-------------------------"
#
#
# C++11
#OPT_CPP11="-std=gnu++0x"
OPT_CPP11="-std=c++0x"
#OPT_CPP11="-std=c++11"
OPT_LIBS_ALL="-D__LINUX__	\
	-D__DEBUG_MODE__ \
	$SRC_IN_LIB_PATH_ALL $OPT_LIBS	\
	$OPT_LIB_GL $OPT_LIB_GTKGL		\
	$OPT_CPP11	\
"


# Final OPT
#
OPT_DEBUG="-g"
OPT_DEFAULT="-Wall -O2 $GTKFLAGS"
OPT_INC="-I./"
OPT_LIB_ALL="$OPT_LIB $OPT_LIBS_ALL"
# library
OPT_SRC_IN_LIB="$SRC_IN_LIB"

OUTPUT=$1
INPUT=$2


echo -ne '
if [ -z "$1" ]; then
	echo 'Usage: $0 [filename] [option]'
	echo ''
	echo '-g	build as debug mode'
	exit
fi

case "$2" in
	-g)
		OPT_DEBUG="-g"
esac
' >> /dev/null

if [ -z "$1" ] || [ -z "$2" ]; then
	echo $0
	echo "Usage: sh $0 {output} {input}"
	echo ""
	exit
fi


# RUN
#
echo "-------------------------"
echo "ATP3 Project Build Script"
echo "-------------------------"
echo "[+] generate a C-style include file from ui.glade"
echo "$XXD_CMD"
$XXD_CMD
echo "$SED_CMD_ADD_COMMENT"
eval $SED_CMD_ADD_COMMENT
echo ""
echo "[+] build... [START]"
echo "$BIN_CC -o $OUTPUT $INPUT $OPT_SRC_IN_LIB $OPT_DEFAULT $OPT_DEBUG $OPT_INC $OPT_LIB_ALL"
$BIN_CC -o $OUTPUT $INPUT $OPT_SRC_IN_LIB $OPT_DEFAULT $OPT_DEBUG $OPT_INC $OPT_LIB_ALL

#echo ""
# Checks the last command return value
if [ $? -ne 0 ]; then
	echo ""
	echo "[-] build... [FINISHED] [FAIL]"
else
	echo ""
	echo "[+] build... [FINISHED] [SUCCESS]"
fi

# EOF
