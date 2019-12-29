/*
 Copyright (C) 2010 Erik Ogenvik <erik@ogenvik.org>

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ResourceListAdapter.h"

#include "components/ogre/widgets/ListHolder.h"
#include "components/ogre/widgets/ColouredListItem.h"

#include <OgreResourceManager.h>
#include <framework/MainLoopController.h>

namespace Ember {
namespace OgreView {

namespace Gui {

namespace Adapters {

namespace Ogre {


ResourceListAdapter::ResourceListAdapter(ListHolder* listHolder, ::Ogre::ResourceManager* resourceManager)
		: mListHolder(listHolder),
		mResourceManager(resourceManager),
		mIndex(0){
}

void ResourceListAdapter::update() {
	mListHolder->resetList();
	mHandles.clear();
	mIndex = 0;

	::Ogre::ResourceManager::ResourceMapIterator I = mResourceManager->getResourceIterator();
	while (I.hasMoreElements()) {
		mHandles.push_back(I.peekNextKey());
		I.moveNext();
	}

	if (!mHandles.empty()) {
		MainLoopController::getSingleton().getEventService().runOnMainThread([&]() {
			populate();
		}, mActiveMarker);
	}
}

void ResourceListAdapter::populate() {
	auto handle = mHandles.front();
	::Ogre::ResourcePtr resource = mResourceManager->getByHandle(handle);
	if (resource) {
		mListHolder->addItem(new ColouredListItem(resource->getName(), mIndex));
	}
	mIndex += 1;

	mHandles.pop_front();
	if (!mHandles.empty()) {
		MainLoopController::getSingleton().getEventService().runOnMainThread([&]() {
			populate();
		}, mActiveMarker);
	}
}

}
}
}
}
}
