#include <utility>

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
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//


#include "HelpMessage.h"
#include <iterator>
#include <boost/algorithm/string.hpp>

namespace Ember {
namespace OgreView {
namespace Gui {
HelpMessage::HelpMessage(std::string title, std::string message, const std::string& tags, std::string id) :
		mMessage(std::move(message)),
		mTitle(std::move(title)),
		mId(std::move(id)) {
	boost::split(mTags, tags, boost::is_any_of(" "));
}

HelpMessage::~HelpMessage() = default;

void HelpMessage::setHelp(const std::string& message) {
	mMessage = message;
}

const std::string& HelpMessage::getHelp() const {
	return mMessage;
}

const std::string& HelpMessage::getId() const {
	return mId;
}

const std::string& HelpMessage::getTitle() const {
	return mTitle;
}


bool HelpMessage::hasId() const {
	return !mId.empty();
}

const std::string HelpMessage::getTags() const {
	std::string tags;
	for (const auto& tag : mTags)
		tags += " " + tag;
	return tags;
}

}
}
}

