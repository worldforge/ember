AM_CPPFLAGS = -I$(top_srcdir)/src  -I$(top_builddir)/src -DPREFIX=\"@prefix@\"

if BUILD_WEBEMBER

embersources = ../../main/Application.cpp ../../main/ConfigBoundLogObserver.cpp

if WIN32
embersources += ../../main/win32/OgreWin32Resources.rc ../../main/win32/ember.ico
.rc.o:
	$(WINDRES) -o $@ -i $<
endif

lib_LTLIBRARIES = libWebEmber.la

libWebEmber_la_CXXFLAGS = $(AM_CXXFLAGS)
libWebEmber_la_SOURCES = WebEmberManager.cpp WebEmberDLL.cpp $(embersources)
libWebEmber_la_LIBADD = $(emberlibs)
libWebEmber_la_LDFLAGS = -release 0.1 -version 0:1:0
libWebEmberdir = $(includedir)

if WIN32
libWebEmber_la_LDFLAGS += -lole32 -loleaut32 -no-undefined
endif

endif


