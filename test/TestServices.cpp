#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

#include "ConvertTestCase.h"
//#include "InputServiceTest.h"
//#include "StateManagerTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION( Ember::ConvertTestCase);
//CPPUNIT_TEST_SUITE_REGISTRATION( dime::InputServiceTestCase );
//CPPUNIT_TEST_SUITE_REGISTRATION( dime::StateManagerTestCase );

int main(int argc, char **argv)
{
	CppUnit::TextUi::TestRunner runner;
	CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();
	runner.addTest(registry.makeTest());
	bool wasSuccessful = runner.run("", false);
	return !wasSuccessful;
}

