//
// C++ Interface: HelpMessage
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
#ifndef EMBEROGRE_HELPMESSAGE_H
#define EMBEROGRE_HELPMESSAGE_H

#include <string>
#include <list>

namespace Ember
{
namespace OgreView
{

namespace Gui
{

/**
@author Tiberiu Paunescu <tpa12@sfu.ca>

@brief Class that holds help messages.

Currently, it only holds plain text messages but future implementations will contain more information regarding the text.

*/
class HelpMessage
{
public:
	/**
	 * @brief Constructor
	 */
	HelpMessage(const std::string &title, const std::string &message, const std::string& tags, const std::string& id = "");

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

	/**
	 * @brief Returns the optional id of the message.
	 * @return The id.
	 */
	const std::string& getId() const;

	/**
	 * @brief Iterates over the list of tags and returns a string containing all of them
	 * @return String containing all of the tags
	 */
	const std::string getTags() const;

	/**
	 * @brief Returns the message title.
	 * @returns The message title.
	 */
	const std::string& getTitle() const;

	/**
	 * @brief Checks if the message contains an optional id.
	 */
	bool hasId() const;

private:
	/**
	 * @brief Stores the various tags for the message.
	 */
	std::list<std::string> mTags;

	/**
	 * @brief Stores the plain text message.
	 */
	std::string mMessage;

	/**
	 * @brief Stores the title of the message.
	 */
	std::string mTitle;

	/**
	 * @brief Stores the optional id.
	 */
	std::string mId;
};
}
}

}

#endif
