INCLUDES = -I$(top_srcdir)/src  -I$(top_builddir)/src -DPREFIX=\"@prefix@\"

bin_PROGRAMS = ember.bin

ember_bin_SOURCES = Application.cpp ConfigBoundLogObserver.cpp Ember.cpp

if WIN32
ember_bin_SOURCES += win32/OgreWin32Resources.rc win32/ember.ico
.rc.o:
	$(WINDRES) -o $@ -i $<
endif

ember_bin_LDADD = \
	$(top_builddir)/src/bindings/lua/liblua_ConnectorDefinitions.a \
	$(top_builddir)/src/components/lua/bindings/lua/liblua_Lua.a \
	$(top_builddir)/src/components/ogre/scripting/bindings/lua/helpers/liblua_Helpers.a \
	$(top_builddir)/src/components/ogre/scripting/bindings/lua/ogre/liblua_Ogre.a \
	$(top_builddir)/src/components/ogre/scripting/bindings/lua/liblua_EmberOgre.a \
	$(top_builddir)/src/components/ogre/widgets/adapters/atlas/bindings/lua/liblua_AtlasAdapters.a \
	$(top_builddir)/src/main/bindings/lua/liblua_Main.a \
	$(top_builddir)/src/services/bindings/lua/liblua_EmberServices.a \
	$(top_builddir)/src/framework/bindings/lua/atlas/liblua_Atlas.a \
	$(top_builddir)/src/framework/bindings/lua/eris/liblua_Eris.a \
	$(top_builddir)/src/framework/bindings/lua/varconf/liblua_Varconf.a \
	$(top_builddir)/src/framework/bindings/lua/liblua_Framework.a \
	$(top_builddir)/src/components/ogre/libEmberOgre.a \
	$(top_builddir)/src/components/ogre/SceneManagers/EmberPagingSceneManager/src/libEmberPagingSceneManager.a \
	$(top_builddir)/src/components/ogre/environment/caelum/libCaelum.a \
	$(top_builddir)/src/components/ogre/environment/pagedgeometry/libpagedgeometry.a \
	$(top_builddir)/src/components/ogre/environment/meshtree/libMeshTree.a \
	$(top_builddir)/src/components/entitymapping/libEntityMapping.a \
	$(top_builddir)/src/components/lua/libLua.a \
	$(top_builddir)/src/services/libServices.a \
	$(top_builddir)/src/services/config/libConfigService.a \
	$(top_builddir)/src/services/input/libInputService.a \
	$(top_builddir)/src/services/logging/libLoggingService.a \
	$(top_builddir)/src/services/metaserver/libMetaserverService.a \
	$(top_builddir)/src/services/scripting/libScriptingService.a \
	$(top_builddir)/src/services/server/libServerService.a \
	$(top_builddir)/src/services/sound/libSoundService.a \
	$(top_builddir)/src/services/time/libTime.a \
	$(top_builddir)/src/services/wfut/libWfut.a \
	$(top_builddir)/src/services/serversettings/libServerSettings.a \
	$(top_builddir)/src/extensions/libExtensions.a \
	$(top_builddir)/src/framework/tasks/libTasks.a \
	$(top_builddir)/src/framework/libFramework.a
ember_bin_LDFLAGS = -Wl

noinst_HEADERS = Application.h ConfigBoundLogObserver.h
SUBDIRS = bindings

EXTRA_DIST=macosx/COPYING.SDL macosx/ReadMeDevLite.txt macosx/SDLMain.h macosx/SDLMain.m
