/*
 Copyright (C) 2017 Erik Ogenvik

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

#include "EmberWorkQueue.h"
#include "EmberOgrePrerequisites.h"

namespace Ember {
namespace OgreView {
EmberWorkQueue::EmberWorkQueue(Eris::EventService& eventService)
		: mEventService(eventService) {
}

Ogre::uint16 EmberWorkQueue::getChannel(const Ogre::String& channelName) {
	auto channelId = Ogre::DefaultWorkQueue::getChannel(channelName);
	//The Ogre Terrain components rely on it's handlers always being executed on the main thread directly when processResponses() is called.
	//We'll have to take this into account and keep track of the terrain channels.
	if (Ogre::StringUtil::startsWith(channelName, "Ogre/Terrain", false)) {
		mTerrainChannels.insert(channelId);
	}
	return channelId;
}


void EmberWorkQueue::processResponses() {
	while (true) {
		Ogre::WorkQueue::Response* response = nullptr;
		{
			OGRE_LOCK_MUTEX(mResponseMutex);

			if (mResponseQueue.empty()) {
				break;
			}

			response = mResponseQueue.front();
			mResponseQueue.pop_front();
		}

		if (response) {
			//The Ogre Terrain components rely on it's handlers always being executed on the main thread directly when processResponses() is called.
			//We'll have to take this into account and process all terrain handlers directly.
			if (mTerrainChannels.find(response->getRequest()->getChannel()) != mTerrainChannels.end()) {
				processResponse(response);
			} else {
				mEventService.runOnMainThread([=] {
					processResponse(response);
					OGRE_DELETE response;
				});
			}
		}
	}
}

}
}