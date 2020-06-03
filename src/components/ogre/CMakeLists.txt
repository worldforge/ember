add_library(emberogre
        authoring/ConsoleDevTools.cpp authoring/DetachedEntity.cpp authoring/EntityMoveAdjuster.cpp
        authoring/EntityMoveManager.cpp authoring/EntityMover.cpp authoring/EntityMoverBase.cpp authoring/EntityRecipe.cpp authoring/EntityRecipeManager.cpp authoring/GUIAdapter.cpp
        authoring/GUIAdapterBindings.cpp authoring/MaterialEditor.cpp authoring/MovementAdapter.cpp authoring/Polygon.cpp authoring/PolygonPoint.cpp
        authoring/PolygonPointMovement.cpp authoring/PolygonPointMover.cpp authoring/PolygonPointPickListener.cpp
        authoring/PolygonPointUserObject.cpp authoring/PolygonRenderer.cpp authoring/XMLEntityRecipeSerializer.cpp authoring/SnapToMovement.cpp
        authoring/AuthoringHandler.cpp authoring/AuthoringManager.cpp authoring/AuthoringVisualization.cpp
        authoring/AuthoringVisualizationMover.cpp authoring/AuthoringMoverConnector.cpp authoring/RulesFetcher.cpp authoring/RuleEditor.cpp
        authoring/SimpleEntityVisualization.cpp authoring/EntityMaker.cpp authoring/AwarenessVisualizer.cpp
        authoring/EntityConsoleEditor.cpp authoring/EntityConsoleEditor.cpp
        authoring/IMovementBridge.h authoring/IPolygonPositionProvider.h authoring/GeometryVisualization.cpp authoring/GeometryVisualization.h

        lod/LodLevelManager.cpp

        camera/MainCamera.cpp camera/CameraMountBase.cpp camera/FirstPersonCameraMount.cpp camera/Recorder.cpp camera/ThirdPersonCameraMount.cpp
        camera/CameraSettings.cpp camera/ICameraMount.h
        environment/CaelumEnvironment.cpp environment/CaelumSky.cpp environment/CaelumSun.cpp
        environment/EmberEntityLoader.cpp environment/Environment.cpp environment/Foliage.cpp environment/FoliageBase.cpp environment/FoliageLayer.cpp
        environment/FoliageLoader.cpp environment/Forest.cpp environment/GrassFoliage.cpp environment/LensFlare.cpp
        environment/ShrubberyFoliage.cpp environment/SimpleEnvironment.cpp environment/SimpleWater.cpp environment/Sun.cpp environment/Tree.cpp environment/Water.cpp
        environment/OceanRepresentation.cpp environment/OceanAction.cpp
        environment/ExclusiveImposterPage.cpp
        environment/FoliageDetailManager.cpp
        environment/IEnvironmentProvider.h
        gui/ActiveWidgetHandler.cpp gui/CursorWorldListener.cpp

        lod/LodDefinition.cpp lod/LodDefinitionManager.cpp lod/LodManager.cpp lod/XMLLodDefinitionSerializer.cpp lod/PMInjectorSignaler.cpp
        lod/ScaledPixelCountLodStrategy.cpp

        mapping/EmberEntityMappingManager.cpp mapping/XMLEntityMappingDefinitionSerializer.cpp mapping/ModelActionCreator.cpp mapping/ModelActionCreator.h

        model/ModelAction.cpp model/AnimationSet.cpp model/Model.cpp
        model/ModelBackgroundLoader.cpp model/ModelDefinition.cpp model/ModelDefinitionAtlasComposer.cpp
        model/ModelDefinitionManager.cpp model/ModelPart.cpp model/ParticleSystem.cpp model/ParticleSystemBinding.cpp model/SubModel.cpp
        model/SubModelPart.cpp model/XMLModelDefinitionSerializer.cpp model/ModelRepresentation.cpp
        model/ModelMount.cpp model/ModelAttachment.cpp model/ModelBoneProvider.cpp model/ModelFitting.cpp model/ModelPartReactivatorVisitor.cpp

        sound/SoundAction.cpp sound/SoundDefinitionManager.cpp
        sound/SoundEntity.cpp sound/SoundGroup.cpp sound/XMLSoundDefParser.cpp

        terrain/OgreTerrain/OgreTerrainMaterialGeneratorEmber.cpp
        terrain/OgreTerrain/OgreTerrainDefiner.cpp
        terrain/OgreTerrain/OgreTerrainAdapter.cpp terrain/OgreTerrain/OgreTerrainObserver.cpp terrain/OgreTerrain/OgreTerrainPageBridge.cpp terrain/OgreTerrain/OgreTerrainPageProvider.cpp
        terrain/OgreTerrain/EmberTerrainGroup.cpp terrain/OgreTerrain/EmberTerrain.cpp terrain/OgreTerrain/CameraFocusedGrid2DPageStrategy.cpp
        terrain/Map.cpp terrain/TerrainArea.cpp
        terrain/TerrainAreaParser.cpp terrain/TerrainEditor.cpp
        terrain/TerrainManager.cpp terrain/TerrainInfo.cpp
        terrain/TerrainLayerDefinitionManager.cpp terrain/TerrainMod.cpp
        terrain/TerrainPage.cpp terrain/TerrainPageGeometry.cpp
        terrain/TerrainPageSurface.cpp terrain/TerrainPageSurfaceCompiler.cpp
        terrain/TerrainPageSurfaceLayer.cpp terrain/TerrainShader.cpp terrain/XMLLayerDefinitionSerializer.cpp
        terrain/PlantAreaQueryResult.cpp terrain/TerrainParser.cpp terrain/TerrainPageCreationTask.cpp
        terrain/TerrainShaderUpdateTask.cpp terrain/TerrainAreaUpdateTask.cpp
        terrain/techniques/Shader.cpp
        terrain/techniques/ShaderPass.cpp
        terrain/techniques/ShaderPassBlendMapBatch.cpp terrain/techniques/Base.cpp
        terrain/Image.cpp terrain/OgreImage.cpp terrain/WFImage.cpp terrain/TerrainMaterialCompilationTask.cpp
        terrain/HeightMapSegment.cpp terrain/HeightMap.cpp terrain/Buffer.cpp terrain/HeightMapBuffer.cpp
        terrain/HeightMapBufferProvider.cpp terrain/HeightMapUpdateTask.cpp terrain/TerrainAreaTaskBase.cpp terrain/TerrainAreaAddTask.cpp
        terrain/TerrainAreaRemoveTask.cpp terrain/TerrainModUpdateTask.cpp
        terrain/GeometryUpdateTask.cpp terrain/TerrainEditorOverlay.cpp
        terrain/TerrainShaderParser.cpp terrain/TerrainUpdateTask.cpp terrain/PlantQueryTask.cpp
        terrain/HeightMapFlatSegment.cpp terrain/Segment.cpp terrain/SegmentHolder.cpp terrain/SegmentManager.cpp
        terrain/foliage/PlantPopulator.cpp terrain/foliage/ClusterPopulator.cpp terrain/foliage/Vegetation.cpp terrain/TerrainHandler.cpp
        terrain/techniques/CompilerTechniqueProvider.cpp terrain/ITerrainObserver.h terrain/TerrainPageDeletionTask.cpp
        terrain/techniques/OnePixelMaterialGenerator.cpp
        terrain/IHeightMapSegment.h terrain/ICompilerTechniqueProvider.h terrain/ITerrainAdapter.h terrain/ITerrainPageBridge.h terrain/PlantInstance.h terrain/Types.h

        Avatar.cpp AvatarCameraWarper.cpp
        MovementController.cpp AvatarLogger.cpp AvatarTerrainCursor.cpp
        EmberEntityActionCreator.cpp
        EmberEntityFactory.cpp EmberEntityHideModelAction.cpp EmberEntityModelAction.cpp
        EmberEntityPartAction.cpp EmberOgre.cpp EmberOgreFileSystem.cpp
        EntityWorldPickListener.cpp GUICEGUIAdapter.cpp GUIManager.cpp
        MediaUpdater.cpp MeshSerializerListener.cpp
        MotionManager.cpp OgreInfo.cpp OgreLogObserver.cpp OgreResourceLoader.cpp
        OgreResourceProvider.cpp OgreWindowProvider.cpp OgreSetup.cpp OgrePluginLoader.cpp NodeAttachment.cpp
        ShaderManager.cpp ShaderDetailManager.cpp ShadowCameraSetup.cpp ShadowDetailManager.cpp SimpleRenderContext.cpp RenderDistanceManager.cpp AutoGraphicsLevelManager.cpp
        XMLHelper.cpp WorldAttachment.cpp NodeController.cpp
        DelegatingNodeController.cpp AvatarAttachmentController.cpp HiddenAttachment.cpp
        AttachmentBase.cpp AvatarCameraMotionHandler.cpp FreeFlyingCameraMotionHandler.cpp SceneNodeProvider.cpp
        EntityObserverBase.cpp TerrainPageDataProvider.cpp Scene.cpp ForestRenderingTechnique.cpp World.cpp
        Screen.cpp ShapeVisual.cpp TerrainEntityManager.cpp CompositionAction.cpp GraphicalChangeAdapter.cpp
        EmberWorkQueue.cpp
        EmberOgrePrerequisites.h EmberOgreSignals.h Convert.h IAnimated.h ICameraMotionHandler.h ILightning.h
        IMovable.h IMovementProvider.h INodeProvider.h ISceneRenderingTechnique.h IWorldPickListener.h
        MousePicker.h OgreIncludes.h PresentAction.cpp PresentAction.h ModelActionBase.cpp ModelActionBase.h
        BulletCollisionDetector.cpp
        BulletWorld.cpp
        SkeletonDisplay.cpp
        ProjectileRenderingTechnique.cpp
        EmptyNodeAttachment.cpp
        )



file(GLOB ENTITY_RECIPES_FILES "${CMAKE_CURRENT_SOURCE_DIR}/authoring/entityrecipes/*.entityrecipe" "${CMAKE_CURRENT_SOURCE_DIR}/authoring/entityrecipes/*.xsd")
install(FILES ${ENTITY_RECIPES_FILES} DESTINATION ${CMAKE_INSTALL_FULL_DATADIR}/ember/entityrecipes)

wf_generate_lua_bindings(scripting/bindings/lua/EmberOgre)
wf_generate_lua_bindings(scripting/bindings/lua/ogre/Ogre)
wf_generate_lua_bindings(scripting/bindings/lua/helpers/Helpers scripting/bindings/lua/helpers/OgreUtils.cpp)


target_link_libraries(emberogre caelum meshtree pagedgeometry widgets)

target_compile_definitions(emberogre PUBLIC PLUGIN_SUFFIX="${CMAKE_SHARED_LIBRARY_SUFFIX}" PLUGIN_PREFIX="${CMAKE_SHARED_LIBRARY_PREFIX}")
add_subdirectory(widgets)