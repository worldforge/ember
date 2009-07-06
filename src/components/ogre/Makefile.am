INCLUDES = -I$(top_srcdir)/src  -I$(top_builddir)/src -DPREFIX=\"@prefix@\"
SUBDIRS = SceneManagers cegui environment gui  \
	manipulation model modeldefinitions sounddefinitions scripting sound terrain widgets mapping
#carpenter jesus ogreopcode

noinst_LIBRARIES = libEmberOgre.a



libEmberOgre_a_LIBADD = \
	$(top_builddir)/src/components/ogre/SceneManagers/EmberPagingSceneManager/src/libEmberPagingSceneManager.a \
	$(top_builddir)/src/components/ogre/environment/libEnvironment.a $(top_builddir)/src/components/ogre/manipulation/libmanipulation.a \
	$(top_builddir)/src/components/ogre/model/libModel.a $(top_builddir)/src/components/ogre/mapping/libMapping.a \
	$(top_builddir)/src/components/ogre/scripting/bindings/lua/helpers/liblua_Helpers.a \
	$(top_builddir)/src/components/ogre/scripting/bindings/lua/liblua_EmberOgre.a \
	$(top_builddir)/src/components/ogre/scripting/bindings/lua/ogre/liblua_Ogre.a $(top_builddir)/src/components/ogre/scripting/libScripting.a \
	$(top_builddir)/src/components/ogre/sound/libSound.a $(top_builddir)/src/components/ogre/terrain/libTerrain.a \
	$(top_builddir)/src/components/ogre/widgets/libWidgets.a $(top_builddir)/src/components/ogre/gui/libGui.a
        
#	$(top_builddir)/src/components/ogre/jesus/libJesus.a $(top_builddir)/src/components/ogre/carpenter/libCarpenter.a $(top_builddir)/src/components/ogre/ogreopcode/src/libOgreOpcode.a 


libEmberOgre_a_SOURCES = AttributeObserver.cpp Avatar.cpp AvatarCamera.cpp \
	AvatarController.cpp AvatarEmberEntity.cpp AvatarLogger.cpp AvatarTerrainCursor.cpp \
	CameraMount.cpp ConsoleObjectImpl.cpp EmberEntity.cpp EmberEntityActionCreator.cpp \
	EmberEntityFactory.cpp EmberEntityHideModelAction.cpp EmberEntityModelAction.cpp \
	EmberEntityPartAction.cpp EmberEntityUserObject.cpp EmberOgre.cpp EmberOgreFileSystem.cpp \
	EmberPhysicalEntity.cpp EntityWorldPickListener.cpp GUICEGUIAdapter.cpp GUIManager.cpp \
	MediaUpdater.cpp MeshCollisionDetector.cpp MeshSerializerListener.cpp ModelMount.cpp \
	MotionManager.cpp MousePicker.cpp OgreInfo.cpp OgreLogObserver.cpp OgreResourceLoader.cpp \
	OgreResourceProvider.cpp OgreSetup.cpp  \
	 ShaderManager.cpp ShadowCameraSetup.cpp SimpleRenderContext.cpp \
	WorldEmberEntity.cpp XMLHelper.cpp
# OpcodeCollisionDetector.cpp  OpcodeCollisionDetectorVisualizer.cpp
confdir = $(sysconfdir)/ember
dist_conf_DATA = ogre.cfg resources.cfg terrain.cfg


noinst_HEADERS = AttributeObserver.h Avatar.h AvatarCamera.h AvatarController.h \
	AvatarEmberEntity.h AvatarLogger.h AvatarTerrainCursor.h CameraMount.h ConsoleObjectImpl.h \
	EmberEntity.h EmberEntityActionCreator.h EmberEntityFactory.h \
	EmberEntityHideModelAction.h EmberEntityModelAction.h EmberEntityPartAction.h \
	EmberEntityUserObject.h EmberOgre.h EmberOgreFileSystem.h EmberOgrePrerequisites.h \
	EmberPhysicalEntity.h EntityWorldPickListener.h GUICEGUIAdapter.h GUIManager.h \
	IWorldPickListener.h Convert.h MediaUpdater.h MeshCollisionDetector.h \
	MeshSerializerListener.h ModelMount.h MotionManager.h MousePicker.h OgreIncludes.h OgreInfo.h \
	OgreLogObserver.h OgreResourceLoader.h OgreResourceProvider.h OgreSetup.h \
	 ShaderManager.h ShadowCameraSetup.h \
	SimpleRenderContext.h Types.h WorldEmberEntity.h XMLHelper.h
#OpcodeCollisionDetector.h OpcodeCollisionDetectorVisualizer.h
#libEmberOgre_la_LDFLAGS = -no-undefined
