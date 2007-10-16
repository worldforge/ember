INCLUDES = -I$(top_srcdir)/src  -I$(top_builddir)/src -DPREFIX=\"@prefix@\"
SUBDIRS = SceneManagers carpenter cegui environment input jesus \
	manipulation model modeldefinitions ogreopcode scripting sound terrain widgets

noinst_LTLIBRARIES = libEmberOgre.la



libEmberOgre_la_LIBADD = \
	$(top_builddir)/src/components/ogre/SceneManagers/EmberPagingSceneManager/src/libEmberPagingSceneManager.la $(top_builddir)/src/components/ogre/carpenter/libCarpenter.la \
	$(top_builddir)/src/components/ogre/environment/libEnvironment.la $(top_builddir)/src/components/ogre/input/libInput.la \
	$(top_builddir)/src/components/ogre/jesus/libJesus.la $(top_builddir)/src/components/ogre/manipulation/libmanipulation.la \
	$(top_builddir)/src/components/ogre/model/libModel.la $(top_builddir)/src/components/ogre/model/mapping/libMapping.la \
	$(top_builddir)/src/components/ogre/scripting/bindings/lua/helpers/liblua_Helpers.la \
	$(top_builddir)/src/components/ogre/scripting/bindings/lua/liblua_EmberOgre.la \
	$(top_builddir)/src/components/ogre/scripting/bindings/lua/ogre/liblua_Ogre.la $(top_builddir)/src/components/ogre/scripting/libScripting.la \
	$(top_builddir)/src/components/ogre/sound/libSound.la $(top_builddir)/src/components/ogre/terrain/libTerrain.la \
	$(top_builddir)/src/components/ogre/widgets/libWidgets.la \
	$(top_builddir)/src/components/ogre/ogreopcode/src/libOgreOpcode.la
        


libEmberOgre_la_SOURCES = AttributeObserver.cpp Avatar.cpp AvatarCamera.cpp \
	AvatarController.cpp AvatarEmberEntity.cpp CameraMount.cpp ConsoleObjectImpl.cpp \
	EmberEntity.cpp EmberEntityActionCreator.cpp EmberEntityFactory.cpp \
	EmberEntityModelAction.cpp EmberEntityPartAction.cpp EmberEntityUserObject.cpp EmberOgre.cpp \
	EmberPhysicalEntity.cpp EntityWorldPickListener.cpp GUICEGUIAdapter.cpp GUIManager.cpp \
	MediaUpdater.cpp MotionManager.cpp MousePicker.cpp OgreInfo.cpp OgreLogObserver.cpp \
	OgreResourceLoader.cpp OgreResourceProvider.cpp OgreSetup.cpp OpcodeCollisionDetector.cpp \
	OpcodeCollisionDetectorVisualizer.cpp SimpleRenderContext.cpp WorldEmberEntity.cpp XMLHelper.cpp

confdir = $(sysconfdir)/ember/
dist_conf_DATA = ogre.cfg resources.cfg terrain.cfg


noinst_HEADERS = AttributeObserver.h Avatar.h AvatarCamera.h AvatarController.h \
	AvatarEmberEntity.h CameraMount.h ConsoleObjectImpl.h EmberEntity.h \
	EmberEntityActionCreator.h EmberEntityFactory.h EmberEntityModelAction.h EmberEntityPartAction.h \
	EmberEntityUserObject.h EmberOgre.h EmberOgrePrerequisites.h EmberPhysicalEntity.h \
	EntityWorldPickListener.h GUICEGUIAdapter.h GUIManager.h IWorldPickListener.h MathConverter.h \
	MediaUpdater.h MotionManager.h MousePicker.h OgreIncludes.h OgreInfo.h \
	OgreLogObserver.h OgreResourceLoader.h OgreResourceProvider.h OgreSetup.h \
	OpcodeCollisionDetector.h OpcodeCollisionDetectorVisualizer.h SimpleRenderContext.h \
	WorldEmberEntity.h XMLHelper.h
	
#libEmberOgre_la_LDFLAGS = -no-undefined
