INCLUDES = -I$(top_srcdir)/src -I$(top_builddir)/src -DPREFIX=\"@prefix@\" -DSYSCONFDIR=\"$(sysconfdir)\" -DDATADIR=\"$(datadir)\"

noinst_LIBRARIES = libConfigService.a

libConfigService_a_SOURCES = ConfigService.cpp ConfigListener.cpp ConfigListenerContainer.cpp 
noinst_HEADERS = ConfigService.h ConfigListener.h ConfigListenerContainer.h
