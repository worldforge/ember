INCLUDES = $(STLPORT_CFLAGS) -I$(top_srcdir)/src -I$(top_srcdir)/src/components/ogre/EmberSceneManager/include 

noinst_LIBRARIES = libEmberSceneManager.a

#pkglib_PROGRAMS = Plugin_EmberSceneManager.so

libEmberSceneManager_a_SOURCES = \
							OgreHeightmapTerrainPageSource.cpp \
							OgreOctreeSceneManagerDll.cpp \
                            OgreOctreeSceneManager.cpp \
							OgreOctreeSceneQuery.cpp \
                            OgreOctreeNode.cpp \
                            OgreOctreeCamera.cpp \
							OgreOctree.cpp \
							OgreTerrainPage.cpp \
							OgreTerrainPageSource.cpp \
							OgreTerrainRenderable.cpp \
							OgreTerrainSceneManager.cpp \
							OgreTerrainVertexProgram.cpp \
							EmberTerrainPageSource.cpp \
							EmberTerrainSceneManager.cpp
							


#libEmberSceneManager_a_LDFLAGS = $(SHARED_FLAGS) -L$(top_srcdir)/OgreMain/src
#libEmberSceneManager_a_LIBADD = -lOgreMain
