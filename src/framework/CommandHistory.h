/*
 Copyright (C) 2011 Erik Ogenvik

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

#ifndef COMMANDHISTORY_H_
#define COMMANDHISTORY_H_

#include <deque>
#include <string>

namespace Ember
{

/**
 * @author Erik Ogenvik <erik@ogenvik.org>
 * @brief A simple command history, basically acts as a cursor into a list of previously registered commands.
 */
class CommandHistory
{
public:
	/**
	 * @brief Ctor.
	 */
	CommandHistory();

	/**
	 * @brief Gets the current position within the history.
	 * When traversing the history we need to keep track of where in the history we are.
	 * @return The current position within the history.
	 */
	size_t getHistoryPosition() const;

	/**
	 * @brief Get the current history string.
	 * The history position 0 is managed in the ConsoleWidget.
	 * @return The current history string.
	 **/
	const std::string & getHistoryString();

	/**
	 * @brief Changes a command in the history.
	 * @param stHistoryIndex The index of the command to change.
	 * @param sCommand The new command.
	 */
	void changeHistory(size_t historyIndex, const std::string & command);

	/**
	 * Moves the history iterator backwards (in time).
	 **/
	void moveBackwards(void);

	/**
	 * Moves the history iterator forwards (in time).
	 **/
	void moveForwards(void);

	/**
	 * @brief Adds a new entry to the history.
	 * @param command The command to add.
	 */
	void addToHistory(const std::string& command);

private:

	/**
	 * @brief Message history.
	 */
	std::deque<std::string> mHistory;

	/**
	 * @brief History iterator.
	 */
	size_t mHistoryPosition;

};

}

#endif /* COMMANDHISTORY_H_ */
