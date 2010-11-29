INCLUDES = -I$(top_srcdir)/src  -I$(top_builddir)/src -DPREFIX=\"@prefix@\"

if USE_CPPUNIT
TESTS = TestOgreView TestTasks
check_PROGRAMS = $(TESTS)

TestOgreView_SOURCES = TestOgreView.cpp ConvertTestCase.cpp ModelMountTestCase.cpp
TestOgreView_CXXFLAGS = $(CPPUNIT_CFLAGS)
TestOgreView_LDFLAGS = $(CPPUNIT_LIBS)
TestOgreView_LDADD = $(top_builddir)/src/components/ogre/libEmberOgre.a \
	$(top_builddir)/src/components/entitymapping/libEntityMapping.a \
	$(top_builddir)/src/framework/libFramework.a
	
TestTasks_SOURCES = TestTasks.cpp
TestTasks_CXXFLAGS = $(CPPUNIT_CFLAGS)
TestTasks_LDFLAGS = $(CPPUNIT_LIBS)
TestTasks_LDADD = $(top_builddir)/src/framework/tasks/libTasks.a \
	$(top_builddir)/src/framework/libFramework.a
	
	
	


noinst_HEADERS = ConvertTestCase.h ModelMountTestCase.h
endif