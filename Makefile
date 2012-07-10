#############################################################################
# Makefile for building: sql
# Generated by qmake (2.01a) (Qt 4.8.1) on: Tue Jul 10 18:39:06 2012
# Project:  sql.pro
# Template: app
# Command: /usr/bin/qmake -spec /usr/share/qt4/mkspecs/linux-g++-32 -o Makefile sql.pro
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -DQT_WEBKIT -DQT_NO_DEBUG -DQT_SQL_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED
CFLAGS        = -m32 -pipe -O2 -Wall -W -D_REENTRANT $(DEFINES)
CXXFLAGS      = -m32 -pipe -O2 -Wall -W -D_REENTRANT $(DEFINES)
INCPATH       = -I/usr/share/qt4/mkspecs/linux-g++-32 -I. -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtSql -I/usr/include/qt4 -I. -I.
LINK          = g++
LFLAGS        = -m32 -Wl,-O1
LIBS          = $(SUBLIBS)  -L/usr/lib/i386-linux-gnu -lQtSql -lQtGui -lQtCore -lpthread 
AR            = ar cqs
RANLIB        = 
QMAKE         = /usr/bin/qmake
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
STRIP         = strip
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = ./

####### Files

SOURCES       = searcher.cpp \
		filters.cpp \
		helpers.cpp \
		switcher.cpp \
		catalog.cpp \
		constants.cpp \
		table.cpp \
		mainwindow.cpp \
		main.cpp moc_searcher.cpp \
		moc_switcher.cpp \
		moc_catalog.cpp \
		moc_table.cpp \
		moc_mainwindow.cpp
OBJECTS       = searcher.o \
		filters.o \
		helpers.o \
		switcher.o \
		catalog.o \
		constants.o \
		table.o \
		mainwindow.o \
		main.o \
		moc_searcher.o \
		moc_switcher.o \
		moc_catalog.o \
		moc_table.o \
		moc_mainwindow.o
DIST          = /usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/common/gcc-base.conf \
		/usr/share/qt4/mkspecs/common/gcc-base-unix.conf \
		/usr/share/qt4/mkspecs/common/g++-base.conf \
		/usr/share/qt4/mkspecs/common/g++-unix.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/modules/qt_webkit_version.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/release.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/unix/gdb_dwarf_index.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
		sql.pro
QMAKE_TARGET  = sql
DESTDIR       = 
TARGET        = sql

first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: Makefile $(TARGET)

$(TARGET):  $(OBJECTS)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

Makefile: sql.pro  /usr/share/qt4/mkspecs/linux-g++-32/qmake.conf /usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/common/gcc-base.conf \
		/usr/share/qt4/mkspecs/common/gcc-base-unix.conf \
		/usr/share/qt4/mkspecs/common/g++-base.conf \
		/usr/share/qt4/mkspecs/common/g++-unix.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/modules/qt_webkit_version.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/release.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/unix/gdb_dwarf_index.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
		/usr/lib/i386-linux-gnu/libQtSql.prl \
		/usr/lib/i386-linux-gnu/libQtGui.prl \
		/usr/lib/i386-linux-gnu/libQtCore.prl
	$(QMAKE) -spec /usr/share/qt4/mkspecs/linux-g++-32 -o Makefile sql.pro
/usr/share/qt4/mkspecs/common/unix.conf:
/usr/share/qt4/mkspecs/common/linux.conf:
/usr/share/qt4/mkspecs/common/gcc-base.conf:
/usr/share/qt4/mkspecs/common/gcc-base-unix.conf:
/usr/share/qt4/mkspecs/common/g++-base.conf:
/usr/share/qt4/mkspecs/common/g++-unix.conf:
/usr/share/qt4/mkspecs/qconfig.pri:
/usr/share/qt4/mkspecs/modules/qt_webkit_version.pri:
/usr/share/qt4/mkspecs/features/qt_functions.prf:
/usr/share/qt4/mkspecs/features/qt_config.prf:
/usr/share/qt4/mkspecs/features/exclusive_builds.prf:
/usr/share/qt4/mkspecs/features/default_pre.prf:
/usr/share/qt4/mkspecs/features/release.prf:
/usr/share/qt4/mkspecs/features/default_post.prf:
/usr/share/qt4/mkspecs/features/unix/gdb_dwarf_index.prf:
/usr/share/qt4/mkspecs/features/warn_on.prf:
/usr/share/qt4/mkspecs/features/qt.prf:
/usr/share/qt4/mkspecs/features/unix/thread.prf:
/usr/share/qt4/mkspecs/features/moc.prf:
/usr/share/qt4/mkspecs/features/resources.prf:
/usr/share/qt4/mkspecs/features/uic.prf:
/usr/share/qt4/mkspecs/features/yacc.prf:
/usr/share/qt4/mkspecs/features/lex.prf:
/usr/share/qt4/mkspecs/features/include_source_dir.prf:
/usr/lib/i386-linux-gnu/libQtSql.prl:
/usr/lib/i386-linux-gnu/libQtGui.prl:
/usr/lib/i386-linux-gnu/libQtCore.prl:
qmake:  FORCE
	@$(QMAKE) -spec /usr/share/qt4/mkspecs/linux-g++-32 -o Makefile sql.pro

