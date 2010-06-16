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

#include "framework/Singleton.h"
#include <sigc++/signal.h>
#include <string>
#include <list>
#include <iterator>
#include "TutorialMessage.h"

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
	 * @brief Add the message to the front of the list and update the widget
	 * @params The new message we're adding
	 */
	void updateText(const std::string& text);

	/**
	 * @brief Called when the next buttons is clicked, increments the current message
	 */
	void nextMessage();

	/**
	 * @brief Called when the back buttons is clicked, decrements the current message
	 */
	void previousMessage();

	/**
	 * @brief Updates the text in the widget
	 */
	sigc::signal <void,const std::string&> EventUpdateText;

private:

	/**
	 * @brief Holds a list of all of the messages. At most MAXTUTORIALS messages
	 */
	std::list<TutorialMessage> mTutorialText;

	/**
	 * @brief Holds the position of the current message displayed in the widget
	 */
	std::list<TutorialMessage>::iterator mCurrentPosition;

};

}
}


#endif
