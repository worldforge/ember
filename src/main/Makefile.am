INCLUDES = -I$(top_srcdir)/src  -I$(top_builddir)/src -DPREFIX=\"@prefix@\"

bin_PROGRAMS = ember.bin

ember_bin_SOURCES = Application.cpp ConfigBoundLogObserver.cpp Ember.cpp

ember_bin_LDADD = $(top_builddir)/src/components/ogre/libEmberOgre.a \
	$(top_builddir)/src/extensions/libExtensions.a $(top_builddir)/src/services/libServices.a $(top_builddir)/src/framework/libFramework.a \
	 $(top_builddir)/src/main/bindings/lua/liblua_Main.a  $(top_builddir)/src/components/entitymapping/libEntityMapping.a



noinst_HEADERS = Application.h ConfigBoundLogObserver.h
SUBDIRS = bindings

EXTRA_DIST=macosx/COPYING.SDL macosx/ReadMeDevLite.txt macosx/SDLMain.h macosx/SDLMain.m
