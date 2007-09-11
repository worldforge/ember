//
// C++ Implementation: IconImageStore
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
#include "IconImageStore.h"
#include <CEGUI.h>
#include <Ogre.h>
#include "services/logging/LoggingService.h"
#include "components/ogre/GUIManager.h"

namespace EmberOgre {

namespace Gui {

namespace Icons {

IconImageStoreEntry::IconImageStoreEntry(IconImageStore& iconImageStore, const PixelPos& pixelPosInImageset)
: mImage(0), mIconImageStore(iconImageStore), mPixelPosInImageset(pixelPosInImageset)
{
	createImage();
}

void IconImageStoreEntry::createImage()
{
	std::stringstream ss;
	ss << mPixelPosInImageset.first << "_" << mPixelPosInImageset.second;
	mImageName = ss.str();
	///we only want one element: the whole texture
	mIconImageStore.mImageset->defineImage(mImageName, CEGUI::Rect(mPixelPosInImageset.first, mPixelPosInImageset.second, mPixelPosInImageset.first + mIconImageStore.mIconSize, mPixelPosInImageset.second + mIconImageStore.mIconSize), CEGUI::Point(0,0));

	///assign our image element to the StaticImage widget
	mImage = &mIconImageStore.mImageset->getImage(mImageName);

}

const CEGUI::Image* IconImageStoreEntry::getImage()
{
	return mImage;
}

const CEGUI::Image* IconImageStoreEntry::getImage() const
{
	return mImage;
}


const std::string& IconImageStoreEntry::getImageName()
{
	return mImageName;
}

const std::string& IconImageStoreEntry::getImageName() const
{
	return mImageName;
}

Ogre::TexturePtr IconImageStoreEntry::getTexture()
{
	return mIconImageStore.mTexPtr;
}

IconImageStore::IconImageStore(const std::string& imagesetName)
: mImagesetName(imagesetName), mIconSize(64), mImageSize(256)
{
	createImageset();
	createEntries();
}


IconImageStore::~IconImageStore()
{
}

void IconImageStore::createImageset()
{

	mTexPtr = Ogre::TextureManager::getSingleton().createManual(mImagesetName, "Gui", Ogre::TEX_TYPE_2D, mImageSize, mImageSize, 0, Ogre::PF_A8R8G8B8,Ogre::TU_DYNAMIC);
	if (mTexPtr.isNull()) {
		S_LOG_WARNING("Could not create a texture.");
		return;
	}
	
	mCeguiTexture = GUIManager::getSingleton().getGuiRenderer()->createTexture(mTexPtr);
	
	///we need a imageset in order to create GUI elements from the ceguiTexture
	//S_LOG_VERBOSE("Creating new CEGUI imageset with name " << imageSetName + "_EntityCEGUITextureImageset");
	mImageset = CEGUI::ImagesetManager::getSingleton().createImageset(mImagesetName, mCeguiTexture);
	
}

void IconImageStore::createEntries()
{
	int entriesPerAxis(mImageSize / mIconSize);
	for (int x = 0; x < entriesPerAxis; ++x) {
		for (int y = 0; y < entriesPerAxis; ++y) {
			int pixelPosStartX = x * mIconSize;
			int pixelPosStartY = y * mIconSize;
			IconImageStoreEntry* entry = new IconImageStoreEntry(*this, IconImageStoreEntry::PixelPos(pixelPosStartX, pixelPosStartY));
			mIconImages.push_back(entry);
			mUnclaimedIconImages.push(entry);
		}
	}
}

size_t IconImageStore::getNumberOfUnclaimedIcons()
{
	return mUnclaimedIconImages.size();
}

IconImageStoreEntry* IconImageStore::claimImageEntry()
{
	if (!getNumberOfUnclaimedIcons()) {
		S_LOG_WARNING("Trying to claim image entry from store with no unclaimed entries.");
		return 0;
	}
	IconImageStoreEntry* entry = mUnclaimedIconImages.top();
	mUnclaimedIconImages.pop();
	return entry;
}

void IconImageStore::reclaimImageEntry(IconImageStoreEntry* imageEntry)
{
	mUnclaimedIconImages.push(imageEntry);
}

Ogre::Image::Box IconImageStoreEntry::getBox()
{
	Ogre::Image::Box box(mPixelPosInImageset.first, mPixelPosInImageset.second, mPixelPosInImageset.first + mIconImageStore.mIconSize, mPixelPosInImageset.second + mIconImageStore.mIconSize);
	return box;
}

// Ogre::TexturePtr IconImageStore::getTexture()
// {
// 	return mTexPtr;
// }


}

}

}
