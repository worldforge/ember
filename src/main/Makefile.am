INCLUDES = -I$(top_srcdir)/src
bin_PROGRAMS = dime

dime_LDADD = $(top_srcdir)/src/services/gui/libGuiService.a $(top_srcdir)/src/services/input/libInputService.a $(top_srcdir)/src/services/platform/libPlatformService.a $(top_srcdir)/src/services/image/libImageService.a $(top_srcdir)/src/services/metaserver/libMetaserverService.a $(top_srcdir)/src/services/font/libFontService.a
dime_SOURCES = Application.cpp Dime.cpp DimeServices.cpp Application.h \
	DimeServices.h

