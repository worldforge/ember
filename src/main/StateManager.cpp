/*
    Copyright (C) 2002  Martin Pollard (Xmp)

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

#include "StateManager.h"
#include "framework/Exception.h"

using namespace dime;

typedef std::list<State>::iterator stateIter;

State& StateManager::findState(const std::string& state)
{
  for(stateIter i = myStates.begin();i != myStates.end();i++)
    {
      if ((*i).getName() == state)
	return *i;
    }

  // Maybe search for the default version of the state in future?
  // If not found then throw.
  THROW("State not found");
}
