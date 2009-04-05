//
// C++ Implementation: IconStore
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2007
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "IconStore.h"
#include "IconImageStore.h"
#include "Icon.h"


namespace EmberOgre {

namespace Gui {

namespace Icons {

IconStore::IconStore()
{
}


IconStore::~IconStore()
{
	for (IconMap::iterator I = mIcons.begin(); I != mIcons.end(); ++I) {
		delete I->second;
	}
	for (IconImageStoreStore::iterator I = mIconImageStores.begin(); I != mIconImageStores.end(); ++I) {
		delete *I;
	}
	for (IconImageStoreMap::iterator I = mPremadeIconImageStores.begin(); I != mPremadeIconImageStores.end(); ++I) {
		delete I->second;
	}
}

Icon* IconStore::createIcon(const std::string& key)
{
	IconImageStoreEntry* imageStoreEntry = getImageStoreEntry();
	Icon* icon = new Icon(key, imageStoreEntry);
	
	mIcons.insert(IconMap::value_type(key, icon));
	return icon;
}

Icon* IconStore::createIcon(const std::string& key, Ogre::TexturePtr texPtr)
{
	IconImageStore* store = new IconImageStore(key, texPtr);
	mPremadeIconImageStores.insert(IconImageStoreMap::value_type(key, store));
	IconImageStoreEntry* imageStoreEntry = store->claimImageEntry();
	
	Icon* icon = new Icon(key, imageStoreEntry);
	
	mIcons.insert(IconMap::value_type(key, icon));
	return icon;
}


Icon* IconStore::getIcon(const std::string& key)
{
	IconMap::iterator I = mIcons.find(key);
	if (I != mIcons.end()) {
		return I->second;
	}
	return 0;
}

bool IconStore::hasIcon(const std::string& key)
{
	return mIcons.find(key) != mIcons.end();
}

void IconStore::destroyIcon(Icon* icon)
{
	mIcons.erase(icon->mKey);
	delete icon;
}

IconImageStoreEntry* IconStore::getImageStoreEntry()
{
	for (IconImageStoreStore::iterator I = mIconImageStores.begin(); I != mIconImageStores.end(); ++I) {
		if ((*I)->getNumberOfUnclaimedIcons()) {
			return (*I)->claimImageEntry();
		}
	}
	std::stringstream ss;
	ss << "iconImageStore_" << mIconImageStores.size();
	IconImageStore* store = new IconImageStore(ss.str());
	mIconImageStores.push_back(store);
	return store->claimImageEntry();
}



}

}

}
