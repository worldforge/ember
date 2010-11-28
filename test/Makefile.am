INCLUDES = -I$(top_srcdir)/src  -I$(top_builddir)/src -DPREFIX=\"@prefix@\"

if USE_CPPUNIT
TESTS = TestOgreView
check_PROGRAMS = $(TESTS)
TestOgreView_SOURCES = TestOgreView.cpp ConvertTestCase.cpp ModelMountTestCase.cpp

TestOgreView_CXXFLAGS = $(CPPUNIT_CFLAGS)
TestOgreView_LDFLAGS = $(CPPUNIT_LIBS)
TestOgreView_LDADD = $(top_builddir)/src/components/ogre/libEmberOgre.a \
	$(top_builddir)/src/components/entitymapping/libEntityMapping.a \
	$(top_builddir)/src/framework/libFramework.a


noinst_HEADERS = TestOgreView.h ConvertTestCase.h ModelMountTestCase.h
endif