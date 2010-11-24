INCLUDES = -I$(top_srcdir)/src  -I$(top_builddir)/src -DPREFIX=\"@prefix@\"

if USE_CPPUNIT
TESTS = TestServices
check_PROGRAMS = $(TESTS)
TestServices_SOURCES = TestServices.cpp ConvertTestCase.cpp ModelMountTestCase.cpp

TestServices_CXXFLAGS = $(CPPUNIT_CFLAGS)
TestServices_LDFLAGS = $(CPPUNIT_LIBS)
TestServices_LDADD = $(top_builddir)/src/components/ogre/libEmberOgre.a \
	$(top_builddir)/src/components/entitymapping/libEntityMapping.a \
	$(top_builddir)/src/framework/libFramework.a


noinst_HEADERS = TestServices.h ConvertTestCase.h ModelMountTestCase.h
endif