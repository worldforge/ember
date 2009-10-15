INCLUDES = -I$(top_srcdir)/src  -I$(top_builddir)/src -DPREFIX=\"@prefix@\"
METASOURCES = AUTO
noinst_LIBRARIES = libTerrain.a
libTerrain_a_SOURCES terrain/= terrain/Map.cpp terrain/TerrainArea.cpp \
	terrain/TerrainAreaParser.cpp terrain/TerrainEditor.cpp terrain/ \
	terrain/TerrainGenerator.cpp terrain/TerrainInfo.cpp terrain/TerrainLayerDefinition.cpp \
	terrain/TerrainLayerDefinitionManager.cpp terrain/TerrainMod.cpp \
	terrain/TerrainPage.cpp terrain/TerrainPageFoliage.cpp terrain/TerrainPageGeometry.cpp \
	terrain/TerrainPageShadow.cpp terrain/TerrainPageSurface.cpp terrain/TerrainPageSurfaceCompiler.cpp \
	terrain/TerrainPageSurfaceCompilerTechniqueShader.cpp terrain/TerrainPageSurfaceCompilerTechniqueSimple.cpp \
	terrain/TerrainPageSurfaceLayer.cpp terrain/TerrainShader.cpp terrain/XMLLayerDefinitionSerializer.cpp terrain/PlantAreaQuery.cpp terrain/TerrainParser.cpp
noinst_HEADERS terrain/= terrain/ISceneManagerAdapter.h terrain/ITerrainPageBridge.h terrain/Map.h terrain/TerrainArea.h \
	terrain/TerrainAreaParser.h terrain/TerrainEditor.h terrain/TerrainGenerator.h terrain/TerrainInfo.h terrain/TerrainLayerDefinition.h terrain/TerrainLayerDefinitionManager.h \
	terrain/TerrainMod.h terrain/TerrainPage.h terrain/TerrainPageFoliage.h terrain/TerrainPageGeometry.h terrain/TerrainPageShadow.h terrain/TerrainPageSurface.h \
	terrain/TerrainPageSurfaceCompiler.h terrain/TerrainPageSurfaceCompilerTechniqueShader.h \
	terrain/TerrainPageSurfaceCompilerTechniqueSimple.h terrain/TerrainPageSurfaceLayer.h terrain/TerrainShader.h terrain/XMLLayerDefinitionSerializer.h terrain/PlantAreaQuery.h terrain/TerrainParser.h
