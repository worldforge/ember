//
// C++ Implementation: IconImageStore
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

#include "IconImageStore.h"
#include <CEGUI.h>
#include <Ogre.h>
#include "framework/LoggingInstance.h"
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
	mIconImageStore.mImageset->defineImage(mImageName, CEGUI::Rect(mPixelPosInImageset.first, mPixelPosInImageset.second, mPixelPosInImageset.first + mIconImageStore.mIconSize, mPixelPosInImageset.second + mIconImageStore.mIconSize), CEGUI::Point(0,0));

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

Ogre::PixelBox IconImageStoreEntry::getImagePixelBox()
{
	return mIconImageStore.getImage().getPixelBox().getSubVolume(getBox());
}


Ogre::Image::Box IconImageStoreEntry::getBox()
{
	Ogre::Image::Box box(mPixelPosInImageset.first, mPixelPosInImageset.second, mPixelPosInImageset.first + mIconImageStore.mIconSize, mPixelPosInImageset.second + mIconImageStore.mIconSize);
	return box;
}

Ogre::TRect<float> IconImageStoreEntry::getRelativeBox()
{
/*	float width(mIconImageStore.getImage()->getWidth());
	float height(mIconImageStore.getImage()->getHeight());*/
	
	float imageSize = static_cast<float>(mIconImageStore.mImageSize);
	
	Ogre::TRect<float> box(static_cast<float>(mPixelPosInImageset.first) / imageSize, static_cast<float>(mPixelPosInImageset.second) / imageSize, static_cast<float>(mPixelPosInImageset.first + mIconImageStore.mIconSize) / imageSize, static_cast<float>(mPixelPosInImageset.second + mIconImageStore.mIconSize) / imageSize);
	return box;
}


IconImageStore::IconImageStore(const std::string& imagesetName)
: mImagesetName(imagesetName)
, mIconSize(64)
, mImageSize(256)
, mImageDataStream(OGRE_NEW Ogre::MemoryDataStream(mImageSize * mImageSize * 4, true))
, mCeguiTexture(0)
, mImageset(0)
{
	createImageset();
	createEntries();
}

/**
Constructor for when we already have a texture of the whole icon.
*/
IconImageStore::IconImageStore(const std::string& imagesetName, Ogre::TexturePtr texPtr)
: mImagesetName(imagesetName)
, mTexPtr(texPtr)
, mImageDataStream(0)
, mCeguiTexture(0)
, mImageset(0)
{
	mCeguiTexture = GUIManager::getSingleton().getGuiRenderer()->createTexture(mTexPtr);
	
	///we need a imageset in order to create GUI elements from the ceguiTexture
	mImageset = CEGUI::ImagesetManager::getSingleton().createImageset(mImagesetName, mCeguiTexture);
	
	///we'll assume that height and width are the same
	mImageSize = texPtr->getWidth();
	mIconSize = mImageSize;
	
	///this will only create one entry
	createEntries();
}


IconImageStore::~IconImageStore()
{
	for (IconImageStoreEntryStore::iterator I(mIconImages.begin()); I != mIconImages.end(); ++I) {
		delete *I;
	}
	OGRE_DELETE mImageDataStream;
}

void IconImageStore::createImageset()
{

	///reset the image
	memset(mImageDataStream->getPtr(), '\0', mImageDataStream->size());
	mImage.loadDynamicImage(mImageDataStream->getPtr(), mImageSize, mImageSize, 1, Ogre::PF_A8R8G8B8);

	///no mipmaps to avoid problems when updating dynamically, also we don't really need it since this is a gui element which will be shown in the same resolution almost all of the time
	mTexPtr = Ogre::TextureManager::getSingleton().loadImage(mImagesetName, "Gui", mImage, Ogre::TEX_TYPE_2D, 0);
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

void IconImageStore::returnImageEntry(IconImageStoreEntry* imageEntry)
{
// 	if (std::find(mUnclaimedIconImages.begin(), mUnclaimedIconImages.end(), imageEntry) != mUnclaimedIconImages.end()) {
// 		S_LOG_WARNING("Trying to return an image entry which is unclaimed.");
// 	}
	if (std::find(mIconImages.begin(), mIconImages.end(), imageEntry) == mIconImages.end()) {
		S_LOG_WARNING("Trying to return an image entry which doesn't belong to this store.");
	}
	
	mUnclaimedIconImages.push(imageEntry);
}


Ogre::Image& IconImageStore::getImage()
{
	return mImage;
}

// Ogre::TexturePtr IconImageStore::getTexture()
// {
// 	return mTexPtr;
// }


}

}

}
