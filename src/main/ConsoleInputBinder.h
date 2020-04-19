/*
 Copyright (C) 2020 Erik Ogenvik

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

#ifndef EMBER_CONSOLEINPUTBINDER_H
#define EMBER_CONSOLEINPUTBINDER_H

#include "framework/ConsoleObject.h"
#include "framework/ConsoleCommandWrapper.h"
#include "services/input/Input.h"

namespace Ember {

class InputCommandMapper;

/**
 * Provides console commands which interacts with the Input system.
 *
 * Mainly these commands:
 * /bind, which allows binding of keys to other console commands (for example binding the key "w" to "move_forward")
 * /unbind, which allows unbinding of keys bound with "/bind"
 * /toggle_fullscreen, which toggles full screen mode.
 */
class ConsoleInputBinder {

public:
	ConsoleInputBinder(Input& input, ConsoleBackend& consoleBackend);

protected:

	Input& mInput;

	const ConsoleCommandWrapper mBind;
	const ConsoleCommandWrapper mUnbind;
	const ConsoleCommandWrapper mToggleFullscreen;

	void bindCommand(const std::string& command, const std::string& args);

	void unbindCommand(const std::string& command, const std::string& args);

	void toggleFullscreenCommand(const std::string& command, const std::string& args);


};
}

#endif //EMBER_CONSOLEINPUTBINDER_H
