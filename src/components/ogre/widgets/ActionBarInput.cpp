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
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
//
#include "ActionBarInput.h"
#include "framework/Tokeniser.h"
#include "framework/ConsoleBackend.h"

using namespace Ember;
namespace Ember {
namespace OgreView {
namespace Gui {

ActionBarInput::ActionBarInput(const std::string& actionBarKey) :
		mHotkey(actionBarKey) {
	ConsoleBackend::getSingleton().registerCommand("actionbar_" + actionBarKey, [&](const std::string& command, const std::string& args) {
		EventGotHotkeyInput.emit(mHotkey);
	});

}

ActionBarInput::~ActionBarInput() {
	ConsoleBackend::getSingleton().deregisterCommand("actionbar_" + mHotkey);
}

}
}
}
