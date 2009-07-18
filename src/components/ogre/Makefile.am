INCLUDES = -I$(top_srcdir)/src  -I$(top_builddir)/src -DPREFIX=\"@prefix@\"
SUBDIRS = SceneManagers cegui environment gui  \
	manipulation model modeldefinitions sounddefinitions scripting sound terrain widgets mapping
#carpenter jesus ogreopcode

noinst_LIBRARIES = libEmberOgre.a



libEmberOgre_a_SOURCES = AttributeObserver.cpp Avatar.cpp AvatarCamera.cpp \
	AvatarController.cpp AvatarEmberEntity.cpp AvatarLogger.cpp AvatarTerrainCursor.cpp \
	CameraMount.cpp ConsoleObjectImpl.cpp EmberEntity.cpp EmberEntityActionCreator.cpp \
	EmberEntityFactory.cpp EmberEntityHideModelAction.cpp EmberEntityModelAction.cpp \
	EmberEntityPartAction.cpp EmberEntityUserObject.cpp EmberOgre.cpp EmberOgreFileSystem.cpp \
	EntityWorldPickListener.cpp GUICEGUIAdapter.cpp GUIManager.cpp \
	MediaUpdater.cpp MeshCollisionDetector.cpp MeshSerializerListener.cpp \
	MotionManager.cpp MousePicker.cpp OgreInfo.cpp OgreLogObserver.cpp OgreResourceLoader.cpp \
	OgreResourceProvider.cpp OgreSetup.cpp SceneNodeAttachment.cpp \
	ShaderManager.cpp ShadowCameraSetup.cpp SimpleRenderContext.cpp \
	WorldEmberEntity.cpp XMLHelper.cpp WorldAttachment.cpp SceneNodeController.cpp \
	DelegatingSceneNodeController.cpp
# OpcodeCollisionDetector.cpp  OpcodeCollisionDetectorVisualizer.cpp
confdir = $(sysconfdir)/ember
dist_conf_DATA = ogre.cfg resources.cfg terrain.cfg


noinst_HEADERS = AttributeObserver.h Avatar.h AvatarCamera.h AvatarController.h \
	AvatarEmberEntity.h AvatarLogger.h AvatarTerrainCursor.h CameraMount.h ConsoleObjectImpl.h \
	EmberEntity.h EmberEntityActionCreator.h EmberEntityFactory.h \
	EmberEntityHideModelAction.h EmberEntityModelAction.h EmberEntityPartAction.h \
	EmberEntityUserObject.h EmberOgre.h EmberOgreFileSystem.h EmberOgrePrerequisites.h \
	EntityWorldPickListener.h GUICEGUIAdapter.h GUIManager.h \
	IWorldPickListener.h Convert.h MediaUpdater.h MeshCollisionDetector.h \
	MeshSerializerListener.h MotionManager.h MousePicker.h OgreIncludes.h OgreInfo.h \
	OgreLogObserver.h OgreResourceLoader.h OgreResourceProvider.h OgreSetup.h \
	ShaderManager.h ShadowCameraSetup.h \
	SimpleRenderContext.h Types.h WorldEmberEntity.h XMLHelper.h IGraphicalRepresentation.h \
	IEntityAttachment.h  WorldAttachment.h SceneNodeAttachment.h IMovable.h IAnimated.h IAttachmentControlDelegate.h \
	SceneNodeController.h DelegatingSceneNodeController.h
#OpcodeCollisionDetector.h OpcodeCollisionDetectorVisualizer.h
