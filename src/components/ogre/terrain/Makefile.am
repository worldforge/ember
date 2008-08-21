INCLUDES = -I$(top_srcdir)/src  -I$(top_builddir)/src -DPREFIX=\"@prefix@\"
METASOURCES = AUTO
noinst_LTLIBRARIES = libTerrain.la
libTerrain_la_SOURCES = TerrainArea.cpp TerrainArea.h TerrainEditor.cpp \
	TerrainEditor.cpp	TerrainEditor.h TerrainEditor.h TerrainGenerator.cpp TerrainGenerator.h TerrainInfo.cpp \
	TerrainLayerDefinition.cpp TerrainLayerDefinitionManager.cpp TerrainMod.h TerrainMod.cpp TerrainPage.cpp \
	TerrainPage.cpp	TerrainPage.h TerrainPage.h TerrainPageFoliage.cpp TerrainPageGeometry.cpp \
	TerrainPageShadow.cpp TerrainPageSurface.cpp TerrainPageSurfaceCompiler.cpp \
	TerrainPageSurfaceCompilerTechniqueShader.cpp TerrainPageSurfaceCompilerTechniqueSimple.cpp \
	TerrainPageSurfaceLayer.cpp TerrainShader.cpp TerrainShader.h XMLLayerDefinitionSerializer.cpp \
	Map.cpp
noinst_HEADERS = ISceneManagerAdapter.h ITerrainPageBridge.h TerrainInfo.h TerrainLayerDefinition.h \
	TerrainLayerDefinitionManager.h TerrainPageFoliage.h TerrainPageGeometry.h TerrainPageShadow.h \
	TerrainPageSurface.h TerrainPageSurfaceCompiler.h \
	TerrainPageSurfaceCompilerTechniqueShader.h TerrainPageSurfaceCompilerTechniqueSimple.h TerrainPageSurfaceLayer.h \
	XMLLayerDefinitionSerializer.h Map.h
