AM_CPPFLAGS = -I$(top_srcdir)/src  -I$(top_builddir)/src -DPREFIX=\"@prefix@\"
AM_LDFLAGS = -lxerces-c -lode

#WARNING: use only absolute path in this file, because its included from webember, which has different working directory!

embersources = $(top_srcdir)/src/main/Application.cpp $(top_srcdir)/src/main/ConfigBoundLogObserver.cpp

if WIN32
embersources += $(top_srcdir)/src/main/win32/OgreWin32Resources.rc $(top_srcdir)/src/main/win32/ember.ico
.rc.o:
	$(WINDRES) -o $@ -i $<
endif

emberlibs = \
	$(top_builddir)/src/bindings/lua/liblua_ConnectorDefinitions.a \
	$(top_builddir)/src/components/lua/bindings/lua/liblua_Lua.a \
	$(top_builddir)/src/components/ogre/scripting/bindings/lua/helpers/liblua_Helpers.a \
	$(top_builddir)/src/components/ogre/scripting/bindings/lua/ogre/liblua_Ogre.a \
	$(top_builddir)/src/components/ogre/scripting/bindings/lua/liblua_EmberOgre.a \
	$(top_builddir)/src/components/ogre/widgets/adapters/atlas/bindings/lua/liblua_AtlasAdapters.a \
	$(top_builddir)/src/components/ogre/widgets/adapters/bindings/lua/liblua_Adapters.a \
	$(top_builddir)/src/components/ogre/widgets/representations/bindings/lua/liblua_Representations.a \
	$(top_builddir)/src/services/bindings/lua/liblua_EmberServices.a \
	$(top_builddir)/src/framework/bindings/lua/atlas/liblua_Atlas.a \
	$(top_builddir)/src/framework/bindings/lua/eris/liblua_Eris.a \
	$(top_builddir)/src/framework/bindings/lua/varconf/liblua_Varconf.a \
	$(top_builddir)/src/framework/bindings/lua/liblua_Framework.a \
	$(top_builddir)/src/domain/bindings/lua/liblua_Domain.a \
	$(top_builddir)/src/components/ogre/libEmberOgre.a \
	$(top_builddir)/src/components/ogre/environment/caelum/libCaelum.a \
	$(top_builddir)/src/components/ogre/environment/pagedgeometry/libpagedgeometry.a \
	$(top_builddir)/src/components/ogre/environment/meshtree/libMeshTree.a \
	$(top_builddir)/src/components/sb/libEmberSmartBody.a \
	$(top_builddir)/src/components/entitymapping/libEntityMapping.a \
	$(top_builddir)/src/components/cegui/bindings/lua/liblua_Cegui.a \
	$(top_builddir)/src/components/lua/libLua.a \
	$(top_builddir)/src/components/cegui/libCEGUI.a \
	$(top_builddir)/src/components/navigation/libnavigation.a \
	$(top_builddir)/src/components/navigation/external/RecastDetour/DetourTileCache/libDetourTileCache.a \
	$(top_builddir)/src/components/navigation/external/RecastDetour/Detour/libDetour.a \
	$(top_builddir)/src/components/navigation/external/RecastDetour/Recast/libRecast.a \
	$(top_builddir)/src/services/libServices.a \
	$(top_builddir)/src/services/config/libConfigService.a \
	$(top_builddir)/src/services/input/libInputService.a \
	$(top_builddir)/src/services/logging/libLoggingService.a \
	$(top_builddir)/src/services/metaserver/libMetaserverService.a \
	$(top_builddir)/src/services/scripting/libScriptingService.a \
	$(top_builddir)/src/services/server/libServerService.a \
	$(top_builddir)/src/services/sound/libSoundService.a \
	$(top_builddir)/src/services/wfut/libWfut.a \
	$(top_builddir)/src/services/serversettings/libServerSettings.a \
	$(top_builddir)/src/domain/libDomain.a \
	$(top_builddir)/src/framework/tasks/libTasks.a \
	$(top_builddir)/src/framework/libFramework.a \
	${BOOST_THREAD_LIB}

if !BUILD_WEBEMBER
bin_PROGRAMS = ember.bin

ember_bin_LDADD = $(emberlibs)
ember_bin_SOURCES = $(embersources) $(top_srcdir)/src/main/Ember.cpp

if OSX
ember_bin_SOURCES += $(top_srcdir)/src/main/macosx/SDLMain.m
endif

endif

noinst_HEADERS = $(top_srcdir)/src/main/Application.h $(top_srcdir)/src/main/ConfigBoundLogObserver.h $(top_srcdir)/src/main/MainWrapper.h


EXTRA_DIST=$(top_srcdir)/src/main/macosx/COPYING.SDL $(top_srcdir)/src/main/macosx/ReadMeDevLite.txt $(top_srcdir)/src/main/macosx/SDLMain.h
