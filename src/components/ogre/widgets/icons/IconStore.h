//
// C++ Interface: IconStore
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
#ifndef EMBEROGRE_GUI_ICONSICONSTORE_H
#define EMBEROGRE_GUI_ICONSICONSTORE_H

#include <unordered_map>
#include <vector>
#include <OgreTexture.h>

#include "IconImageStore.h"
#include "Icon.h"
namespace Ember {
namespace OgreView {

namespace Gui {

namespace Icons {

class Icon;
class IconImageStoreEntry;

/**
	@author Erik Ogenvik <erik@ogenvik.org>
*/
class IconStore{
public:

	explicit IconStore() = default;

    ~IconStore();
    
    Icon* createIcon(const std::string& key);
    Icon* createIcon(const std::string& key, const Ogre::TexturePtr& texPtr);
    Icon* getIcon(const std::string& key);
    bool hasIcon(const std::string& key);
    void destroyIcon(Icon* icon);
    
protected:
	std::unordered_map<std::string, std::unique_ptr<Icon>> mIcons;
	std::vector<std::unique_ptr<IconImageStore>> mIconImageStores;
	std::unordered_map<std::string, std::unique_ptr<IconImageStore>> mPremadeIconImageStores;
	
	IconImageStoreEntry* getImageStoreEntry();

};

}

}

}

}

#endif
