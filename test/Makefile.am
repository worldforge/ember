# Run 'make check' to build and invoke test program

#point to uninstalled Dime headers
INCLUDES = '-I$(top_srcdir)/src'

if USE_CPPUNIT
TESTPROGS = testsuite
TESTSRC = TestSuite.cpp DataModelTest.cpp 
TESTLIST = testsuite
else
TESTPROGS = 
TESTSRC = 
TESTLIST = nulltest.sh
endif

check_PROGRAMS = $(TESTPROGS)

testsuite_SOURCES = $(TESTSRC)
testsuite_LDFLAGS = '-L$(top_builddir)/src'
testsuite_LDADD = -lcppunit $(top_srcdir)/src/services/datamodel/libDataModelService.a
# Rebuild testsuite executable if libcoal changes
testsuite_DEPENDENCIES = $(top_builddir)/src/services/datamodel/libDataModelService.a


TESTS = $(TESTLIST)
