INCLUDES=-I$(top_srcdir)/src
bin_PROGRAMS = DimeOgre

DimeOgre_LDADD = \
$(top_builddir)/src/services/libServices.a \
$(top_builddir)/src/services/gui/libGuiService.a \
$(top_builddir)/src/services/gui/widget/libGuiServiceWidget.a \
$(top_builddir)/src/services/config/libConfigService.a \
$(top_builddir)/src/services/input/libInputService.a \
$(top_builddir)/src/services/platform/libPlatformService.a \
$(top_builddir)/src/services/image/libImageService.a \
$(top_builddir)/src/services/metaserver/libMetaserverService.a \
$(top_builddir)/src/services/font/libFontService.a \
$(top_builddir)/src/services/server/libServerService.a \
$(top_builddir)/src/services/sound/libSoundService.a \
$(top_builddir)/test/libTestServices.a \
$(top_builddir)/src/services/logging/libLoggingService.a \
$(top_builddir)/src/framework/libFramework.a 


DimeOgre_SOURCES = DimeOgre.cpp Console.cpp InputManager.cpp \
			Avatar.cpp PlayerMouseListener.cpp \
			EntityListener.cpp ConsoleObjectImpl.cpp \
			MediaDeployer.cpp DimeEventProcessor.cpp \
			AvatarController.cpp DebugListener.cpp \
			DimeEntity.cpp DimeEntityFactory.cpp \
			AvatarDimeEntity.cpp TerrainGenerator.cpp \
			DimeTerrainRenderable.cpp DimeTerrainSceneManager.cpp
			

EXTRA_DIST = plugins.cfg  resources.cfg  terrain.cfg

