INCLUDES = -I$(top_srcdir)/src  -I$(top_builddir)/src -DPREFIX=\"@prefix@\"
SUBDIRS = environment model SceneManagers input widgets carpenter jesus \
		ogreopcode scripting cegui sound manipulation terrain modeldefinitions

#gamesdir = $(bindir)/games
noinst_LTLIBRARIES = libEmberOgre.la
#bin_PROGRAMS = ember.bin
if BUILD_STATIC

#ember_bin_LDFLAGS = -nodefaultlibs -rdynamic -export-dynamic -dlpreopen self

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
	-lGL \
	-laudio -lc -lm -lgcc_s -lXext -lstdc++ /usr/lib/libltdl.a 



else
EMBER_EXT_LIBS = $(EMBER_LIBS)
endif


libEmberOgre_la_LIBADD = \
	scripting/bindings/lua/liblua_EmberOgre.la \
	scripting/bindings/lua/ogre/liblua_Ogre.la \
	scripting/bindings/lua/helpers/liblua_Helpers.la \
	scripting/libScripting.la\
	widgets/libWidgets.la \
	terrain/libTerrain.la \
	environment/libEnvironment.la \
	model/libModel.la \
	model/mapping/libMapping.la \
	SceneManagers/EmberPagingSceneManager/src/libEmberPagingSceneManager.la \
	input/libInput.la \
	carpenter/libCarpenter.la \
	jesus/libJesus.la \
	manipulation/libmanipulation.la \
	sound/libSound.la \
	ogreopcode/libOgreOpcode.la \
	$(EMBER_EXT_LIBS) 
        


libEmberOgre_la_SOURCES = Avatar.cpp AvatarCamera.cpp AvatarController.cpp \
	AvatarEmberEntity.cpp CameraMount.cpp ConsoleObjectImpl.cpp EmberEntity.cpp \
	EmberEntityActionCreator.cpp EmberEntityFactory.cpp EmberEntityModelAction.cpp \
	EmberEntityPartAction.cpp EmberEntityUserObject.cpp EmberOgre.cpp EmberPhysicalEntity.cpp \
	EntityWorldPickListener.cpp GUICEGUIAdapter.cpp GUIManager.cpp MediaUpdater.cpp MotionManager.cpp \
	MousePicker.cpp OgreInfo.cpp OgreLogObserver.cpp OgreResourceLoader.cpp OgreSetup.cpp \
	OpcodeCollisionDetector.cpp WorldEmberEntity.cpp XMLHelper.cpp

confdir = $(sysconfdir)/ember/
dist_conf_DATA = ogre.cfg plugins.cfg  resources.cfg terrain.cfg


noinst_HEADERS = Avatar.h AvatarCamera.h AvatarController.h AvatarEmberEntity.h \
	CameraMount.h ConsoleObjectImpl.h EmberEntity.h EmberEntityActionCreator.h \
	EmberEntityFactory.h EmberEntityModelAction.h EmberEntityPartAction.h \
	EmberEntityUserObject.h EmberOgre.h EmberOgrePrerequisites.h EmberPhysicalEntity.h \
	EntityWorldPickListener.h GUICEGUIAdapter.h GUIManager.h IWorldPickListener.h MathConverter.h \
	MediaUpdater.h MotionManager.h MousePicker.h OgreIncludes.h OgreInfo.h \
	OgreLogObserver.h OgreResourceLoader.h OgreSetup.h OpcodeCollisionDetector.h \
	WorldEmberEntity.h XMLHelper.h
	
