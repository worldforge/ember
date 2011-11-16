INCLUDES = -I$(top_srcdir)/src  -I$(top_builddir)/src -DPREFIX=\"@prefix@\"

if USE_CPPUNIT
TESTS = TestOgreView TestTasks TestTerrain
check_PROGRAMS = $(TESTS)
CLEANFILES = Ogre.log

TestOgreView_SOURCES = TestOgreView.cpp ConvertTestCase.cpp ModelMountTestCase.cpp
TestOgreView_CXXFLAGS = $(CPPUNIT_CFLAGS)
TestOgreView_LDFLAGS = $(CPPUNIT_LIBS)
TestOgreView_LDADD = $(top_builddir)/src/components/ogre/libEmberOgre.a \
	$(top_builddir)/src/components/entitymapping/libEntityMapping.a \
	$(top_builddir)/src/framework/libFramework.a
	
TestTasks_SOURCES = TestTasks.cpp
TestTasks_CXXFLAGS = $(CPPUNIT_CFLAGS) -DLOG_TASKS
TestTasks_LDFLAGS = $(CPPUNIT_LIBS)
TestTasks_LDADD = $(top_builddir)/src/framework/tasks/libTasks.a \
	$(top_builddir)/src/framework/libFramework.a
	
TestTerrain_SOURCES = TestTerrain.cpp
TestTerrain_CXXFLAGS = $(CPPUNIT_CFLAGS) -DLOG_TASKS
TestTerrain_LDFLAGS = $(CPPUNIT_LIBS)
TestTerrain_LDADD = $(top_builddir)/src/components/ogre/libEmberOgre.a \
	$(top_builddir)/src/components/ogre/SceneManagers/EmberPagingSceneManager/src/libEmberPagingSceneManager.a \
	$(top_builddir)/src/components/ogre/environment/caelum/libCaelum.a \
	$(top_builddir)/src/components/ogre/environment/pagedgeometry/libpagedgeometry.a \
	$(top_builddir)/src/components/ogre/environment/meshtree/libMeshTree.a \
	$(top_builddir)/src/components/entitymapping/libEntityMapping.a \
	$(top_builddir)/src/components/lua/libLua.a \
	$(top_builddir)/src/services/libServices.a \
	$(top_builddir)/src/services/input/libInputService.a \
	$(top_builddir)/src/services/config/libConfigService.a \
	$(top_builddir)/src/services/logging/libLoggingService.a \
	$(top_builddir)/src/services/metaserver/libMetaserverService.a \
	$(top_builddir)/src/services/scripting/libScriptingService.a \
	$(top_builddir)/src/services/server/libServerService.a \
	$(top_builddir)/src/services/sound/libSoundService.a \
	$(top_builddir)/src/services/time/libTime.a \
	$(top_builddir)/src/services/wfut/libWfut.a \
	$(top_builddir)/src/services/serversettings/libServerSettings.a \
	$(top_builddir)/src/framework/tasks/libTasks.a \
	$(top_builddir)/src/framework/libFramework.a
	


noinst_HEADERS = ConvertTestCase.h ModelMountTestCase.h
endif
