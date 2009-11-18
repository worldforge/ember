INCLUDES = -I$(top_srcdir)/src  -I$(top_builddir)/src -DPREFIX=\"@prefix@\"
SUBDIRS = SceneManagers authoring environment modeldefinitions sounddefinitions scripting widgets
#carpenter jesus ogreopcode

noinst_LIBRARIES = libEmberOgre.a



libEmberOgre_a_SOURCES = \
	authoring/DetachedEntity.cpp authoring/EntityMoveAdjuster.cpp \
	authoring/EntityMoveManager.cpp authoring/EntityMover.cpp authoring/EntityMoverBase.cpp authoring/EntityRecipe.cpp authoring/EntityRecipeManager.cpp authoring/GUIAdapter.cpp \
	authoring/GUIAdapterBindings.cpp authoring/MaterialEditor.cpp authoring/MovementAdapter.cpp authoring/Polygon.cpp authoring/PolygonPoint.cpp \
	authoring/PolygonPointMovement.cpp authoring/PolygonPointMover.cpp authoring/PolygonPointPickListener.cpp \
	authoring/PolygonPointUserObject.cpp authoring/PolygonRenderer.cpp authoring/XMLEntityRecipeSerializer.cpp authoring/SnapToMovement.cpp \
	authoring/AuthoringHandler.cpp authoring/AuthoringManager.cpp authoring/AuthoringVisualization.cpp authoring/AuthoringVisualizationCollisionDetector.cpp \
	authoring/AuthoringVisualizationMover.cpp authoring/AuthoringMoverConnector.cpp authoring/RawTypeInfoRepository.cpp \
	camera/MainCamera.cpp camera/CameraMountBase.cpp camera/FirstPersonCameraMount.cpp camera/Recorder.cpp camera/ThirdPersonCameraMount.cpp \
	environment/CaelumEnvironment.cpp environment/CaelumSky.cpp environment/CaelumSun.cpp \
	environment/EmberEntityLoader.cpp environment/Environment.cpp environment/Foliage.cpp environment/FoliageBase.cpp environment/FoliageLayer.cpp \
	environment/FoliageLoader.cpp environment/Forest.cpp environment/GrassFoliage.cpp environment/LensFlare.cpp \
	environment/ShrubberyFoliage.cpp environment/SimpleEnvironment.cpp environment/SimpleWater.cpp environment/Sun.cpp environment/Tree.cpp environment/Water.cpp environment/OceanRepresentation.cpp environment/OceanAction.cpp environment/SimpleWaterCollisionDetector.cpp \
	gui/ActiveWidgetHandler.cpp gui/CEGUILogger.cpp \
\
	mapping/EmberEntityMappingManager.cpp mapping/XMLEntityMappingDefinitionSerializer.cpp \
\
	model/Action.cpp model/AnimationSet.cpp model/Model.cpp \
	model/ModelBackgroundLoader.cpp model/ModelDefinition.cpp model/ModelDefinitionAtlasComposer.cpp \
	model/ModelDefinitionManager.cpp model/ModelPart.cpp model/ParticleSystem.cpp model/ParticleSystemBinding.cpp model/SubModel.cpp \
	model/SubModelPart.cpp model/XMLModelDefinitionSerializer.cpp model/ModelRepresentation.cpp \
	model/ModelRepresentationManager.cpp model/ModelMount.cpp model/ModelAttachment.cpp model/ModelBoneProvider.cpp model/ModelFitting.cpp model/ModelPartReactivatorVisitor.cpp\
\
	sound/SoundAction.cpp sound/SoundDefinition.cpp sound/SoundDefinitionManager.cpp \
	sound/SoundEntity.cpp sound/SoundGroup.cpp sound/SoundGroupDefinition.cpp sound/XMLSoundDefParser.cpp \
\
	terrain/Map.cpp terrain/TerrainArea.cpp \
	terrain/TerrainAreaParser.cpp terrain/TerrainEditor.cpp \
	terrain/TerrainGenerator.cpp terrain/TerrainInfo.cpp terrain/TerrainLayerDefinition.cpp \
	terrain/TerrainLayerDefinitionManager.cpp terrain/TerrainMod.cpp \
	terrain/TerrainPage.cpp terrain/TerrainPageFoliage.cpp terrain/TerrainPageGeometry.cpp \
	terrain/TerrainPageShadow.cpp terrain/TerrainPageSurface.cpp terrain/TerrainPageSurfaceCompiler.cpp \
	terrain/TerrainPageSurfaceCompilerTechniqueShader.cpp terrain/TerrainPageSurfaceCompilerTechniqueSimple.cpp \
	terrain/TerrainPageSurfaceLayer.cpp terrain/TerrainShader.cpp terrain/XMLLayerDefinitionSerializer.cpp terrain/PlantAreaQuery.cpp terrain/TerrainParser.cpp \
