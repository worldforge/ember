//
// C++ Implementation: IconStore
//
// Description: 
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2007
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "IconStore.h"
#include "IconImageStore.h"
#include "Icon.h"


namespace Ember {
namespace OgreView {

namespace Gui {

namespace Icons {


IconStore::~IconStore() = default;

Icon* IconStore::createIcon(const std::string& key) {
	IconImageStoreEntry* imageStoreEntry = getImageStoreEntry();
	auto I = mIcons.emplace(key, std::make_unique<Icon>(key, imageStoreEntry));
	return I.first->second.get();
}

Icon* IconStore::createIcon(const std::string& key, Ogre::TexturePtr texPtr) {
	auto I = mPremadeIconImageStores.emplace(key, new IconImageStore(key, texPtr));
	IconImageStoreEntry* imageStoreEntry = I.first->second->claimImageEntry();

	auto J = mIcons.emplace(key, std::make_unique<Icon>(key, imageStoreEntry));
	return J.first->second.get();
}


Icon* IconStore::getIcon(const std::string& key) {
	auto I = mIcons.find(key);
	if (I != mIcons.end()) {
		return I->second.get();
	}
	return nullptr;
}

bool IconStore::hasIcon(const std::string& key) {
	return mIcons.find(key) != mIcons.end();
}

void IconStore::destroyIcon(Icon* icon) {
	mIcons.erase(icon->mKey);
	delete icon;
}

IconImageStoreEntry* IconStore::getImageStoreEntry() {
	for (auto& iconImageStore : mIconImageStores) {
		if (iconImageStore->getNumberOfUnclaimedIcons()) {
			return iconImageStore->claimImageEntry();
		}
	}
	std::stringstream ss;
	ss << "iconImageStore_" << mIconImageStores.size();
	mIconImageStores.emplace_back(new IconImageStore(ss.str()));
	return mIconImageStores.back()->claimImageEntry();
}


}

}

}
}
