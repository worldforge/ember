
if BUILD_WEBEMBER
SUBDIRS = ../../main/bindings
lib_LTLIBRARIES = libWebEmber-0.1.la
include $(top_srcdir)/src/main/Makefile.am
INCLUDES = -I$(top_srcdir)/src  -I$(top_builddir)/src
libWebEmber_0_1_la_SOURCES = WebEmberManager.cpp WebEmberDLL.cpp $(embersources)
libWebEmber_0_1_la_LIBADD = $(emberlibs)
libWebEmber_0_1_la_LDFLAGS = -version 0:1:0
libWebEmberdir = $(includedir)


endif