/*
	ConsoleObjectImpl.h by Miguel Guzman (Aglanor)
	Copyright (C) 2002 Miguel Guzman & The Worldforge Project

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

#ifndef __EmberOgre_ConsoleObjectImpl_H__
#define __EmberOgre_ConsoleObjectImpl_H__

#include "framework/ConsoleObject.h"
#include "framework/Singleton.h"

namespace EmberOgre {
class ConsoleObjectImpl: public Ember::ConsoleObject, public Ember::Singleton<ConsoleObjectImpl>
{
	public:

	ConsoleObjectImpl(void);
	~ConsoleObjectImpl();

	/**
	 * Receive commands from console
	 */
	void runCommand(const std::string &command, const std::string &args);

	private:



	void quit();

	/// List of Ogre's console commands
	const Ember::ConsoleCommandWrapper Quit;
	const Ember::ConsoleCommandWrapper ToggleErisPolling;


}; // End of class declaration

}

#endif
