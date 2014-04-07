SUBDIRS =  ogre helpers

SUFFIXES: .cxx .pkg .lo .la .cpp .o .obj

.pkg.cxx:
	cd $(srcdir) && TOLUAXX=${TOLUAXX} $(abs_top_srcdir)/scripts/update_lua_bindings.sh `basename $@ .cxx` `basename $@ .cxx`.pkg $(abs_builddir)/`basename $@` $<

AM_CPPFLAGS = -I$(top_srcdir)/src -I$(srcdir) -I$(top_builddir)/src -DPREFIX=\"@prefix@\"

noinst_LIBRARIES = liblua_EmberOgre.a
#if CHECK_BINDINGS
#BUILT_SOURCES = EmberOgre.cxx
#endif
liblua_EmberOgre_a_SOURCES = EmberOgre.cxx

CLEANFILES = EmberOgre.cxx
TOLUA_PKGS = ActionBar.pkg ActionBarIcon.pkg ActionBarIconDragDropTarget.pkg ActionBarIconSlot.pkg ActionBarIconManager.pkg AssetsManager.pkg AtlasHelper.pkg AuthoringManager.pkg Avatar.pkg ColouredListItem.pkg Compass.pkg \
 ConsoleAdapter.pkg Convert.pkg EmberEntityFactory.pkg EmberOgre.pkg EntityCEGUITexture.pkg EntityCreator.pkg EntityEditor.pkg EntityIcon.pkg \
  EntityIconDragDropTarget.pkg EntityIconDragDropPreview.pkg EntityIconManager.pkg EntityIconSlot.pkg EntityMoveManager.pkg EntityRecipe.pkg EntityRecipeManager.pkg EntityWorldPickListener.pkg \
  Environment.pkg GUIAdapter.pkg GUIManager.pkg Icon.pkg IconBar.pkg IconBase.pkg IconManager.pkg ITerrainAdapter.pkg \
  IWorldPickListener.pkg ListHolder.pkg MainCamera.pkg Map.pkg MaterialPicker.pkg Model.pkg ModelDefinition.pkg ModelDefinitionAtlasComposer.pkg \
  ModelDefinitionManager.pkg ModelRenderer.pkg ModelRepresentation.pkg ModelRepresentationManager.pkg MotionManager.pkg MousePicker.pkg MovableObjectRenderer.pkg \
  MovementController.pkg OgreEntityRenderer.pkg OgreInfo.pkg QuaternionAdapter.pkg QuickHelpCursor.pkg ResourceListAdapter.pkg sigc.pkg SimpleRenderContext.pkg \
  StackableContainer.pkg stdlib.pkg SubModel.pkg TerrainArea.pkg TerrainEditor.pkg TerrainLayerDefinition.pkg TerrainLayerDefinitionManager.pkg TerrainManager.pkg \
  RuleTreeAdapter.pkg Vector3Adapter.pkg Widget.pkg World.pkg Scene.pkg EntityTooltip.pkg TerrainHandler.pkg EntityCreatorTypeHelper.pkg Screen.pkg ModelEditHelper.pkg \
  EntityTextureManipulator.pkg RuleEditor.pkg LodDefinitionManager.pkg LodDefinition.pkg LodManager.pkg MeshInfoProvider.pkg PMInjectorSignaler.pkg
EXTRA_DIST = $(TOLUA_PKGS)
EmberOgre.cxx: $(TOLUA_PKGS)
noinst_HEADERS = GUIManager_helper.h

bindings: clean all

check:
	@set -e; cd $(srcdir) && for filename in *.pkg ; do \
		if [[ "$(TOLUA_PKGS)" != *$${filename}* ]] ;\
		then \
			echo "The file $${filename} exists in $(srcdir) but is not referenced in the Makefile.am. Either remove it if it's not used, or update the Makefile.am"; \
		  	exit 1; \
		fi \
	done;
