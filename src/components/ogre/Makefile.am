INCLUDES=-I$(top_srcdir)/src
bin_PROGRAMS = EmberOgre

EmberOgre_LDADD = \
$(top_builddir)/src/services/libServices.a \
$(top_builddir)/src/services/config/libConfigService.a \
$(top_builddir)/src/services/metaserver/libMetaserverService.a \
$(top_builddir)/src/services/server/libServerService.a \
$(top_builddir)/src/services/sound/libSoundService.a \
$(top_builddir)/src/services/logging/libLoggingService.a \
$(top_builddir)/src/framework/libFramework.a 


EmberOgre_SOURCES = DimeOgre.cpp InputManager.cpp \
			Avatar.cpp \
			EntityListener.cpp ConsoleObjectImpl.cpp \
			MediaDeployer.cpp DimeEventProcessor.cpp \
			AvatarController.cpp DebugListener.cpp \
			DimeEntity.cpp DimeEntityFactory.cpp \
			DimeTerrainPageSource.cpp environment/Water.cpp \
			TerrainGenerator.cpp AvatarDimeEntity.cpp \
			MotionManager.cpp AvatarCamera.cpp \
			DimeTerrainSceneManager.cpp TerrainShader.cpp \
			GUIManager.cpp MousePicker.cpp WorldDimeEntity.cpp \
			DimePhysicalEntity.cpp Model.cpp \
			SubModel.cpp SubModelPart.cpp \
			PersonDimeEntity.cpp GroundCover.cpp \
			widgets/Widget.cpp widgets/ConsoleWidget.cpp \
			widgets/DebugWidget.cpp widgets/IngameEventsWidget.cpp widgets/ChatWidget.cpp \
			widgets/EntityPickerWidget.cpp widgets/InventoryWidget.cpp widgets/ServerBrowserWidget.cpp \
			widgets/InspectWidget.cpp widgets/MakeEntityWidget.cpp widgets/ServerWidget.cpp \
			widgets/GiveWidget.cpp \
			DimeOgre.h InputManager.h\
                        Avatar.h MathConverter.h  DimeOgrePrerequisites.h \
			EntityListener.h ConsoleObjectImpl.h \
			MediaDeployer.h DimeEventProcessor.h \
			AvatarController.h DebugListener.h \
			DimeEntity.h DimeEntityFactory.h \
			DimeTerrainPageSource.h environment/Water.h \
			TerrainGenerator.h AvatarDimeEntity.h \
			MotionManager.h AvatarCamera.h \
			DimeTerrainSceneManager.h TerrainShader.h \
			GUIManager.h MousePicker.h WorldDimeEntity.h \
			DimePhysicalEntity.h Model.h \
			SubModel.h SubModelPart.h \
			PersonDimeEntity.h GroundCover.h \
			widgets/Widget.h widgets/ConsoleWidget.h \
			widgets/DebugWidget.h widgets/IngameEventsWidget.h widgets/ChatWidget.h \
			widgets/EntityPickerWidget.h widgets/InventoryWidget.h widgets/ServerBrowserWidget.h \
			widgets/InspectWidget.h widgets/MakeEntityWidget.h widgets/ServerWidget.h \
			widgets/GiveWidget.h
																																																						
EXTRA_DIST = plugins.cfg  resources.cfg  terrain.cfg

confdir = $(datadir)/ember/data/
dist_conf_DATA = ogre.cfg plugins.cfg  resources.cfg  terrain.cfg


