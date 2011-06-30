
if BUILD_WEBEMBER
lib_LTLIBRARIES = libWebEmber-0.1.la

INCLUDES = -I$(top_srcdir)/src
libWebEmber_0_1_la_SOURCES = WebEmberManager.cpp $(ember_bin_SOURCES)
libWebEmber_0_1_la_LIBADD = $(ember_bin_LDADD)
libWebEmber_0_1_la_LDFLAGS = -version 0:1:0
libWebEmberdir = $(includedir)
libWebEmber_HEADERS = WebEmberManager.h
endif