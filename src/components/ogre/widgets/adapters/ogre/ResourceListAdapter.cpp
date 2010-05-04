/*
 Copyright (C) 2010 Erik Hjortsberg <erik.hjortsberg@gmail.com>

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

#include "ResourceListAdapter.h"

#include "components/ogre/widgets/ListHolder.h"
#include "components/ogre/widgets/ColouredListItem.h"

#include <OgreResourceManager.h>
#include <OgreResource.h>

namespace EmberOgre
{

namespace Gui
{

namespace Adapters
{

namespace Ogre
{


ResourceListAdapter::ResourceListAdapter(ListHolder* listHolder, ::Ogre::ResourceManager* resourceManager)
: mListHolder(listHolder), mResourceManager(resourceManager)
{

}

ResourceListAdapter::~ResourceListAdapter()
{
//	delete mListHolder;
}

void ResourceListAdapter::update()
{
	int i = 0;
	mListHolder->resetList();
	::Ogre::ResourceManager::ResourceMapIterator I = mResourceManager->getResourceIterator();
	while (I.hasMoreElements()) {
		::Ogre::ResourcePtr resource = I.getNext();
		if (!resource.isNull()) {
			mListHolder->addItem(new ColouredListItem(resource->getName(), i));
			++i;
		}
	}
}

}
}
}
}
