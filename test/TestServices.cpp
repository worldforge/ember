#ifdef USE_CPP_UNIT
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

#include "DataModelTest.h"


bool runTests() 
{
    CppUnit::TextUi::TestRunner runner;
    CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();
    runner.addTest( registry.makeTest() );
    bool wasSucessful = runner.run( "", false );
    return wasSucessful;
}

#else

bool runTests()
{
    return true;
}

#endif
