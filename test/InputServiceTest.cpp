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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "services/gui/GuiService.h"

#include "InputServiceTest.h"

namespace dime
{


void InputServiceTestCase::testGetInputDevice()
{
	CPPUNIT_ASSERT(myKD == static_cast<KeyboardDevice*>(myIS->getInputDevice(InputDevice::KEYBOARD)));
	CPPUNIT_ASSERT(myMD == static_cast<MouseDevice*>(myIS->getInputDevice(InputDevice::MOUSE)));
	
	InputService::InputDeviceIterator i = myIS->getDevices();

	RepetitionDevice * RD1 = static_cast<RepetitionDevice*>(myIS->getInputDevice(InputDevice::REPETITOR, &i));
	RepetitionDevice * RD2 = static_cast<RepetitionDevice*>(myIS->getInputDevice(InputDevice::REPETITOR, &i));
	RepetitionDevice * RD3 = static_cast<RepetitionDevice*>(myIS->getInputDevice(InputDevice::REPETITOR, &i));

	CPPUNIT_ASSERT(RD3 == NULL);

	CPPUNIT_ASSERT((myRD1 == RD1 && myRD2 == RD2) || (myRD2 == RD1 && myRD1 == RD2));
}

void InputServiceTestCase::testInputDevices()
{
	CPPUNIT_ASSERT(myMD->getAxisCount() == 2);
	CPPUNIT_ASSERT(myKD->getAxisCount() == 0);
	CPPUNIT_ASSERT(myRD1->getAxisCount() == 0);

	myMD->getAxisPosition(0);
	CPPUNIT_ASSERT(0.0 <= myMD->getAxisScaledPosition(1) && 
				   1.0 >= myMD->getAxisScaledPosition(1));
	
	CPPUNIT_ASSERT(myMD->getKeyState(SDLK_LEFT_MB) != InputDevice::UNSUPPORTED_KEY);
	CPPUNIT_ASSERT(myMD->getKeyState(SDLK_RETURN) == InputDevice::UNSUPPORTED_KEY);
	
	CPPUNIT_ASSERT(myKD->getKeyState(SDLK_CAPSLOCK) != InputDevice::UNSUPPORTED_KEY);
}

void InputServiceTestCase::testInputMappings()
{
	SDL_Event event;

	myEvents.clear();

	event.motion.type = SDL_MOUSEMOTION;
	event.motion.state = 0;
	event.motion.x = 5;
	event.motion.y = 10;
	event.motion.xrel = 5;
	event.motion.yrel = 10;

	myIS->handleEvent(event);
	myIS->handleEvent(event);

	CPPUNIT_ASSERT(myEvents.size() == 2);

	CPPUNIT_ASSERT(myEvents[0].myID == myMD);
	CPPUNIT_ASSERT(myEvents[1].myID == myMD);

	CPPUNIT_ASSERT(myEvents[0].myKey == SDLK_UNKNOWN);
	CPPUNIT_ASSERT(myEvents[1].myKey == SDLK_UNKNOWN);

	CPPUNIT_ASSERT(myEvents[0].myType == InputMapping::EVENT_OCCURED);
	CPPUNIT_ASSERT(myEvents[1].myType == InputMapping::EVENT_OCCURED);
}

void InputServiceTestCase::setUp ()
{
	myIS = InputService::getInstance();

	CPPUNIT_ASSERT (myIS != NULL);
	CPPUNIT_ASSERT (myIS->getName().compare("InputService") == 0);
	
	myKD = new KeyboardDevice();
	myMD = new MouseDevice();
	myRD1 = new RepetitionDevice(500, 0, true);
	myRD2 = new RepetitionDevice(2000, 0, true);

	myIS->addInputMapping(new InputMapping(myMD, 
								SigC::slot(*this, &InputServiceTestCase::eventWasFired)));
	
	myIS->addInputMapping(new InputMapping(myKD, SDLK_RETURN, KMOD_NONE, false, 
								SigC::slot(*this, &InputServiceTestCase::eventWasFired)));

	myIS->addInputMapping(new InputMapping(myKD, SDLK_DELETE, (SDLMod)KMOD_ALT, true, 
								SigC::slot(*this, &InputServiceTestCase::eventWasFired)));
	
	/*myIS->addInputMapping(new InputMapping(myRD2, myKD, SDLK_UP, KMOD_NONE, InputMapping::KEY_PRESSED | InputMapping::EVENT_OCCURED, 
								SigC::slot(*this, &InputServiceTestCase::eventWasFired)));*/

}

void InputServiceTestCase::tearDown()
{
	delete myKD;
	delete myMD;
	delete myRD1;
	delete myRD2;
}


void InputServiceTestCase::eventWasFired(InputDevice * device, 
										 InputDevice * device2, 
										 DimeKey const & key, 
										 InputMapping::InputSignalType type)
{
	InputEvent ie;

	ie.myID = device;
	ie.myKey = key.getKey();
	ie.myType = type;
	myEvents.push_back(ie);
}

}

#endif
