INCLUDES=-I$(top_srcdir)/src
bin_PROGRAMS = DimeOgre

DimeOgre_LDADD = \
$(top_builddir)/src/services/libServices.a \
$(top_builddir)/src/services/config/libConfigService.a \
$(top_builddir)/src/services/input/libInputService.a \
$(top_builddir)/src/services/image/libImageService.a \
$(top_builddir)/src/services/metaserver/libMetaserverService.a \
$(top_builddir)/src/services/font/libFontService.a \
$(top_builddir)/src/services/server/libServerService.a \
$(top_builddir)/src/services/sound/libSoundService.a \
$(top_builddir)/src/services/logging/libLoggingService.a \
$(top_builddir)/src/framework/libFramework.a 


DimeOgre_SOURCES = DimeOgre.cpp Console.cpp InputManager.cpp \
			Avatar.cpp \
			EntityListener.cpp ConsoleObjectImpl.cpp \
			MediaDeployer.cpp DimeEventProcessor.cpp \
			AvatarController.cpp DebugListener.cpp \
			DimeEntity.cpp DimeEntityFactory.cpp \
			DimeTerrainPageSource.cpp \
			TerrainGenerator.cpp AvatarDimeEntity.cpp \
			MotionManager.cpp AvatarCamera.cpp \
			DimeTerrainSceneManager.cpp TerrainShader.cpp \
			GUIManager.cpp MousePicker.cpp WorldDimeEntity.cpp \
			DimePhysicalEntity.cpp Model.cpp \
			SubModel.cpp SubModelPart.cpp \
			PersonDimeEntity.cpp GroundCover.cpp \
			widgets/Widget.cpp widgets/ConsoleWidget.cpp \
			widgets/DebugWidget.cpp widgets/IngameEventsWidget.cpp widgets/ChatWidget.cpp 

EXTRA_DIST = plugins.cfg  resources.cfg  terrain.cfg

