INCLUDES = -I$(top_srcdir)/src  -I$(top_builddir)/src -DPREFIX=\"@prefix@\"

bin_PROGRAMS = ember.bin

ember_bin_SOURCES = Application.cpp ConfigBoundLogObserver.cpp Ember.cpp

ember_bin_LDADD = \
	$(top_builddir)/src/components/ogre/SceneManagers/EmberPagingSceneManager/src/libEmberPagingSceneManager.a \
	$(top_builddir)/src/components/ogre/environment/caelum/libCaelum.a \
	$(top_builddir)/src/components/ogre/environment/pagedgeometry/libpagedgeometry.a \
	$(top_builddir)/src/components/ogre/environment/meshtree/libMeshTree.a \
	$(top_builddir)/src/components/ogre/environment/libEnvironment.a \
	$(top_builddir)/src/components/ogre/gui/libGui.a \
	$(top_builddir)/src/components/ogre/manipulation/libmanipulation.a \
	$(top_builddir)/src/components/ogre/model/libModel.a \
	$(top_builddir)/src/components/ogre/scripting/bindings/lua/helpers/liblua_Helpers.a \
	$(top_builddir)/src/components/ogre/scripting/bindings/lua/ogre/liblua_Ogre.a \
	$(top_builddir)/src/components/ogre/scripting/bindings/lua/liblua_EmberOgre.a \
	$(top_builddir)/src/components/ogre/scripting/libScripting.a \
	$(top_builddir)/src/components/ogre/sound/libSound.a \
	$(top_builddir)/src/components/ogre/terrain/libTerrain.a \
	$(top_builddir)/src/components/ogre/widgets/adapters/atlas/libAtlas.a \
	$(top_builddir)/src/components/ogre/widgets/adapters/libAdapters.a \
	$(top_builddir)/src/components/ogre/widgets/icons/libIcons.a \
	$(top_builddir)/src/components/ogre/widgets/libWidgets.a \
	$(top_builddir)/src/components/ogre/mapping/libMapping.a \
	$(top_builddir)/src/components/ogre/camera/libCamera.a \
	$(top_builddir)/src/components/ogre/libEmberOgre.a \
	$(top_builddir)/src/components/entitymapping/libEntityMapping.a \
	$(top_builddir)/src/extensions/libExtensions.a \
	$(top_builddir)/src/framework/bindings/lua/atlas/liblua_Atlas.a \
	$(top_builddir)/src/framework/bindings/lua/eris/liblua_Eris.a \
	$(top_builddir)/src/framework/bindings/lua/varconf/liblua_Varconf.a \
	$(top_builddir)/src/framework/bindings/lua/liblua_Framework.a \
	$(top_builddir)/src/framework/tasks/libTasks.a \
	$(top_builddir)/src/framework/libFramework.a \
	$(top_builddir)/src/main/bindings/lua/liblua_Main.a \
	$(top_builddir)/src/services/bindings/lua/liblua_EmberServices.a \
	$(top_builddir)/src/services/config/libConfigService.a \
	$(top_builddir)/src/services/input/libInputService.a \
	$(top_builddir)/src/services/logging/libLoggingService.a \
	$(top_builddir)/src/services/metaserver/libMetaserverService.a \
	$(top_builddir)/src/services/scripting/libScriptingService.a \
	$(top_builddir)/src/services/server/libServerService.a \
	$(top_builddir)/src/services/sound/libSoundService.a \
	$(top_builddir)/src/services/time/libTime.a \
	$(top_builddir)/src/services/wfut/libWfut.a \
	$(top_builddir)/src/services/libServices.a
ember_bin_LDFLAGS = -Wl,--start-group

noinst_HEADERS = Application.h ConfigBoundLogObserver.h
SUBDIRS = bindings

EXTRA_DIST=macosx/COPYING.SDL macosx/ReadMeDevLite.txt macosx/SDLMain.h macosx/SDLMain.m
