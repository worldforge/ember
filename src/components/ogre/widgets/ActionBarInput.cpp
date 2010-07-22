//
// C++ Implementation: ActionBarInput
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
#include "ActionBarInput.h"

using namespace Ember;
namespace EmberOgre
{
namespace Gui
{

ActionBarInput::ActionBarInput(const std::string& actionBarKey) : ActionBarButton("actionbar_" + actionBarKey, this, "Hotkey for ActionBar key: " + actionBarKey + "."), mHotkey(actionBarKey)
{
}

void ActionBarInput::runCommand(const std::string &command, const std::string &args)
{
	if (ActionBarButton == command)
		EventGotHotkeyInput.emit(mHotkey);
}

ActionBarInput::~ActionBarInput()
{
}

}
}
