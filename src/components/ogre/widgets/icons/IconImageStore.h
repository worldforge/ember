//
// C++ Interface: IconImageStore
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
#ifndef EMBEROGRE_GUI_ICONSICONIMAGESTORE_H
#define EMBEROGRE_GUI_ICONSICONIMAGESTORE_H

#include <map>
#include <string>
#include <stack>
#include <vector>

#include <OgreTexture.h>

namespace CEGUI {
	class Imageset;
	class Image;
	class Texture;
}

namespace Ogre {
	class Image;
}

namespace EmberOgre {

namespace Gui {

namespace Icons {

class IconImageStore;

class IconImageStoreEntry {
public:
	typedef std::pair<size_t, size_t> PixelPos;
	
	IconImageStoreEntry(IconImageStore& iconImageStore, const PixelPos& pixelPosInImageset);
	virtual ~IconImageStoreEntry() {}
	
	const CEGUI::Image* getImage();
	const CEGUI::Image* getImage() const;
	
	const std::string& getImageName();
	const std::string& getImageName() const;
	
	Ogre::Image::Box getBox();
    Ogre::TexturePtr getTexture();
	
protected:
	void createImage();
	
	const CEGUI::Image* mImage;
	IconImageStore& mIconImageStore;
	PixelPos mPixelPosInImageset;
	std::string mImageName;
};

/**
	@author Erik Hjortsberg <erik.hjortsberg@iteam.se>
*/
class IconImageStore {
public:
	friend class IconImageStoreEntry;
	//typedef std::map<int, IconImageStoreEntry*> IconImageStoreEntryMap;
	typedef std::stack<IconImageStoreEntry*> IconImageStoreEntryStack;
	typedef std::vector<IconImageStoreEntry*> IconImageStoreEntryStore;
    IconImageStore(const std::string& imagesetName);
    virtual ~IconImageStore();
    
    size_t getNumberOfUnclaimedIcons();
    
    IconImageStoreEntry* claimImageEntry();
    void reclaimImageEntry(IconImageStoreEntry* imageEntry);
    
//     Ogre::TexturePtr getTexture();
    
private:
	void createImageset();
	void createEntries();

	CEGUI::Texture* mCeguiTexture;
	CEGUI::Imageset* mImageset;
	std::string mImagesetName;
	//Ogre::Image* mOgreImage;
	Ogre::TexturePtr mTexPtr;
	
	IconImageStoreEntryStore mIconImages;
	IconImageStoreEntryStack mUnclaimedIconImages;
	int mIconSize;
	int mImageSize;

};

}

}

}

#endif
