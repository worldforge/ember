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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "EntityTalk.h"

#include <Atlas/Message/Element.h>
#include <Atlas/Objects/RootOperation.h>
#include <algorithm>

namespace Ember
{

EntityTalk::EntityTalk(const Atlas::Objects::Operation::RootOperation& talkArgs)
{
	const std::vector<Atlas::Objects::Root>& args = talkArgs->getArgs();
	if (!args.empty()) {

		const Atlas::Objects::Root& talk = args.front();

		Atlas::Message::Element sayAttrib;
		if (!talk->copyAttr("say", sayAttrib) && sayAttrib.isString()) {
			mMessage = talk->getAttr("say").asString();
		}

		Atlas::Message::Element responseAttrib;
		//some talk operations come with a predefined set of suitable responses, so we'll store those so that they can later on be queried by the GUI for example
		if (!talk->copyAttr("responses", responseAttrib) && responseAttrib.isList()) {
			const Atlas::Message::ListType & responseList = responseAttrib.asList();
			Atlas::Message::ListType::const_iterator I = responseList.begin();
			for (; I != responseList.end(); ++I) {
				if (I->isString()) {
					mSuggestedResponses.push_back(I->asString());
				}
			}
		}

		Atlas::Message::Element addressAttrib;
		//some talk operations come with a predefined set of suitable responses, so we'll store those so that they can later on be queried by the GUI for example
		if (!talk->copyAttr("address", addressAttrib) && addressAttrib.isList()) {
			const Atlas::Message::ListType & addressList = addressAttrib.asList();
			Atlas::Message::ListType::const_iterator I = addressList.begin();
			for (; I != addressList.end(); ++I) {
				if (I->isString()) {
					mAddressedEntityIds.push_back(I->asString());
				}
			}
		}
	}

}

const std::string& EntityTalk::getMessage() const
{
	return mMessage;
}

bool EntityTalk::isAddressedToNone() const
{
	return mAddressedEntityIds.size() == 0;
}

bool EntityTalk::isAddressedToEntity(const std::string& entityId) const
{
	return std::find(mAddressedEntityIds.begin(), mAddressedEntityIds.end(), entityId) != mAddressedEntityIds.end();
}

bool EntityTalk::isAddressedToEntityOrNone(const std::string& entityId) const
{
	return isAddressedToNone() || isAddressedToEntity(entityId);
}

const std::vector<std::string>& EntityTalk::getAddressedEntityIds() const
{
	return mAddressedEntityIds;
}

const std::vector<std::string>& EntityTalk::getSuggestedResponses() const
{
	return mSuggestedResponses;
}

}
