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
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "CommandHistory.h"

namespace Ember
{

CommandHistory::CommandHistory() :
	mHistoryPosition(0)
{
}

void CommandHistory::moveBackwards(void)
{
	if (mHistoryPosition < mHistory.size()) {
		mHistoryPosition++;
	}
}

void CommandHistory::moveForwards(void)
{
	if (mHistoryPosition > 0) {
		mHistoryPosition--;
	}
}

const std::string& CommandHistory::getHistoryString()
{
	static std::string sEmpty("");

	if (mHistoryPosition == 0) {
		return sEmpty;
	} else {
		return mHistory[mHistoryPosition - 1];
	}
}

void CommandHistory::changeHistory(size_t historyIndex, const std::string & command)
{
	if (historyIndex < mHistory.size()) {
		mHistory[historyIndex - 1] = command;
	}
}

size_t CommandHistory::getHistoryPosition() const
{
	return mHistoryPosition;
}

void CommandHistory::addToHistory(const std::string& command)
{
	mHistory.push_front(command);
	mHistoryPosition = 0;
}

}
