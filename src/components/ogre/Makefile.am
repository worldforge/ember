INCLUDES=-I$(top_srcdir)/src
bin_PROGRAMS = DimeOgre

DimeOgre_LDADD = \
$(top_srcdir)/src/services/libServices.a \
$(top_srcdir)/src/services/gui/libGuiService.a \
$(top_srcdir)/src/services/gui/widget/libGuiServiceWidget.a \
$(top_srcdir)/src/services/config/libConfigService.a \
$(top_srcdir)/src/services/input/libInputService.a \
$(top_srcdir)/src/services/platform/libPlatformService.a \
$(top_srcdir)/src/services/image/libImageService.a \
$(top_srcdir)/src/services/metaserver/libMetaserverService.a \
$(top_srcdir)/src/services/font/libFontService.a \
$(top_srcdir)/src/services/server/libServerService.a \
$(top_srcdir)/src/services/sound/libSoundService.a \
$(top_srcdir)/test/libTestServices.a \
$(top_srcdir)/src/services/logging/libLoggingService.a \
$(top_srcdir)/src/framework/libFramework.a 


DimeOgre_SOURCES = DimeOgre.cpp Console.cpp InputManager.cpp PlayerMouseListener.cpp \
		   EntityListener.cpp ConsoleObjectImpl.cpp \
		   MediaDeployer.cpp

EXTRA_DIST = plugins.cfg  resources.cfg  terrain.cfg

