//
// C++ Interface: IconStore
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@iteam.se>, (C) 2007
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
#ifndef EMBEROGRE_GUI_ICONSICONSTORE_H
#define EMBEROGRE_GUI_ICONSICONSTORE_H

#include <map>
#include <vector>
#include <OgreTexture.h>

namespace EmberOgre {

namespace Gui {

namespace Icons {

class Icon;
class IconImageStore;
class IconImageStoreEntry;

/**
	@author Erik Hjortsberg <erik.hjortsberg@iteam.se>
*/
class IconStore{
public:
	typedef std::map<std::string, Icon*> IconMap;
	typedef std::vector<IconImageStore*> IconImageStoreStore;
	typedef std::map<std::string, IconImageStore*> IconImageStoreMap;
    IconStore();

    ~IconStore();
    
    Icon* createIcon(const std::string& key);
    Icon* createIcon(const std::string& key, Ogre::TexturePtr texPtr);
    Icon* getIcon(const std::string& key);
    bool hasIcon(const std::string& key);
    void destroyIcon(Icon* icon);
    
protected:
	IconMap mIcons;
	IconImageStoreStore mIconImageStores;
	IconImageStoreMap mPremadeIconImageStores;
	
	IconImageStoreEntry* getImageStoreEntry();

};

}

}

}

#endif
