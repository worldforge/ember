/*
    Copyright (C) 2002 Tim Enderling

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifdef USE_CPP_UNIT

#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>
#include "services/DimeServices.h"
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
