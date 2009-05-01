INCLUDES = -I$(top_srcdir)/src  -I$(top_builddir)/src -DPREFIX=\"@prefix@\"

bin_PROGRAMS = ember.bin

ember_bin_SOURCES = Application.cpp ConfigBoundLogObserver.cpp Ember.cpp \
	SnapToMovement.cpp

ember_bin_LDADD = $(top_builddir)/src/components/ogre/libEmberOgre.la \
	$(top_builddir)/src/extensions/libExtensions.la $(top_builddir)/src/services/libServices.la $(top_builddir)/src/framework/libFramework.la \
	 $(top_builddir)/src/main/bindings/lua/liblua_Main.la



noinst_HEADERS = Application.h ConfigBoundLogObserver.h SnapToMovement.h
SUBDIRS = bindings

EXTRA_DIST=macosx/COPYING.SDL macosx/ReadMeDevLite.txt macosx/SDLMain.h macosx/SDLMain.m
