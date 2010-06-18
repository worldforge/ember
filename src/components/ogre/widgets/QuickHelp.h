//
// C++ Interface: QuickHelp
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
#ifndef EMBEROGRE_QUICKHELP_H
#define EMBEROGRE_QUICKHELP_H

#include "HelpMessage.h"
#include "framework/Singleton.h"
#include <string>
#include <list>
#include <iterator>
#include <sigc++/signal.h>


namespace EmberOgre {

namespace Gui {

const short unsigned int MAXTUTORIALS = 5;
/**
@author Tiberiu Paunescu <tpa12@sfu.ca>

@brief Interface for handling all tutorial messages

This class is a singleton, and accessbile using EmberOgre::Gui::QuickHelp.getSingleton().
Tutorial messages are held in a list structure, and are added using the updateText function.
The functions nextMessage and previousMessage are called from the lua code for the tutorial widget.

*/
class QuickHelp : public Ember::Singleton<QuickHelp>
{
public:
	/**
	 * @brief Constructor
	 */
	QuickHelp();

	/**
	 * @brief Destructor
	 */
	virtual ~QuickHelp();

	/**
	 * @brief Add the message to the front of the list and update the widget.
	 * @params The new message we're adding.
	 */
	void updateText(const HelpMessage& message);

	/**
	 * @brief Returns an iterator to the end of the list of messages
	 *
	 */
	const std::list<HelpMessage>::const_iterator getEnd() const;

	/**
	 * @brief Returns an iterator to the beginning of the list of messages
	 *
	 */
	const std::list<HelpMessage>::const_iterator getBeginning() const;


	const std::list<HelpMessage>::const_iterator insertAtEnd(const HelpMessage& message);

	const std::list<HelpMessage>::const_iterator messagePosition(const HelpMessage& message);

	sigc::signal <void, std::list<HelpMessage>::const_iterator> EventTutorialAdded;

private:

	/**
	 * @brief Holds a list of all of the messages. At most MAXTUTORIALS messages.
	 */
	std::list<HelpMessage> mTutorialText;
};

}
}


#endif
