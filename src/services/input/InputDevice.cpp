/*
    Copyright (C) 2002  Tim Enderling

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

#include "InputDevice.h"
#include "InputMapping.h"
#include "InputService.h"

namespace dime
{
 
InputDevice::InputDevice(InputDevice::DeviceType type, int index, int subIndex)
{
	myType		= type;
	myIndex		= index;
	mySubIndex	= subIndex;

	//automatic registration
	
	InputService * inputService = InputService::getInstance();
	inputService->myInputDevices.push_back(this);
}

InputDevice::~InputDevice()
{
	InputService * inputService = InputService::getInstance();

	//all mappings have to be removed prior to deleting this input device
	while (!myMotionMappings.empty())
	{
		inputService->removeInputMapping(*myMotionMappings.begin());		
	}

	while (!myKeyMappings.empty())
	{
		inputService->removeInputMapping(*myKeyMappings.begin());		
	}

	InputService::InputDeviceIterator i2;

	for (i2 = inputService->myInputDevices.begin(); i2 != inputService->myInputDevices.end(); i2++)
	{
		if (*i2 == this)
		{
			inputService->myInputDevices.erase(i2);
			break;
		}
	}
}

InputDevice::DeviceType InputDevice::getType()
{
	return myType;
}

int InputDevice::getAxisCount()
{
	return myPhysicalPosition.size();
}

int InputDevice::getAxisPosition(int axis)
{
	return myPhysicalPosition[axis];
}

float InputDevice::getAxisScaledPosition(int axis)
{
	return ((float)(myPhysicalPosition[axis]-myPhysicalMin[axis]) /
			((float)(myPhysicalMax[axis]-myPhysicalMin[axis])));
}

void InputDevice::initAxis()
{
	//default implementation is a device with no axis	
}


InputDevice::KeyState InputDevice::getKeyState(SDLKey key)
{
	//default implementation is a device with no keys
	return UNSUPPORTED_KEY;
}

void InputDevice::keyPressed(DimeKey & key)
{
	SDLMod modifiers = SDL_GetModState();

	for (MappingIterator i = myKeyMappings.begin(); i != myKeyMappings.end(); i++)
	{
		(*i)->keyPressed(key, modifiers);
	}
}

void InputDevice::keyReleased(DimeKey & key)
{
	for (MappingIterator i = myKeyMappings.begin(); i != myKeyMappings.end(); i++)
	{
		(*i)->keyReleased(key);
	}
}

void InputDevice::motion()
{
	for (MappingIterator i = myMotionMappings.begin(); i != myMotionMappings.end(); i++)
	{
		(*i)->motion();
	}
}

KeyboardDevice::KeyboardDevice():
	  InputDevice(KEYBOARD, 0, 0)
{
	Uint8 * keys = SDL_GetKeyState(&myKeyCount);

	myKeys = new KeyState[myKeyCount];

	for (int i = 0; i < myKeyCount; i++)
	{
		//TODO: Test if the key is actually supported. Supported by SDL?

		if (keys[i] == 1)
		{
			myKeys[i] = PRESSED;
		}
		else
		{
			myKeys[i] = RELEASED;
		}
	}
}

KeyboardDevice::~KeyboardDevice()
{
	delete myKeys;
}

InputDevice::KeyState KeyboardDevice::getKeyState(SDLKey key)
{
	if (static_cast<int>(key) < myKeyCount &&
		static_cast<int>(key) > 0)
	{
		return myKeys[static_cast<int>(key)];
	}

	return UNSUPPORTED_KEY;
}

bool KeyboardDevice::handleEvent(SDL_Event & event)
{
    DimeKey key(event.key.keysym.sym, event.key.keysym.unicode);
	switch(event.type)
	{
	case SDL_KEYDOWN:
		myKeys[static_cast<int>(event.key.keysym.sym)] = PRESSED;
		keyPressed(key);
		return true;

	case SDL_KEYUP:
		myKeys[static_cast<int>(event.key.keysym.sym)] = RELEASED;
		keyReleased(key);
		return true;

	default:
		return false;
	}
}

MouseDevice::MouseDevice():
		  InputDevice(MOUSE, 0, 0)
{
	//TODO: Really test if the mouse has three buttons
	myHasThreeButtons = true;
	
	initAxis();
}

MouseDevice::~MouseDevice()
{
}

InputDevice::KeyState MouseDevice::getKeyState(SDLKey key)
{
	Uint8 state = SDL_GetMouseState(NULL, NULL);

	switch(key)
	{
	case SDLK_LEFT_MB:
		return (SDL_BUTTON_LMASK & state) ? PRESSED : RELEASED;
		break;

	case SDLK_RIGHT_MB:
		return (SDL_BUTTON_RMASK & state) ? PRESSED : RELEASED;
		break;

	case SDLK_MIDDLE_MB:
		return myHasThreeButtons ? ( (SDL_BUTTON_MMASK & state) ? PRESSED : RELEASED) : UNSUPPORTED_KEY;
		break;

	default:
		return UNSUPPORTED_KEY;
	}
}
	
void MouseDevice::initAxis()
{
	//HINT: If at some day mouse wheel supports get's available, consider carefully if
	//to put it here (think of motion events)! I would prefer to make an extra device type.

	//mouse has two axis
	//the minimum of both is 0
	//the maximum of both is the screen resolution

	myPhysicalMin.resize(2, 0);
	myPhysicalMax.resize(2, 0);
	myPhysicalPosition.resize(2, 0);
	
	myPhysicalMin[0] = 0;
	myPhysicalMax[1] = 0;
	
	SDL_Surface * surf = SDL_GetVideoSurface();

	if (surf)
	{
		myPhysicalMax[0] = surf->w;
		myPhysicalMax[1] = surf->h;
	}
	else
	{
		myPhysicalMax[0] = 800;
		myPhysicalMax[1] = 600;
	}

	int x, y;

	SDL_GetMouseState(&x, &y);

	myPhysicalPosition[0] = x;
	myPhysicalPosition[1] = y;
}
	
bool MouseDevice::handleEvent(SDL_Event & event)
{
	DimeKey key(static_cast<SDLKey>(event.button.button),static_cast<Uint16>(event.button.button));

	switch (event.type)
	{
	case SDL_MOUSEMOTION:
		//TODO: Test if this works properly also, if the cursor is hidden
		//esp. concerning if the mouse movements are cut at the screen borders

		myPhysicalPosition[0] = event.motion.x;
		myPhysicalPosition[1] = event.motion.y;
		
		motion();

		return true;

	case SDL_MOUSEBUTTONDOWN:
		keyPressed(key);
		return true;
	

	case SDL_MOUSEBUTTONUP:	
		keyReleased(key);
		return true;
	
	case SDL_VIDEORESIZE:
		initAxis();
		return false;
	
	default:
		return false;
	}
}

RepetitionDevice::RepetitionDevice(long unsigned int initialDelay, long unsigned int delay,
								   bool alwaysRunning):
	InputDevice(REPETITOR, 0, 0)
{
	myInitialDelay = initialDelay;
	myDelay = delay;	
	myTimerID = 0;

	if (alwaysRunning)
	{
		myTimerID = SDL_AddTimer(delay, TimerCallback, static_cast<void*>(this));	
	}

}

RepetitionDevice::~RepetitionDevice()
{
	if (myTimerID)
	{
		SDL_RemoveTimer(myTimerID);
	}
}

Uint32 RepetitionDevice::TimerCallback(Uint32 interval, void *param)
{
	SDL_Event event;

	event.type = TIMEREVENT;
	event.user.data1 = param;

	SDL_PushEvent(&event);

	return ((RepetitionDevice*)param)->myDelay;
}

void RepetitionDevice::switchOn(InputDevice * motionDevice, InputDevice * keyDevice, 
				const DimeKey & Key, InputMapping::InputSignalType type)
{

	if (type == InputMapping::KEY_PRESSED)
	{
		if (myTimerID)
		{
			SDL_RemoveTimer(myTimerID);
			myTimerID = 0;
		}

		myTimerID = SDL_AddTimer(myInitialDelay, TimerCallback, static_cast<void*>(this));	
	}
}

long unsigned int RepetitionDevice::getDelay()
{
	return myDelay;
}

bool RepetitionDevice::handleEvent(SDL_Event & event)
{
	switch(event.type)
	{
	case TIMEREVENT:
		//HINT: Repetition events should be handled by the respective owner only, to ensure
		//that the RepetitionDevice wasn't deleted in the meantime.

		if (event.user.data1 == (void*)this) 
		{
			motion();
			return true;
		}

		return false;

	default:
		return false;
	}
}

} // namespace dime

