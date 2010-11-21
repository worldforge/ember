INCLUDES = -I$(top_srcdir)/src

if USE_CPPUNIT
TESTS = TestServices
check_PROGRAMS = $(TESTS)
TestServices_SOURCES = TestServices.cpp ConvertTestCase.cpp

TestServices_CXXFLAGS = $(CPPUNIT_CFLAGS)
TestServices_LDFLAGS = $(CPPUNIT_LIBS)


noinst_HEADERS = TestServices.h ConvertTestCase.h
endif