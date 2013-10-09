AM_CPPFLAGS = -I$(top_srcdir)/src  -I$(top_builddir)/src -DPREFIX=\"@prefix@\"

SUBDIRS=bindings

noinst_LIBRARIES = libDomain.a

libDomain_a_SOURCES = EntityTalk.cpp

noinst_HEADERS = EntityTalk.h Types.h IHeightProvider.h
