// Master driver for COAL Library test suites

#include "DataModelTest.h"
#include <cppunit/TextTestResult.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <iostream>

// Subclass display class to customize test output
class CoalTestResult : public CppUnit::TextTestResult
{
    virtual void addError (CppUnit::Test *test, 
                           CppUnit::Exception *e);
    virtual void addFailure (CppUnit::Test *test, CppUnit::Exception *e);
    virtual void startTest (CppUnit::Test *test);
    virtual void endTest (CppUnit::Test *test);
};


void CoalTestResult::addError (CppUnit::Test *test, CppUnit::Exception *e)
{
    TestResult::addError (test, e);
    std::cout << std::endl << std::endl
              << "\t*** ERROR! ***" << std::endl;
}

void CoalTestResult::addFailure (CppUnit::Test *test, CppUnit::Exception *e)
{
    TestResult::addFailure (test, e);
	std::cout << std::endl << std::endl
			  << e->sourceLine().fileName() << ":"
		          << e->sourceLine().lineNumber()
              << "\t*** FAILURE!! ***" << std::endl;
}

void CoalTestResult::startTest (CppUnit::Test *test)
{
    std::cout << "Running " << test->getName() << "";
    TestResult::startTest (test);
    std::cout << "...";
}

void CoalTestResult::endTest (CppUnit::Test *test)
{
  TestResult::endTest (test);
  std::cout << std::endl;
}

int main (int argc, char **argv)
{
    CppUnit::TestSuite suite;
    CoalTestResult res;

    dime::DataModelTestCase myDMtestcase("DataModelTestCase");
    myDMtestcase.registerTests(suite);
    
    // Report results
    std::cout << std::endl << "*** BEGIN UNIT TESTS ***" <<
        std::endl << std::endl;
    suite.run (&res);
    std::cout << res << std::endl;

    std::cout << "*** UNIT TESTS COMPLETED ***" << std::endl << std::endl;

    // Pass on results to external shell.  This will cause 'make distcheck'
    // to fail if any unit tests fail (but 'make dist' will pass, of course).
    return (!res.wasSuccessful());
}
