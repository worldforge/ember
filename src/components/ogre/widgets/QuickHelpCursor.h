//
// C++ Interface: QuickHelpCursor
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
#ifndef EMBEROGRE_QUICKHELP_CURSOR_H
#define EMBEROGRE_QUICKHELP_CURSOR_H

#include <list>
#include <sigc++/signal.h>
#include <string>

namespace EmberOgre {

namespace Gui {

class QuickHelp;
class HelpMessage;

/**
@author Tiberiu Paunescu <tpa12@sfu.ca>

@brief Interface for accessing HelpMessages in the QuickHelp class

Instances of this class should be used to keep track and access HelpMessages stored in the QuickHelp class.
It is not for inserting new messages, that is handled by the QuickHelp class.

*/

class QuickHelpCursor
{
public:
	QuickHelpCursor();
	~QuickHelpCursor();

	/**
	 * @brief Increments the current position iterator.
	 */
	void nextMessage();

	/**
	 * @brief Decrements the current position iterator.
	 */
	void previousMessage();

	/**
	 * @brief When the position iterator changes, we call this to emit the current message.
	 */
	void updateMessage();

	/**
	 * @brief Catches new messages emitted from QuickHelp.
	 */
	void getLatestTutorial(std::list<HelpMessage>::const_iterator position);

	/**
	 * @brief Called when position iterator changes.
	 */
	sigc::signal <void,const std::string&> EventUpdateText;
private:

	/**
	 * @brief Holds the position of the current message displayed in the widget.
	 */
	std::list<HelpMessage>::const_iterator mCurrentPosition;

	QuickHelp& mQuickHelp;

};

}

}

#endif
