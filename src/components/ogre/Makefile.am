SUBDIRS =  environment model


INCLUDES=-I$(top_srcdir)/src
bin_PROGRAMS = EmberOgre

EmberOgre_LDADD = \
$(top_builddir)/src/services/libServices.a \
$(top_builddir)/src/services/config/libConfigService.a \
$(top_builddir)/src/services/metaserver/libMetaserverService.a \
$(top_builddir)/src/services/server/libServerService.a \
$(top_builddir)/src/services/sound/libSoundService.a \
$(top_builddir)/src/services/logging/libLoggingService.a \
$(top_builddir)/src/framework/libFramework.a \
$(top_builddir)/src/components/ogre/environment/libEnvironment.a \
$(top_builddir)/src/components/ogre/model/libModel.a


EmberOgre_SOURCES = InputManager.cpp Avatar.cpp EntityListener.cpp ConsoleObjectImpl.cpp MediaDeployer.cpp AvatarController.cpp DebugListener.cpp environment/Water.cpp TerrainGenerator.cpp MotionManager.cpp AvatarCamera.cpp TerrainShader.cpp GUIManager.cpp MousePicker.cpp  widgets/Widget.cpp widgets/ConsoleWidget.cpp widgets/DebugWidget.cpp widgets/IngameEventsWidget.cpp widgets/ChatWidget.cpp widgets/EntityPickerWidget.cpp widgets/InventoryWidget.cpp widgets/ServerBrowserWidget.cpp widgets/InspectWidget.cpp widgets/MakeEntityWidget.cpp widgets/ServerWidget.cpp widgets/GiveWidget.cpp InputManager.h Avatar.h MathConverter.h EntityListener.h ConsoleObjectImpl.h MediaDeployer.h AvatarController.h DebugListener.h environment/Water.h TerrainGenerator.h MotionManager.h AvatarCamera.h TerrainShader.h GUIManager.h MousePicker.h  widgets/Widget.h widgets/ConsoleWidget.h widgets/DebugWidget.h widgets/IngameEventsWidget.h widgets/ChatWidget.h widgets/EntityPickerWidget.h widgets/InventoryWidget.h widgets/ServerBrowserWidget.h widgets/InspectWidget.h widgets/MakeEntityWidget.h widgets/ServerWidget.h widgets/GiveWidget.h EmberEntityFactory.cpp EmberEntityFactory.h EmberEntity.h EmberEventProcessor.cpp EmberEventProcessor.h EmberOgre.cpp EmberOgre.h EmberPhysicalEntity.cpp EmberPhysicalEntity.h EmberTerrainPageSource.cpp EmberTerrainPageSource.h EmberTerrainSceneManager.cpp EmberTerrainSceneManager.h EmberEntity.cpp EmberOgrePrerequisites.h AvatarEmberEntity.cpp AvatarEmberEntity.h PersonEmberEntity.cpp PersonEmberEntity.h WorldEmberEntity.cpp WorldEmberEntity.h 
																																																						
EXTRA_DIST = plugins.cfg  resources.cfg  terrain.cfg

confdir = $(datadir)/ember/data/
dist_conf_DATA = ogre.cfg plugins.cfg  resources.cfg  terrain.cfg



