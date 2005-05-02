SUBDIRS = environment model EmberSceneManager modeldefinitions input widgets \
		carpenter jesus

#gamesdir = $(bindir)/games
bin_PROGRAMS = ember.bin

if BUILD_STATIC

ember_bin_LDFLAGS = -nodefaultlibs -rdynamic -export-dynamic -dlpreopen self

EMBER_EXT_LIBS = -L/usr/X11R6/lib \
        /usr/local/lib/liberis-1.3.a \
        /usr/local/lib/libmercator-0.2.a \
        /usr/local/lib/libAtlasObjects-0.6.a \
        /usr/local/lib/libAtlasNet-0.6.a \
        /usr/local/lib/libAtlasCodecs-0.6.a \
        /usr/local/lib/libAtlasMessage-0.6.a \
        /usr/local/lib/libAtlas-0.6.a \
        /usr/local/lib/libskstream-0.3.a \
        /usr/local/lib/libwfmath-0.3.a \
        /usr/local/lib/libCEGUIOgreRenderer.a \
	/usr/local/lib/libCEGUIBase.a \
	/home/erik/opt/ogre_static/lib/libOgreMain.so \
	/usr/lib/libSDL.a \
        /usr/local/lib/libILU.a \
        /usr/local/lib/libIL.a \
 	/usr/lib/libtiff.a \
	/usr/lib/libjpeg.a \
	/usr/lib/libpng12.a \
	/usr/lib/libfreetype.a \
	/usr/lib/libmng.a \
	/usr/lib/liblcms.a \
        /usr/lib/libzzip.a \
	/usr/lib/libz.a \
	/usr/lib/libvarconf-1.0.a \
	/usr/lib/libsigc-2.0.a \
	/home/erik/code/openal/linux/src/libopenal.a \
	/usr/lib/libasound.a \
 	$(STATIC_LIBSTDCPP) \
	$(STATIC_LIBGCC) \
	-lGL -lxerces-c \
	-laudio -lc -lm -lgcc_s -lXext -lstdc++ /usr/lib/libltdl.a 



else
EMBER_EXT_LIBS = $(EMBER_LIBS)
endif


ember_bin_LDADD = $(top_builddir)/src/components/ogre/widgets/libWidgets.a \
	$(top_builddir)/src/services/libServices.a $(top_builddir)/src/services/config/libConfigService.a \
	$(top_builddir)/src/services/metaserver/libMetaserverService.a $(top_builddir)/src/services/server/libServerService.a \
	$(top_builddir)/src/services/sound/libSoundService.a $(top_builddir)/src/services/logging/libLoggingService.a \
	$(top_builddir)/src/framework/libFramework.a $(top_builddir)/src/extensions/libExtensions.a \
	$(top_builddir)/src/components/ogre/environment/libEnvironment.a $(top_builddir)/src/components/ogre/model/libModel.a \
	$(top_builddir)/src/components/ogre/EmberSceneManager/src/libEmberSceneManager.a $(top_builddir)/src/components/ogre/environment/meshtree/libMeshTree.a \
	$(top_builddir)/src/components/ogre/input/libInput.a $(top_builddir)/src/components/ogre/carpenter/libCarpenter.a \
	$(top_builddir)/src/components/ogre/jesus/libJesus.a $(EMBER_EXT_LIBS)


ember_bin_SOURCES = Avatar.cpp EntityListener.cpp ConsoleObjectImpl.cpp \
		MediaDeployer.cpp AvatarController.cpp environment/Water.cpp \
		TerrainGenerator.cpp MotionManager.cpp AvatarCamera.cpp TerrainShader.cpp GUIManager.cpp \
		MousePicker.cpp Avatar.h MathConverter.h EntityListener.h ConsoleObjectImpl.h \
	MediaDeployer.h AvatarController.h  environment/Water.h \
	TerrainGenerator.h MotionManager.h AvatarCamera.h TerrainShader.h GUIManager.h \
	MousePicker.h EmberEntityFactory.cpp EmberEntityFactory.h EmberEntity.h \
	EmberEventProcessor.cpp EmberEventProcessor.h EmberOgre.cpp EmberOgre.h \
	EmberPhysicalEntity.cpp EmberPhysicalEntity.h EmberEntity.cpp EmberOgrePrerequisites.h \
	AvatarEmberEntity.cpp AvatarEmberEntity.h PersonEmberEntity.cpp PersonEmberEntity.h \
	WorldEmberEntity.cpp WorldEmberEntity.h TerrainPage.cpp


#widgetdir = $(datadir)/ember/data/media/gui/cegui/widgets/
#dist_widget_DATA = widgets/ChatWidget.xml widgets/ConsoleWidget.xml widgets/DebugWidget.xml widgets/GiveWidget.xml widgets/InspectWidget.xml widgets/InventoryWidget.xml widgets/MakeEntityWidget.xml widgets/ServerBrowserWidget.xml widgets/ServerWidget.xml widgets/IngameChatWidget.xml widgets/HelpWidget.xml widgets/GUILayout.xsd

#ceguidir = $(datadir)/ember/data/cegui/
#dist_cegui_DATA = cegui


ceguifontsdir = $(datadir)/games/ember/media/gui/cegui/datafiles/fonts
ceguiimagesetsdir = $(datadir)/games/ember/media/gui/cegui/datafiles/imagesets
ceguilayoutsdir = $(datadir)/games/ember/media/gui/cegui/datafiles/layouts
ceguischemesdir = $(datadir)/games/ember/media/gui/cegui/datafiles/schemes
ceguischemadir = $(datadir)/games/ember/media/gui


dist_ceguifonts_DATA = cegui/datafiles/fonts/Font.xsd cegui/datafiles/fonts/tahoma-14.xml cegui/datafiles/fonts/tahoma-10.xml cegui/datafiles/fonts/tahoma-8.xml cegui/datafiles/fonts/tahoma.ttf
dist_ceguiimagesets_DATA = cegui/datafiles/imagesets/Imageset.xsd cegui/datafiles/imagesets/TaharezLook.imageset  cegui/datafiles/imagesets/TaharezLookMouse.imageset cegui/datafiles/imagesets/WindowsLook.imageset
dist_ceguilayouts_DATA = cegui/datafiles/layouts/GUILayout.xsd
dist_ceguischemes_DATA = cegui/datafiles/schemes/TaharezLook.scheme cegui/datafiles/schemes/TaharezLookWidgetAliases.scheme cegui/datafiles/schemes/TaharezLookWidgets.scheme cegui/datafiles/schemes/WindowsLook.scheme cegui/datafiles/schemes/WindowsLookWidgets.scheme cegui/datafiles/schemes/GUIScheme.xsd
dist_ceguischema_DATA = cegui/datafiles/layouts/GUILayout.xsd cegui/datafiles/imagesets/Imageset.xsd cegui/datafiles/fonts/Font.xsd cegui/datafiles/schemes/GUIScheme.xsd

INCLUDES = -I$(top_srcdir)/src 

confdir = $(sysconfdir)/ember/
dist_conf_DATA = ogre.cfg plugins.cfg  resources.cfg 





noinst_HEADERS = TerrainPage.h