\
	widgets/AssetsManager.cpp widgets/ColouredListItem.cpp widgets/Compass.cpp \
	widgets/ConsoleAdapter.cpp widgets/EntityCEGUITexture.cpp widgets/EntityCreator.cpp widgets/EntityCreatorActionCreator.cpp \
	widgets/EntityCreatorHideModelAction.cpp widgets/EntityCreatorModelAction.cpp widgets/EntityCreatorMovement.cpp \
	widgets/EntityCreatorMovementBridge.cpp widgets/EntityCreatorPartAction.cpp widgets/EntityEditor.cpp widgets/EntityIcon.cpp \
	widgets/EntityIconDragDropTarget.cpp widgets/EntityIconManager.cpp widgets/EntityIconSlot.cpp widgets/Help.cpp widgets/IconBar.cpp \
	widgets/IconBase.cpp widgets/IngameChatWidget.cpp widgets/InspectWidget.cpp widgets/ListHolder.cpp \
	widgets/LoadingBar.cpp widgets/MakeEntityWidget.cpp widgets/MeshPreview.cpp widgets/ModelRenderer.cpp \
	widgets/MovableObjectRenderer.cpp widgets/OgreEntityRenderer.cpp widgets/QuaternionAdapter.cpp widgets/Quit.cpp widgets/ServerWidget.cpp \
	widgets/StackableContainer.cpp widgets/Vector3Adapter.cpp widgets/Widget.cpp widgets/WidgetDefinitions.cpp widgets/WidgetPool.cpp widgets/AtlasHelper.cpp \
	widgets/icons/Icon.cpp widgets/icons/IconImageStore.cpp widgets/icons/IconManager.cpp \
	widgets/icons/IconRenderer.cpp widgets/icons/IconStore.cpp \
	widgets/adapters/ListBinder.cpp widgets/adapters/atlas/AdapterBase.cpp widgets/adapters/atlas/AdapterFactory.cpp widgets/adapters/atlas/AreaAdapter.cpp \
	widgets/adapters/atlas/CustomAdapter.cpp widgets/adapters/atlas/ListAdapter.cpp widgets/adapters/atlas/MapAdapter.cpp widgets/adapters/atlas/NumberAdapter.cpp widgets/adapters/atlas/OrientationAdapter.cpp \
	widgets/adapters/atlas/PolygonAdapter.cpp widgets/adapters/atlas/Position2DAdapter.cpp widgets/adapters/atlas/PositionAdapter.cpp widgets/adapters/atlas/SizeAdapter.cpp \
	widgets/adapters/atlas/StaticAdapter.cpp widgets/adapters/atlas/StringAdapter.cpp widgets/adapters/atlas/TerrainModAdapter.cpp widgets/adapters/atlas/bindings/lua/bindings.cpp \
	widgets/adapters/eris/TypeTreeAdapter.cpp \
\
	AttributeObserver.cpp Avatar.cpp \
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
	AttachmentBase.cpp AvatarCameraMotionHandler.cpp FreeFlyingCameraMotionHandler.cpp SceneNodeProvider.cpp \
	EntityObserverBase.cpp
	
# OpcodeCollisionDetector.cpp  OpcodeCollisionDetectorVisualizer.cpp
confdir = $(sysconfdir)/ember
dist_conf_DATA = ogre.cfg resources.cfg terrain.cfg


noinst_HEADERS = \
	authoring/DetachedEntity.h authoring/EntityMoveAdjuster.h authoring/EntityMoveManager.h \
	authoring/EntityMover.h authoring/EntityMoverBase.h authoring/EntityRecipe.h authoring/EntityRecipeManager.h authoring/GUIAdapter.h authoring/GUIAdapterBindings.h \
	authoring/IMovementBridge.h authoring/IPolygonPositionProvider.h authoring/MaterialEditor.h authoring/MovementAdapter.h authoring/Polygon.h \
	authoring/PolygonPoint.h authoring/PolygonPointMovement.h authoring/PolygonPointMover.h authoring/PolygonPointPickListener.h \
	authoring/PolygonPointUserObject.h authoring/PolygonRenderer.h authoring/XMLEntityRecipeSerializer.h authoring/SnapToMovement.h \
	authoring/AuthoringHandler.h authoring/AuthoringManager.h authoring/AuthoringVisualization.h authoring/AuthoringVisualizationCollisionDetector.h \
	authoring/AuthoringVisualizationMover.h authoring/AuthoringMoverConnector.h authoring/RawTypeInfoRepository.h \
\
	camera/MainCamera.h camera/CameraMountBase.h camera/FirstPersonCameraMount.h camera/Recorder.h camera/ThirdPersonCameraMount.h camera/ICameraMount.h \
\
	environment/CaelumEnvironment.h environment/CaelumSky.h environment/CaelumSun.h \
	environment/EmberEntityLoader.h environment/Environment.h environment/Foliage.h environment/FoliageBase.h environment/FoliageLayer.h environment/FoliageLoader.h \
	environment/Forest.h environment/GrassFoliage.h environment/LensFlare.h environment/ShrubberyFoliage.h \
	environment/SimpleEnvironment.h environment/SimpleWater.h environment/Sun.h environment/Tree.h environment/Water.h environment/OceanRepresentation.h environment/OceanAction.h environment/SimpleWaterCollisionDetector.h \
\
	gui/ActiveWidgetHandler.h gui/CEGUILogger.h \
