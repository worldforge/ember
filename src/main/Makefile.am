INCLUDES = -I$(top_srcdir)/src

bin_PROGRAMS = dime

dime_LDADD = $(top_srcdir)/src/services/gui/libGuiService.a \
$(top_srcdir)/src/services/config/libConfigService.a \
$(top_srcdir)/src/services/input/libInputService.a \
$(top_srcdir)/src/services/platform/libPlatformService.a \
$(top_srcdir)/src/services/image/libImageService.a \
$(top_srcdir)/src/services/metaserver/libMetaserverService.a \
$(top_srcdir)/src/services/font/libFontService.a \
$(top_srcdir)/src/services/server/libServerService.a \
$(top_srcdir)/src/services/sound/libSoundService.a \
$(top_srcdir)/test/libTestServices.a \
$(top_srcdir)/src/services/datamodel/libDataModelService.a \
$(top_srcdir)/src/services/logging/libLoggingService.a

dime_SOURCES = Application.cpp Dime.cpp DimeServices.cpp StateManager.cpp

noinst_HEADERS = Application.h DimeServices.h StateManager.h
