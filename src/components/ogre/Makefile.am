INCLUDES = -I$(top_srcdir)/src  -I$(top_builddir)/src -DPREFIX=\"@prefix@\"
SUBDIRS = SceneManagers carpenter cegui environment image input jesus \
	manipulation model modeldefinitions ogreopcode scripting sound terrain widgets

noinst_LTLIBRARIES = libEmberOgre.la



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
	image/libImage.la 
        


libEmberOgre_la_SOURCES = Avatar.cpp AvatarCamera.cpp AvatarController.cpp \
	AvatarEmberEntity.cpp CameraMount.cpp ConsoleObjectImpl.cpp EmberEntity.cpp \
	EmberEntityActionCreator.cpp EmberEntityFactory.cpp EmberEntityModelAction.cpp \
	EmberEntityPartAction.cpp EmberEntityUserObject.cpp EmberOgre.cpp EmberPhysicalEntity.cpp \
	EntityWorldPickListener.cpp GUICEGUIAdapter.cpp GUIManager.cpp MediaUpdater.cpp MotionManager.cpp \
	MousePicker.cpp OgreInfo.cpp OgreLogObserver.cpp OgreResourceLoader.cpp \
	OgreResourceProvider.cpp OgreSetup.cpp OpcodeCollisionDetector.cpp WorldEmberEntity.cpp \
	XMLHelper.cpp

confdir = $(sysconfdir)/ember/
dist_conf_DATA = ogre.cfg plugins.cfg  resources.cfg terrain.cfg


noinst_HEADERS = Avatar.h AvatarCamera.h AvatarController.h AvatarEmberEntity.h \
	CameraMount.h ConsoleObjectImpl.h EmberEntity.h EmberEntityActionCreator.h \
	EmberEntityFactory.h EmberEntityModelAction.h EmberEntityPartAction.h \
	EmberEntityUserObject.h EmberOgre.h EmberOgrePrerequisites.h EmberPhysicalEntity.h \
	EntityWorldPickListener.h GUICEGUIAdapter.h GUIManager.h IWorldPickListener.h MathConverter.h \
	MediaUpdater.h MotionManager.h MousePicker.h OgreIncludes.h OgreInfo.h \
	OgreLogObserver.h OgreResourceLoader.h OgreResourceProvider.h OgreSetup.h \
	OpcodeCollisionDetector.h WorldEmberEntity.h XMLHelper.h
	
