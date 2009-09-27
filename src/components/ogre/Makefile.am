INCLUDES = -I$(top_srcdir)/src  -I$(top_builddir)/src -DPREFIX=\"@prefix@\"
SUBDIRS = SceneManagers environment gui  \
	authoring model modeldefinitions sounddefinitions scripting sound terrain widgets mapping camera
#carpenter jesus ogreopcode

noinst_LIBRARIES = libEmberOgre.a



libEmberOgre_a_SOURCES = AttributeObserver.cpp Avatar.cpp \
	MovementController.cpp AvatarLogger.cpp AvatarTerrainCursor.cpp \
	CameraMount.cpp ConsoleObjectImpl.cpp EmberEntity.cpp EmberEntityActionCreator.cpp \
	EmberEntityFactory.cpp EmberEntityHideModelAction.cpp EmberEntityModelAction.cpp \
	EmberEntityPartAction.cpp EmberEntityUserObject.cpp EmberOgre.cpp EmberOgreFileSystem.cpp \
	EntityWorldPickListener.cpp GUICEGUIAdapter.cpp GUIManager.cpp \
	MediaUpdater.cpp MeshCollisionDetector.cpp MeshSerializerListener.cpp \
	MotionManager.cpp MousePicker.cpp OgreInfo.cpp OgreLogObserver.cpp OgreResourceLoader.cpp \
	OgreResourceProvider.cpp OgreSetup.cpp NodeAttachment.cpp \
	ShaderManager.cpp ShadowCameraSetup.cpp SimpleRenderContext.cpp \
	WorldEmberEntity.cpp XMLHelper.cpp WorldAttachment.cpp NodeController.cpp \
	DelegatingNodeController.cpp AvatarAttachmentController.cpp HiddenAttachment.cpp \
	AttachmentBase.cpp AvatarCameraMotionHandler.cpp FreeFlyingCameraMotionHandler.cpp SceneNodeProvider.cpp
# OpcodeCollisionDetector.cpp  OpcodeCollisionDetectorVisualizer.cpp
confdir = $(sysconfdir)/ember
dist_conf_DATA = ogre.cfg resources.cfg terrain.cfg


noinst_HEADERS = AttributeObserver.h Avatar.h MovementController.h \
	AvatarLogger.h AvatarTerrainCursor.h CameraMount.h ConsoleObjectImpl.h \
	EmberEntity.h EmberEntityActionCreator.h EmberEntityFactory.h \
	EmberEntityHideModelAction.h EmberEntityModelAction.h EmberEntityPartAction.h \
	EmberEntityUserObject.h EmberOgre.h EmberOgreFileSystem.h EmberOgrePrerequisites.h \
	EntityWorldPickListener.h GUICEGUIAdapter.h GUIManager.h \
	IWorldPickListener.h Convert.h MediaUpdater.h MeshCollisionDetector.h \
	MeshSerializerListener.h MotionManager.h MousePicker.h OgreIncludes.h OgreInfo.h \
	OgreLogObserver.h OgreResourceLoader.h OgreResourceProvider.h OgreSetup.h \
	ShaderManager.h ShadowCameraSetup.h \
	SimpleRenderContext.h Types.h WorldEmberEntity.h XMLHelper.h IGraphicalRepresentation.h \
	IEntityAttachment.h  WorldAttachment.h NodeAttachment.h IMovable.h IAnimated.h \
	IAttachmentControlDelegate.h NodeController.h DelegatingNodeController.h \
	AvatarAttachmentController.h IMovementProvider.h ICameraMotionHandler.h HiddenAttachment.h \
	AttachmentBase.h AvatarCameraMotionHandler.h FreeFlyingCameraMotionHandler.h \
	ICollisionDetector.h INodeProvider.h SceneNodeProvider.h IVisualizable.h IEntityVisitor.h
#OpcodeCollisionDetector.h OpcodeCollisionDetectorVisualizer.h
