//
// C++ Interface: IInputAdapter
//
// Description: 
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2005
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
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
//
#ifndef EMBEROGREIINPUTADAPTOR_H
#define EMBEROGREIINPUTADAPTOR_H

#include "Input.h"

#include <SDL_scancode.h>


namespace Ember {


/**
	@brief An adapter class which is used to listen to events from the Input class, when the system is in gui input mode.
	Use instances of this if you want to hook into the input system and override the default gui handling.
	@author Erik Ogenvik <erik@ogenvik.org>
*/
struct IInputAdapter
{

	virtual ~IInputAdapter() = default;
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
	virtual bool injectMouseButtonUp(Input::MouseButton button) = 0;
	/**
	 *    Injects a mouse button down event. Returns false if the event shouldn't be processed any more.
	 * @param button 
	 * @return true if other IInputAdapters should continue handle the event, false if no more handling should happen
	 */
	virtual bool injectMouseButtonDown(Input::MouseButton button) = 0;
	/**
	 *    Injects a character. Returns false if the event shouldn't be processed any more.
	 * @param character 
	 * @return true if other IInputAdapters should continue handle the event, false if no more handling should happen
	 */
	virtual bool injectChar(int character) = 0;
	/**
	 *    Injects a key down event. Returns false if the event shouldn't be processed any more.
	 * @param key 
	 * @return true if other IInputAdapters should continue handle the event, false if no more handling should happen
	 */
	virtual bool injectKeyDown(const SDL_Scancode& key) = 0;
	/**
	 *    Injects a key up event. Returns false if the event shouldn't be processed any more.
	 * @param key 
	 * @return true if other IInputAdapters should continue handle the event, false if no more handling should happen
	 */
	virtual bool injectKeyUp(const SDL_Scancode& key) = 0;
};

}

#endif
