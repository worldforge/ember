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

namespace dime
{
 
InputDevice::InputDevice(DeviceType type, int index, int subIndex)
{
	myType		= type;
	myIndex		= sdlIndex;
	mySubIndex	= sdlSubIndex;

	//How to retrieve the singleton of InputService?
	//TODO:  - registration

	initAxis();
}

InputDevice::~InputDevice()
{
	//TODO
}

DeviceType InputDevice::getType()
{
	return myType;
}

int InputDevice::getAxisCount()
{
	return myPosition.size();
}

int InputDevice::getAxisPosition(int axis)
{
	return myPosition[axis];
}

float InputDevice::getAxisScaledPosition(int axis)
{
	return ((float)(myPosition[axis]-myPhysicalMin[axis]) /
			((float)(myPhysicalMax[axis]-myPhysicalMin[axis])));
}

KeyState InputDevice::getKeyState(SDLKey key)
{
	//default implementation is a device with no keys
	return UNSUPPORTED_KEY;
}

void InputDevice::keyPressed(SDLKey & key)
{
	SDLMod modifiers = SDL_GetModState();

	for (MappingIterator i = myKeyMappings.begin(); i != myKeyMappings.end(); i++)
	{
		(*i)->keyPressed(key, modifiers);
	}
}

void InputDevice::keyReleased(SDLKey & key)
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
		(*i)->motion(key);
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

	delete keys;
}

KeyboardDevice::~KeyboardDevice()
{
	delete myKeys;
}

KeyboardDevice::getKeyState(SDLKey key)
{
	if (static_cast<int>(SDLKey) < myKeyCount &&
		static_cast<int>(SDLKey) > 0)
	{
		return myKeys[static_cast<int>(key)];
	}

	return UNSUPPORTED_KEY;
}

BOOL KeyboardDevice::handleEvent(SDL_Event & event)
{
	switch(event.type)
	{
	SDL_KEYDOWN:
		myKeys[static_cast<int>(event.key.keysim.sim)] = PRESSED;
		keyPressed(event.key.keysim.sim);
		return TRUE;
	
	SDL_KEYUP:
		myKeys[static_cast<int>(event.key.keysim.sim)] = RELEASED;
		keyReleased(event.key.keysim.sim);
		return TRUE;

	default:
		return FALSE;
	}
}

MouseDevice::MouseDevice():
		  InputDevice(MOUSE, 0, 0)
{
	//TODO: Really test if the mouse has three buttons
	myHasThreeButtons = TRUE;
}

MouseDevice::~MouseDevice()
{
}

KeyState MouseDevice::getKeyState(SDLKey key)
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
	
	myPhysicalMax[0] = SDL_GetVideoSurface()->w;
	myPhysicalMax[1] = SDL_GetVideoSurface()->h;

	int x, y;

	SDL_GetMouseState(&x, &y);

	setPhysicalPosition[0] = x;
	setPhysicalPosition[1] = y;
}
	
bool MouseDevice::handleEvent(SDL_Event & event)
{
	switch (event.type)
	{
	case SDL_MOUSEMOTION:
		//TODO: Test if this works properly also, if the cursor is hidden
		//esp. concerning if the mouse movements are cut at the screen borders

		setPhysicalPosition[0] = event.motion.x;
		setPhysicalPosition[1] = event.motion.y;
		
		motion();

		return TRUE;

	case SDL_MOUSEBUTTONDOWN:
		keyPressed(static_cast<SDLKey>(event.button.button));
		return TRUE;
	

	case SDL_MOUSEBUTTONUP:
		keyReleased(static_cast<SDLKey>(event.button.button));
		return TRUE;
	
	case SDL_VIDEORESIZE:
		initAxis();
		return FALSE;
	
	default:
		return FALSE;
	}
}

RepetitionDevice::RepetitionDevice(long unsigned int delay):
	InputDevice(REPETITOR, 0, 0)
{
	myDelay = delay;

	myTimerID = SDL_AddTimer(delay, TimerCallback, static_cast<void*>(this));
}

RepetitionDevice::~RepetitionDevice()
{
	SDL_RemoveTimer(myTimerID)	
	return;
}

static Uint32 RepetitionDevice::TimerCallback(Uint32 interval, void *param)
{
	SDL_Event event;

	event.type = TIMEREVENT;
	event.user.data1 = param;

	SDL_PushEvent(&event);

	return interval;
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
			return TRUE;
		}

		return FALSE;

	default:
		return FALSE;
	}
}

} // namespace dime
