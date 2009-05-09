INCLUDES = -I$(top_srcdir)/src -I$(top_builddir)/src -DPREFIX=\"@prefix@\" -DSYSCONFDIR=\"$(sysconfdir)\" -DDATADIR=\"$(datadir)\"

noinst_LTLIBRARIES = libConfigService.la

libConfigService_la_SOURCES = ConfigService.cpp ConfigListener.cpp ConfigListenerContainer.cpp 
noinst_HEADERS = ConfigService.h ConfigListener.h ConfigListenerContainer.h
