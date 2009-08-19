INCLUDES = -I$(top_srcdir)/src  -I$(top_builddir)/src -DPREFIX=\"@prefix@\"
METASOURCES = AUTO
noinst_LIBRARIES = libTerrain.a
libTerrain_a_SOURCES = Map.cpp TerrainArea.cpp TerrainArea.h \
	TerrainAreaParser.cpp TerrainEditor.cpp TerrainEditor.cpp	TerrainEditor.h TerrainEditor.h \
	TerrainGenerator.cpp TerrainGenerator.h TerrainInfo.cpp TerrainLayerDefinition.cpp \
	TerrainLayerDefinitionManager.cpp TerrainMod.cpp TerrainMod.h TerrainPage.cpp \
	TerrainPage.cpp	TerrainPage.h TerrainPage.h TerrainPageFoliage.cpp TerrainPageGeometry.cpp \
	TerrainPageShadow.cpp TerrainPageSurface.cpp TerrainPageSurfaceCompiler.cpp \
	TerrainPageSurfaceCompilerTechniqueShader.cpp TerrainPageSurfaceCompilerTechniqueSimple.cpp \
	TerrainPageSurfaceLayer.cpp TerrainShader.cpp TerrainShader.h XMLLayerDefinitionSerializer.cpp PlantAreaQuery.cpp TerrainParser.cpp
noinst_HEADERS = ISceneManagerAdapter.h ITerrainPageBridge.h Map.h \
	TerrainAreaParser.h TerrainInfo.h TerrainLayerDefinition.h TerrainLayerDefinitionManager.h \
	TerrainPageFoliage.h TerrainPageGeometry.h TerrainPageShadow.h TerrainPageSurface.h \
	TerrainPageSurfaceCompiler.h TerrainPageSurfaceCompilerTechniqueShader.h \
	TerrainPageSurfaceCompilerTechniqueSimple.h TerrainPageSurfaceLayer.h XMLLayerDefinitionSerializer.h PlantAreaQuery.h TerrainParser.h
