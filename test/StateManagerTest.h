#ifdef USE_CPP_UNIT

#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>
#include "main/StateManager.h"
#include <sigc++/signal_system.h>

namespace dime
{

/* 
 * A test case that is designed to produce
 * example errors and failures
 *
 */

class StateManagerTestCase : public CppUnit::TestCase, public SigC::Object 
{

  CPPUNIT_TEST_SUITE( StateManagerTestCase );
  CPPUNIT_TEST( testFindStateNonExistant );
  CPPUNIT_TEST( testFindStateLegal );
  CPPUNIT_TEST_SUITE_END();
protected:
	StateManager * mySM;

public:
	void			setUp ();
	void			tearDown();
	
protected:
	void			testFindStateNonExistant();
	void			testFindStateLegal();


	
};

}
#endif
