SUFFIXES: .cxx .pkg .lo .la .cpp .o .obj

.pkg.cxx:
	cd $(srcdir) && TOLUAXX=${TOLUAXX} $(abs_top_srcdir)/scripts/update_lua_bindings.sh `basename $@ .cxx` `basename $@ .cxx`.pkg $(abs_builddir)/`basename $@` $<

INCLUDES = -I$(top_srcdir)/src -I$(srcdir) -I$(top_builddir)/src -DPREFIX=\"@prefix@\"

noinst_LIBRARIES = liblua_Ogre.a
liblua_Ogre_a_SOURCES = Ogre.cxx

CLEANFILES = Ogre.cxx
TOLUA_PKGS = Ogre.pkg OgreAxisAlignedBox.pkg OgreColourValue.pkg OgreEntity.pkg OgreGpuProgram.pkg OgreGpuProgramManager.pkg OgreHighLevelGpuProgram.pkg \
 OgreHighLevelGpuProgramManager.pkg OgreIteratorWrappers.pkg OgreMaterial.pkg OgreMaterialManager.pkg OgreMaterialSerializer.pkg OgreMath.pkg OgreMesh.pkg \
 OgreMeshManager.pkg OgreNode.pkg OgreQuaternion.pkg OgreRenderTarget.pkg OgreRenderWindow.pkg OgreResource.pkg OgreResourceManager.pkg OgreRoot.pkg \
 OgreSceneNode.pkg OgreSubEntity.pkg OgreSubMesh.pkg OgreTexture.pkg OgreTextureManager.pkg OgreVector3.pkg
EXTRA_DIST = $(TOLUA_PKGS)
Ogre.cxx: $(TOLUA_PKGS)

noinst_HEADERS = required.h

bindings: clean all
