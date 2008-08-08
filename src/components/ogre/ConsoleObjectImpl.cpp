/*
	ConsoleObjectImpl.cpp by Miguel Guzman (Aglanor)
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

// config headers
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

// system headers

// library headers
#include "EmberOgrePrerequisites.h"

// local headers
#include "ConsoleObjectImpl.h"
#include "framework/ConsoleBackend.h"
#include "framework/Tokeniser.h"

#include "main/Application.h"
// #include <SDL.h>

template<> EmberOgre::ConsoleObjectImpl* Ember::Singleton<EmberOgre::ConsoleObjectImpl>::ms_Singleton = 0;

namespace EmberOgre {


ConsoleObjectImpl::ConsoleObjectImpl(void)
{
}
ConsoleObjectImpl::~ConsoleObjectImpl()
{

}


void ConsoleObjectImpl::runCommand(const std::string &command, const std::string &args)
{

}

}

