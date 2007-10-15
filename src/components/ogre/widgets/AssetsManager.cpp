//
// C++ Implementation: AssetsManager
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
#include "AssetsManager.h"
#include <OgreCEGUIRenderer.h>
#include <OgreCEGUITexture.h>
#include "../EmberOgrePrerequisites.h"

#include <CEGUIImagesetManager.h> 
#include <CEGUIImageset.h> 

#include "../EmberOgre.h"
#include "../GUIManager.h"

#include "framework/Exception.h"
// #include <OgreBitwise.h>


namespace EmberOgre {

namespace Gui {

AssetsManager::AssetsManager()
: mOgreCEGUITexture(0), mTextureImage(0), mTextureImageset(0)
{
// 	createTextureImage();
}


AssetsManager::~AssetsManager()
{
}

bool AssetsManager::showTexture(const std::string textureName)
{
// 	if (!mOgreCEGUITexture) {
// 		S_LOG_WARNING("You must first create a valid OgreCEGUITexture instance.");
// 		return;
// 	}
	if (Ogre::TextureManager::getSingleton().resourceExists(textureName)) {
		Ogre::TexturePtr texturePtr = static_cast<Ogre::TexturePtr>(Ogre::TextureManager::getSingleton().getByName(textureName));
		if (!texturePtr.isNull()) {
			try {
				texturePtr->load();
			} catch (...) {
				S_LOG_WARNING("Error when loading " << textureName << ". This texture will not be shown.");
				return false;
			}
			createTextureImage(texturePtr);
			return true;
// 			mOgreCEGUITexture->setOgreTexture(texturePtr);
		}
	}
	return false;
	
}


void AssetsManager::createTextureImage(Ogre::TexturePtr texturePtr)
{
	std::string textureName (texturePtr->getName());
// 	if (mOgreCEGUITexture) {
// 		GUIManager::getSingleton().getGuiRenderer()->destroyTexture(mOgreCEGUITexture);
// 		mOgreCEGUITexture = 0;
// 	}
	
	std::string imageSetName(textureName + "_AssetsManager");
	
	if (CEGUI::ImagesetManager::getSingleton().isImagesetPresent(imageSetName)) {
		mTextureImageset = CEGUI::ImagesetManager::getSingleton().getImageset(imageSetName);
	} else {
		///create a CEGUI texture from our Ogre texture
		S_LOG_VERBOSE("Creating new CEGUI texture from Ogre texture.");
		mOgreCEGUITexture = GUIManager::getSingleton().getGuiRenderer()->createTexture(texturePtr);
		
		///we need a imageset in order to create GUI elements from the ceguiTexture
		S_LOG_VERBOSE("Creating new CEGUI imageset with name " << imageSetName);
		mTextureImageset = CEGUI::ImagesetManager::getSingleton().createImageset(imageSetName , mOgreCEGUITexture);
		
		///we only want one element: the whole texture
		mTextureImageset->defineImage("full_image", CEGUI::Rect(0, 0, texturePtr->getWidth(), texturePtr->getHeight()), CEGUI::Point(0,0));
	}
	///assign our image element to the StaticImage widget
	mTextureImage = &mTextureImageset->getImage("full_image");

}

const CEGUI::Image* AssetsManager::getCEGUIImage()
{
	return mTextureImage;
}


}

}
