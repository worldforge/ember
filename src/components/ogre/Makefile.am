INCLUDES=-I$(top_srcdir)/src
bin_PROGRAMS = DimeOgre Cal3D2Ogre

DimeOgre_LDADD = $(top_srcdir)/src/services/gui/libGuiService.a \
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
$(top_srcdir)/src/services/datamodel/libDataModelService.a \
$(top_srcdir)/src/services/logging/libLoggingService.a \
$(top_srcdir)/src/framework/libFramework.a \
$(top_srcdir)/src/services/libServices.a


DimeOgre_SOURCES = DimeOgre.cpp
Cal3D2Ogre_SOURCES = Cal3DOgreConverter.cpp

Cal3D2Ogre_LDADD = -lcal3d

EXTRA_DIST = plugins.cfg  resources.cfg  terrain.cfg

