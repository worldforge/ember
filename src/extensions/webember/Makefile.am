
if BUILD_WEBEMBER
SUBDIRS = ../../main/bindings
include $(top_srcdir)/src/main/Makefile.am
INCLUDES = -I$(top_srcdir)/src  -I$(top_builddir)/src

lib_LTLIBRARIES = libWebEmber.la

libWebEmber_la_SOURCES = WebEmberManager.cpp WebEmberDLL.cpp $(embersources)
libWebEmber_la_LIBADD = $(emberlibs)
if(WIN32)
libWebEmber_la_LDFLAGS = -lole32 -loleaut32 -no-undefined
endif
libWebEmber_la_LDFLAGS = -release 0.1 -version 0:1:0
libWebEmberdir = $(includedir)


endif