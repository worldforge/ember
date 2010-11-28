//
// C++ Interface: ActionBarInput
//
// Description:
//
//
// Author: Tiberiu Paunescu <tpa12@sfu.ca>, (C) 2010
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
#ifndef ACTIONBARINPUT_H
#define ACTIONBARINPUT_H

#include "services/input/InputCommandMapper.h"
#include <string>
#include <sigc++/signal.h>

namespace Ember
{
namespace OgreView
{
namespace Gui
{
/**
	@brief
	@author Tiberiu Paunescu <tpa12@sfu.ca>
	The ActionBarInput class is used to capture keyboard input based on the key passed to the constructor. Each instance of the class is created from the lua ActionBar object,
	and captures one key.
*/
class ActionBarInput : public Ember::ConsoleObject
{
public:
	/**
	 * @brief Ctor
	 * @param actionBarKey The key we intend to capture presses from.
	 */
	ActionBarInput(const std::string& actionBarKey);

	/**
	 * @brief Dtor
	 */
	~ActionBarInput();

	virtual	void runCommand(const std::string &command, const std::string &args);

	/**
	 * @brief Signal the event when we get a key press
	 */
	sigc::signal<void, const std::string&> EventGotHotkeyInput;
private:
	/**
	 * @brief The hotkey we're monitoring.
	 */
	std::string mHotkey;
	const Ember::ConsoleCommandWrapper ActionBarButton;
};
}
}
}

#endif
