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

#ifndef __DimeOgre_ConsoleObjectImpl_H__
#define __DimeOgre_ConsoleObjectImpl_H__

#include <framework/ConsoleObject.h>

class ConsoleObjectImpl: public dime::ConsoleObject
{
	public:

	~ConsoleObjectImpl();

	static ConsoleObjectImpl & getSingleton(void);

	/**
	 * Receive commands from console
	 */
	void runCommand(const std::string &command, const std::string &args);

	private:

	ConsoleObjectImpl(void);

	/**
	 * Instance variable for singleton console object implementation.
	 */
    static ConsoleObjectImpl* _consoleObjectImplInstance;


	void quit();

	// List of Ogre's console commands
	static const char * const QUIT;
	static const char * const ADDMEDIA;
	static const char * const MOVEMEDIA;

}; // End of class declaration

#endif
