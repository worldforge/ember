SUBDIRS = environment model EmberSceneManager


INCLUDES = -I$(top_srcdir)/src
bin_PROGRAMS = EmberOgre

EmberOgre_LDADD = \
$(top_builddir)/src/services/libServices.a \
$(top_builddir)/src/services/config/libConfigService.a \
$(top_builddir)/src/services/metaserver/libMetaserverService.a \
$(top_builddir)/src/services/server/libServerService.a \
$(top_builddir)/src/services/sound/libSoundService.a \
$(top_builddir)/src/services/logging/libLoggingService.a \
$(top_builddir)/src/framework/libFramework.a \
$(top_builddir)/src/extensions/libExtensions.a \
$(top_builddir)/src/components/ogre/environment/libEnvironment.a \
$(top_builddir)/src/components/ogre/model/libModel.a \
$(top_builddir)/src/components/ogre/EmberSceneManager/src/libEmberSceneManager.a


EmberOgre_SOURCES =  Avatar.cpp EntityListener.cpp ConsoleObjectImpl.cpp MediaDeployer.cpp AvatarController.cpp DebugListener.cpp environment/Water.cpp TerrainGenerator.cpp MotionManager.cpp AvatarCamera.cpp TerrainShader.cpp GUIManager.cpp MousePicker.cpp  widgets/Widget.cpp widgets/ConsoleWidget.cpp widgets/DebugWidget.cpp widgets/IngameEventsWidget.cpp widgets/ChatWidget.cpp widgets/EntityPickerWidget.cpp widgets/InventoryWidget.cpp widgets/ServerBrowserWidget.cpp widgets/InspectWidget.cpp widgets/MakeEntityWidget.cpp widgets/ServerWidget.cpp widgets/GiveWidget.cpp Avatar.h MathConverter.h EntityListener.h ConsoleObjectImpl.h MediaDeployer.h AvatarController.h DebugListener.h environment/Water.h TerrainGenerator.h MotionManager.h AvatarCamera.h TerrainShader.h GUIManager.h MousePicker.h  widgets/Widget.h widgets/ConsoleWidget.h widgets/DebugWidget.h widgets/IngameEventsWidget.h widgets/ChatWidget.h widgets/EntityPickerWidget.h widgets/InventoryWidget.h widgets/ServerBrowserWidget.h widgets/InspectWidget.h widgets/MakeEntityWidget.h widgets/ServerWidget.h widgets/GiveWidget.h EmberEntityFactory.cpp EmberEntityFactory.h EmberEntity.h EmberEventProcessor.cpp EmberEventProcessor.h EmberOgre.cpp EmberOgre.h EmberPhysicalEntity.cpp EmberPhysicalEntity.h  EmberEntity.cpp EmberOgrePrerequisites.h AvatarEmberEntity.cpp AvatarEmberEntity.h PersonEmberEntity.cpp PersonEmberEntity.h WorldEmberEntity.cpp WorldEmberEntity.h 
																																																						

confdir = $(datadir)/ember/conf/
dist_conf_DATA = ogre.cfg plugins.cfg  resources.cfg  terrain.cfg

widgetdir = $(datadir)/ember/data/widgets/
dist_widget_DATA = widgets/ChatWidget.xml widgets/ConsoleWidget.xml widgets/DebugWidget.xml widgets/GiveWidget.xml widgets/InspectWidget.xml widgets/InventoryWidget.xml widgets/MakeEntityWidget.xml widgets/ServerBrowserWidget.xml widgets/ServerWidget.xml widgets/GUILayout.xsd

#ceguidir = $(datadir)/ember/data/cegui/
#dist_cegui_DATA = cegui





ceguifontsdir = $(datadir)/ember/data/cegui/datafiles/fonts
ceguiimagesetsdir = $(datadir)/ember/data/cegui/datafiles/imagesets
ceguilayoutsdir = $(datadir)/ember/data/cegui/datafiles/layouts
ceguischemesdir = $(datadir)/ember/data/cegui/datafiles/schemes

dist_ceguifonts_DATA = cegui/datafiles/fonts/Font.xsd cegui/datafiles/fonts/tahoma-14.xml cegui/datafiles/fonts/tahoma-8.xml cegui/datafiles/fonts/tahoma.ttf
dist_ceguiimagesets_DATA = cegui/datafiles/imagesets/Imageset.xsd cegui/datafiles/imagesets/TaharezLook.imageset cegui/datafiles/imagesets/WindowsLook.imageset
dist_ceguilayouts_DATA = cegui/datafiles/layouts/GUILayout.xsd
dist_ceguischemes_DATA = cegui/datafiles/schemes/TaharezLook.scheme cegui/datafiles/schemes/TaharezLookWidgetAliases.scheme cegui/datafiles/schemes/TaharezLookWidgets.scheme cegui/datafiles/schemes/WindowsLook.scheme cegui/datafiles/schemes/WindowsLookWidgets.scheme cegui/datafiles/schemes/GUIScheme.xsd
