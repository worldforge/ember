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
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
//
#ifndef EMBEROGRE_QUICKHELP_H
#define EMBEROGRE_QUICKHELP_H

#include "HelpMessage.h"
#include "framework/Singleton.h"
#include <string>
#include <list>
#include <iterator>
#include <sigc++/signal.h>


namespace Ember {
namespace OgreView {

namespace Gui {

const short unsigned int MAXTUTORIALS = 5;

/**
@author Tiberiu Paunescu <tpa12@sfu.ca>

@brief Interface for managing help messages

This class is a singleton, and accessbile using OgreView::Gui::QuickHelp.getSingleton().
Help messages are held in a list structure, and are added using the updateText function.
When a message is added, an event is emitted containing an iterator to the position in the list where the message is located.

*/
class QuickHelp : public Singleton<QuickHelp> {
public:
	/**
	 * @brief Constructor
	 */
	QuickHelp();

	/**
	 * @brief Destructor
	 */
	~QuickHelp() override;

	/**
	 * @brief Add the message to the front of the list and update the widget.
	 * @params message The new message we're adding.
	 */
	void updateText(const HelpMessage& message);

	/**
	 * @brief Returns an iterator to the end of the list of messages.
	 *
	 */
	std::list<HelpMessage>::const_iterator getEnd() const;

	/**
	 * @brief Returns an iterator to the beginning of the list of messages.
	 *
	 */
	std::list<HelpMessage>::const_iterator getBeginning() const;

	/**
	 * @brief Inserts a help message at the end of the list.
	 * @params message The message to insert.
	 * @return An iterator to the end of the list.
	 */
	std::list<HelpMessage>::const_iterator insertAtEnd(const HelpMessage& message);

	/**
	 * @brief Finds the position to insert the message and inserts it if it doesn't already exist.
	 * @params message The message to insert.
	 * @return The position where the message is located in the list.
	 */
	std::list<HelpMessage>::const_iterator messagePosition(const HelpMessage& message);

	/**
	 * @brief We use this function to respond to the "?" icon pressed.
	 * @note Presents the widget to the user.
	 */
	void showWidget() const;

	/**
	 * @brief Emitted when a help message is added to the list.
	 * @note Emits an iterator to the message.
	 */
	sigc::signal<void, std::list<HelpMessage>::const_iterator> EventHelpMessageAdded;

	sigc::signal<void, int> EventHelpMessageLocationChanged;

	/**
	 * @brief Emitted when we want to show the QuickHelp widget
	 */
	sigc::signal<void> EventToggleWidgetVisibility;

	/**
	 * @brief Returns the size of the message list
	 * @return The size of the message list.
	 */
	size_t getSize() const;

private:

	/**
	 * @brief Holds a list of all of the messages. At most MAXTUTORIALS messages.
	 */
	std::list<HelpMessage> mTutorialText;
};

}
}
}

#endif
