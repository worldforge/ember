//
// C++ Interface: TutorialMessage
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
#ifndef EMBEROGRE_HELPMESSAGE_H
#define EMBEROGRE_HELPMESSAGE_H

#include <string>

namespace EmberOgre
{

namespace Gui
{

/**
@author Tiberiu Paunescu <tpa12@sfu.ca>

@brief Class that holds tutorial / help messages.

Currently, it only holds plain text messages but future implementations will contain more information regarding the text.

*/
class HelpMessage
{
public:
	/**
	 * @brief Constructor
	 */
	HelpMessage(const std::string &message, const std::string &caption, const std::string& id = "");

	/**
	 * @brief Desctructor
	 */
	~HelpMessage();

	/**
	 * @brief Sets the message.
	 * @param message The new message.
	 */
	void setHelp(const std::string &message);

	/**
	 * @brief Returns the message stored.
	 * @return The message.
	 */
	const std::string& getHelp() const;

	const std::string& getId() const;

	bool hasId() const;

private:

	/**
	 * @brief Stores the plain text mesasge.
	 */
	std::string mMessage;
	std::string mCaption;
	std::string mId;
};
}
}

#endif