\
	mapping/EmberEntityMappingManager.h mapping/XMLEntityMappingDefinitionSerializer.h \
\
	model/Action.h model/AnimationSet.h model/Model.h model/ModelBackgroundLoader.h \
	model/ModelDefinition.h model/ModelDefinitionAtlasComposer.h model/ModelDefinitionManager.h model/ModelPart.h \
	model/ParticleSystem.h model/ParticleSystemBinding.h model/SubModel.h model/SubModelPart.h \
	model/XMLModelDefinitionSerializer.h model/ModelRepresentation.h model/ModelRepresentationManager.h \
	model/ModelMount.h model/ModelAttachment.h model/ModelBoneProvider.h model/ModelFitting.h model/ModelPartReactivatorVisitor.h \
\
	sound/SoundAction.h sound/SoundDefinition.h sound/SoundDefinitionManager.h sound/SoundEntity.h \
	sound/SoundGroup.h sound/SoundGroupDefinition.h sound/XMLSoundDefParser.h \
\
	terrain/ISceneManagerAdapter.h terrain/ITerrainPageBridge.h terrain/Map.h terrain/TerrainArea.h \
	terrain/TerrainAreaParser.h terrain/TerrainEditor.h terrain/TerrainGenerator.h terrain/TerrainInfo.h terrain/TerrainLayerDefinition.h terrain/TerrainLayerDefinitionManager.h \
	terrain/TerrainMod.h terrain/TerrainPage.h terrain/TerrainPageFoliage.h terrain/TerrainPageGeometry.h terrain/TerrainPageShadow.h terrain/TerrainPageSurface.h \
	terrain/TerrainPageSurfaceCompiler.h terrain/TerrainPageSurfaceCompilerTechniqueShader.h \
	terrain/TerrainPageSurfaceCompilerTechniqueSimple.h terrain/TerrainPageSurfaceLayer.h terrain/TerrainShader.h terrain/XMLLayerDefinitionSerializer.h terrain/PlantAreaQuery.h terrain/TerrainParser.h \
\
	widgets/AssetsManager.h widgets/ColouredListItem.h widgets/Compass.h widgets/ConsoleAdapter.h \
	widgets/EntityCEGUITexture.h widgets/EntityCreator.h widgets/EntityCreatorActionCreator.h \
	widgets/EntityCreatorHideModelAction.h widgets/EntityCreatorModelAction.h widgets/EntityCreatorMovement.h \
	widgets/EntityCreatorMovementBridge.h widgets/EntityCreatorPartAction.h widgets/EntityEditor.h widgets/EntityIcon.h \
	widgets/EntityIconDragDropTarget.h widgets/EntityIconManager.h widgets/EntityIconSlot.h widgets/Help.h widgets/IconBar.h widgets/IconBase.h \
	widgets/IngameChatWidget.h widgets/InspectWidget.h widgets/ListHolder.h widgets/LoadingBar.h \
	widgets/MakeEntityWidget.h widgets/MeshPreview.h widgets/ModelRenderer.h widgets/MovableObjectRenderer.h \
	widgets/OgreEntityRenderer.h widgets/QuaternionAdapter.h widgets/Quit.h widgets/ServerWidget.h widgets/StackableContainer.h \
	widgets/Vector3Adapter.h widgets/Widget.h widgets/WidgetDefinitions.h widgets/WidgetPool.h widgets/AtlasHelper.h \
	widgets/icons/Icon.h widgets/icons/IconImageStore.h widgets/icons/IconManager.h widgets/icons/IconRenderer.h \
	widgets/icons/IconStore.h \
	widgets/adapters/ListBinder.h widgets/adapters/atlas/AdapterBase.h widgets/adapters/atlas/AdapterFactory.h widgets/adapters/atlas/AreaAdapter.h widgets/adapters/atlas/CustomAdapter.h \
	widgets/adapters/atlas/ListAdapter.h widgets/adapters/atlas/MapAdapter.h widgets/adapters/atlas/NumberAdapter.h widgets/adapters/atlas/OrientationAdapter.h widgets/adapters/atlas/PolygonAdapter.h \
	widgets/adapters/atlas/Position2DAdapter.h widgets/adapters/atlas/PositionAdapter.h widgets/adapters/atlas/SizeAdapter.h widgets/adapters/atlas/StaticAdapter.h widgets/adapters/atlas/StringAdapter.h \
	widgets/adapters/atlas/TerrainModAdapter.h \
	widgets/adapters/eris/TypeTreeAdapter.h \
\
	AttributeObserver.h Avatar.h MovementController.h \
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
	IEntityControlDelegate.h NodeController.h DelegatingNodeController.h \
	AvatarAttachmentController.h IMovementProvider.h ICameraMotionHandler.h HiddenAttachment.h \
	AttachmentBase.h AvatarCameraMotionHandler.h FreeFlyingCameraMotionHandler.h \
	ICollisionDetector.h INodeProvider.h SceneNodeProvider.h IVisualizable.h IEntityVisitor.h \
	EntityObserverBase.h
#OpcodeCollisionDetector.h OpcodeCollisionDetectorVisualizer.h
