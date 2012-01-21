SUFFIXES: .cxx .pkg .lo .la .cpp .o .obj

#Replace "float" in the pkg-files with the actual type of Ogre::Real (which will be "float" unless you've built Ogre with double precision).
#Note that we cannot use "Ogre::Real" in the pkg-files, since tolua++ would then think it to be a class or struct and handle it incorrectly.
.pkg.cxx:
	cd $(srcdir) && TOLUAXX=${TOLUAXX} $(abs_top_srcdir)/scripts/update_lua_bindings.sh `basename $@ .cxx` `basename $@ .cxx`.pkg $(abs_builddir)/`basename $@` $< && sed -i "s/float/@OGRE_REAL_TYPE@/g" $(abs_builddir)/$@

INCLUDES = -I$(top_srcdir)/src -I$(srcdir) -I$(top_builddir)/src -DPREFIX=\"@prefix@\"

noinst_LIBRARIES = liblua_Ogre.a
liblua_Ogre_a_SOURCES = Ogre.cxx

CLEANFILES = Ogre.cxx
TOLUA_PKGS = Ogre.pkg OgreAxisAlignedBox.pkg OgreColourValue.pkg OgreEntity.pkg OgreGpuProgram.pkg OgreGpuProgramManager.pkg OgreHighLevelGpuProgram.pkg \
 OgreHighLevelGpuProgramManager.pkg OgreIteratorWrappers.pkg OgreMaterial.pkg OgreMaterialManager.pkg OgreMaterialSerializer.pkg OgreMath.pkg OgreMesh.pkg \
 OgreMeshManager.pkg OgreNode.pkg OgreQuaternion.pkg OgreRenderTarget.pkg OgreRenderWindow.pkg OgreResource.pkg OgreResourceManager.pkg OgreRoot.pkg \
 OgreSceneNode.pkg OgreSubEntity.pkg OgreSubMesh.pkg OgreTexture.pkg OgreTextureManager.pkg OgreVector3.pkg OgreSceneManager.pkg
EXTRA_DIST = $(TOLUA_PKGS)
Ogre.cxx: $(TOLUA_PKGS)

noinst_HEADERS = required.h

bindings: clean all
