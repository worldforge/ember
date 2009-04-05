//
// C++ Interface: IInputAdapter
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2005
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//
#ifndef EMBEROGREIINPUTADAPTOR_H
#define EMBEROGREIINPUTADAPTOR_H

#include "Input.h"

#include <SDL.h>


namespace Ember {


/**
	@brief An adapter class which is used to listen to events from the Input class, when the system is in gui input mode.
	Use instances of this if you want to hook into the input system and override the default gui handling.
	@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class IInputAdapter
{
public:

	virtual ~IInputAdapter() {}
	/**
	 *    Injects a mouse move. Returns false if the event shouldn't be processed any more.
	 * @param x 
	 * @param y 
	 * @return true if other IInputAdapters should continue handle the event, false if no more handling should happen
	 */
	virtual bool injectMouseMove(const MouseMotion& motion, bool& freezeMouse) = 0;
	/**
	 *    Injects a mouse button up event. Returns false if the event shouldn't be processed any more.
	 * @param button 
	 * @return true if other IInputAdapters should continue handle the event, false if no more handling should happen
	 */
	virtual bool injectMouseButtonUp(const Input::MouseButton& button) = 0;
	/**
	 *    Injects a mouse button down event. Returns false if the event shouldn't be processed any more.
	 * @param button 
	 * @return true if other IInputAdapters should continue handle the event, false if no more handling should happen
	 */
	virtual bool injectMouseButtonDown(const Input::MouseButton& button) = 0;
	/**
	 *    Injects a character. Returns false if the event shouldn't be processed any more.
	 * @param character 
	 * @return true if other IInputAdapters should continue handle the event, false if no more handling should happen
	 */
	virtual bool injectChar(char character) = 0;
	/**
	 *    Injects a key down event. Returns false if the event shouldn't be processed any more.
	 * @param key 
	 * @return true if other IInputAdapters should continue handle the event, false if no more handling should happen
	 */
	virtual bool injectKeyDown(const SDLKey& key) = 0;
	/**
	 *    Injects a key up event. Returns false if the event shouldn't be processed any more.
	 * @param key 
	 * @return true if other IInputAdapters should continue handle the event, false if no more handling should happen
	 */
	virtual bool injectKeyUp(const SDLKey& key) = 0;
};

}

#endif