dist: 
	@$(CHK_DIR_EXISTS) .tmp/sql1.0.0 || $(MKDIR) .tmp/sql1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) .tmp/sql1.0.0/ && $(COPY_FILE) --parents searcher.h filters.h constants.h helpers.h switcher.h catalog.h table.h mainwindow.h .tmp/sql1.0.0/ && $(COPY_FILE) --parents searcher.cpp filters.cpp helpers.cpp switcher.cpp catalog.cpp constants.cpp table.cpp mainwindow.cpp main.cpp .tmp/sql1.0.0/ && (cd `dirname .tmp/sql1.0.0` && $(TAR) sql1.0.0.tar sql1.0.0 && $(COMPRESS) sql1.0.0.tar) && $(MOVE) `dirname .tmp/sql1.0.0`/sql1.0.0.tar.gz . && $(DEL_FILE) -r .tmp/sql1.0.0


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) Makefile


check: first

mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

compiler_moc_header_make_all: moc_searcher.cpp moc_switcher.cpp moc_catalog.cpp moc_table.cpp moc_mainwindow.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) moc_searcher.cpp moc_switcher.cpp moc_catalog.cpp moc_table.cpp moc_mainwindow.cpp
moc_searcher.cpp: filters.h \
		helpers.h \
		constants.h \
		switcher.h \
		catalog.h \
		table.h \
		searcher.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) searcher.h -o moc_searcher.cpp

moc_switcher.cpp: constants.h \
		switcher.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) switcher.h -o moc_switcher.cpp

moc_catalog.cpp: helpers.h \
		constants.h \
		catalog.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) catalog.h -o moc_catalog.cpp

moc_table.cpp: helpers.h \
		constants.h \
		switcher.h \
		table.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) table.h -o moc_table.cpp

moc_mainwindow.cpp: searcher.h \
		filters.h \
		helpers.h \
		constants.h \
		switcher.h \
		catalog.h \
		table.h \
		mainwindow.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) mainwindow.h -o moc_mainwindow.cpp

compiler_rcc_make_all:
compiler_rcc_clean:
compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_uic_make_all:
compiler_uic_clean:
compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_moc_header_clean 

####### Compile

searcher.o: searcher.cpp searcher.h \
		filters.h \
		helpers.h \
		constants.h \
		switcher.h \
		catalog.h \
		table.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o searcher.o searcher.cpp

filters.o: filters.cpp filters.h \
		helpers.h \
		constants.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o filters.o filters.cpp

helpers.o: helpers.cpp helpers.h \
		constants.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o helpers.o helpers.cpp

switcher.o: switcher.cpp switcher.h \
		constants.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o switcher.o switcher.cpp

catalog.o: catalog.cpp catalog.h \
		helpers.h \
		constants.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o catalog.o catalog.cpp

constants.o: constants.cpp constants.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o constants.o constants.cpp

table.o: table.cpp table.h \
		helpers.h \
		constants.h \
		switcher.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o table.o table.cpp

mainwindow.o: mainwindow.cpp mainwindow.h \
		searcher.h \
		filters.h \
		helpers.h \
		constants.h \
		switcher.h \
		catalog.h \
		table.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o mainwindow.o mainwindow.cpp

main.o: main.cpp searcher.h \
		filters.h \
		helpers.h \
		constants.h \
		switcher.h \
		catalog.h \
		table.h \
		mainwindow.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o main.o main.cpp

moc_searcher.o: moc_searcher.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_searcher.o moc_searcher.cpp

moc_switcher.o: moc_switcher.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_switcher.o moc_switcher.cpp

moc_catalog.o: moc_catalog.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_catalog.o moc_catalog.cpp

moc_table.o: moc_table.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_table.o moc_table.cpp

moc_mainwindow.o: moc_mainwindow.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_mainwindow.o moc_mainwindow.cpp

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:

