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
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef RESOURCELISTADAPTER_H_
#define RESOURCELISTADAPTER_H_

#include <OgreResource.h>
#include "components/ogre/OgreIncludes.h"

namespace Ember {
namespace OgreView {

namespace Gui {

class ListHolder;

namespace Adapters {

namespace Ogre {

/**
 * @author Erik Ogenvik <erik@ogenvik.org>
 * @brief An adapter which binds an Ogre resource manager to a ListHolder instance.
 *
 * Make sure to call update() to activate the adapter.
 */
class ResourceListAdapter {
public:

	/**
	 * @brief Ctor.
	 *
	 * @note Remember to call update() to make sure the list is populated.
	 * @param listHolder The list holder instance to bind to. The ownership will be transferred to this instance.
	 * @param resourceManager The resource manager to get resources from.
	 */
	ResourceListAdapter(ListHolder* listHolder, ::Ogre::ResourceManager* resourceManager);

	/**
	 * @brief Dtor.
	 */
	virtual ~ResourceListAdapter();

	/**
	 * @brief Updates the list.
	 */
	void update();

private:

	/**
	 * @brief The listholder. Owned by the instance, and deleted at destruction.
	 */
	ListHolder* mListHolder;

	/**
	 * @brief The resource manager from which resources will be listed.
	 */
	::Ogre::ResourceManager* mResourceManager;

	std::shared_ptr<bool> mActiveMarker;

	std::list<::Ogre::ResourceHandle> mHandles;
	unsigned int mIndex;

	void populate();

};

}
}
}
}

}

#endif /* RESOURCELISTADAPTER_H_ */
