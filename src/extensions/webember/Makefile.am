
include $(top_srcdir)/src/main/Makefile.am

lib_LTLIBRARIES = libWebEmber.la

libWebEmber_la_SOURCES = WebEmberManager.cpp WebEmberDLL.cpp $(embersources)
libWebEmber_la_LIBADD = $(emberlibs)
libWebEmber_la_LDFLAGS = -release 0.1 -version 0:1:0
libWebEmberdir = $(includedir)

if WIN32
libWebEmber_la_LDFLAGS += -lole32 -loleaut32 -no-undefined
endif