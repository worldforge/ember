#ifdef USE_CPP_UNIT

#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>
#include "main/DimeServices.h"
#include "services/input/InputService.h"

namespace dime
{

/* 
 * A test case that is designed to produce
 * example errors and failures
 *
 */

class InputServiceTestCase : public CppUnit::TestCase, public SigC::Object 
{
  struct InputEvent
  {
	InputDevice * myID;
	SDLKey		  myKey;
	InputMapping::InputSignalType myType;
  };

  CPPUNIT_TEST_SUITE( InputServiceTestCase );
  CPPUNIT_TEST( testGetInputDevice );
  CPPUNIT_TEST( testInputDevices );
  CPPUNIT_TEST( testInputMappings );
  CPPUNIT_TEST_SUITE_END();
protected:
	InputService * myIS;
	KeyboardDevice * myKD;
	MouseDevice *	 myMD;
	RepetitionDevice* myRD1;
	RepetitionDevice* myRD2;

	std::vector<InputEvent>	  myEvents; //InputDevice not identifier!	

public:
	void			setUp ();
	void			tearDown();
	
protected:
	void			testGetInputDevice();
	void			testInputDevices();
	void			testInputMappings();
	void			eventWasFired(InputDevice *, InputDevice *,
				DimeKey const &, InputMapping::InputSignalType);

	
};

}
#endif

