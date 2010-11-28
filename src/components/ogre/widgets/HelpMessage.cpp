//
// C++ Implementation: HelpMessage
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


#include "HelpMessage.h"
#include <iterator>
#include <boost/algorithm/string.hpp>

namespace Ember
{
namespace OgreView
{
namespace Gui
{
HelpMessage::HelpMessage(const std::string &title, const std::string &message, const std::string& tags, const std::string& id) : mMessage(message), mTitle(title), mId(id)
{
	boost::split(mTags, tags, boost::is_any_of(" "));
}

HelpMessage::~HelpMessage()
{
}

void HelpMessage::setHelp(const std::string &message)
{
	mMessage = message;
}

const std::string& HelpMessage::getHelp() const
{
	return mMessage;
}

const std::string& HelpMessage::getId() const
{
	return mId;
}

const std::string& HelpMessage::getTitle() const
{
	return mTitle;
}


bool HelpMessage::hasId() const
{
	return mId.size() != 0;
}

const std::string HelpMessage::getTags() const
{
	std::string tags;
	for (std::list<std::string>::const_iterator list_iterator = mTags.begin(); list_iterator != mTags.end(); list_iterator++ )
		tags += " " + (*list_iterator);
	return tags;
}

}
}
}

