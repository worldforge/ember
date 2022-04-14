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

#ifndef EMBER_EMBERWORKQUEUE_H
#define EMBER_EMBERWORKQUEUE_H

#include <Threading/OgreDefaultWorkQueueStandard.h>
#include <Eris/EventService.h>

#include <set>

namespace Ember {
namespace OgreView {

/**
 * A specialization of the default Ogre Workqueue which uses the Eris EventService for
 * processing responses on the main thread.
 */
class EmberWorkQueue : public Ogre::DefaultWorkQueue {

public:
	explicit EmberWorkQueue(Eris::EventService& eventService);

	~EmberWorkQueue() override = default;


	void processResponses() override;

	Ogre::uint16 getChannel(const Ogre::String& channelName) override;

private:
	Eris::EventService& mEventService;

	/**
	 * The Ogre Terrain components rely on it's handlers always being executed on the main thread directly when processResponses() is called.
	 * We'll have to take this into account and keep track of the terrain channels.
	 */
	std::set<Ogre::uint16> mTerrainChannels;
};

}
}
#endif //EMBER_EMBERWORKQUEUE_H
