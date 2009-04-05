//
// C++ Implementation: AssetsManager
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

#include "AssetsManager.h"
#include <OgreCEGUIRenderer.h>
#include <OgreCEGUITexture.h>
#include <OgreMaterialSerializer.h>
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
{
// 	createTextureImage();
}


AssetsManager::~AssetsManager()
{
}

TexturePair AssetsManager::showTexture(const std::string textureName)
{
// 	if (!mOgreCEGUITexture) {
// 		S_LOG_WARNING("You must first create a valid OgreCEGUITexture instance.");
// 		return;
// 	}
	if (Ogre::TextureManager::getSingleton().resourceExists(textureName)) {
		Ogre::TexturePtr texturePtr = static_cast<Ogre::TexturePtr>(Ogre::TextureManager::getSingleton().getByName(textureName));
		if (!texturePtr.isNull()) {
			if (!texturePtr->isLoaded()) {
				try {
					texturePtr->load();
				} catch (...) {
					S_LOG_WARNING("Error when loading " << textureName << ". This texture will not be shown.");
					return TexturePair();
				}
			}
			std::string textureName (texturePtr->getName());
			std::string imageSetName(textureName + "_AssetsManager");

			return createTextureImage(texturePtr, imageSetName);
// 			mOgreCEGUITexture->setOgreTexture(texturePtr);
		}
	}
	return TexturePair();
	
}


TexturePair AssetsManager::createTextureImage(Ogre::TexturePtr texturePtr, const std::string& imageSetName)
{
// 	if (mOgreCEGUITexture) {
// 		GUIManager::getSingleton().getGuiRenderer()->destroyTexture(mOgreCEGUITexture);
// 		mOgreCEGUITexture = 0;
// 	}
	
	
	CEGUI::Imageset* textureImageset;
	
	if (CEGUI::ImagesetManager::getSingleton().isImagesetPresent(imageSetName)) {
		textureImageset = CEGUI::ImagesetManager::getSingleton().getImageset(imageSetName);
	} else {
		///create a CEGUI texture from our Ogre texture
		S_LOG_VERBOSE("Creating new CEGUI texture from Ogre texture.");
		CEGUI::Texture* ogreCEGUITexture = GUIManager::getSingleton().getGuiRenderer()->createTexture(texturePtr);
		
		///we need a imageset in order to create GUI elements from the ceguiTexture
		S_LOG_VERBOSE("Creating new CEGUI imageset with name " << imageSetName);
		textureImageset = CEGUI::ImagesetManager::getSingleton().createImageset(imageSetName , ogreCEGUITexture);
		
		///we only want one element: the whole texture
		textureImageset->defineImage("full_image", CEGUI::Rect(0, 0, texturePtr->getWidth(), texturePtr->getHeight()), CEGUI::Point(0,0));
	}
	///assign our image element to the StaticImage widget
	const CEGUI::Image* textureImage = &textureImageset->getImage("full_image");
	
	return TexturePair(texturePtr, textureImage, textureImageset);

}

std::string AssetsManager::materialAsText(Ogre::MaterialPtr material)
{
	if (material.isNull()) {
		return "";
	}
	Ogre::MaterialSerializer serializer;
	serializer.queueForExport(material, true, false);
	return serializer.getQueuedAsString();
}


// bool AssetsManager::exportTexture(Ogre::TexturePtr texturePtr)
// {
//  getRenderTarget()->writeContentsToFile();
// }


TexturePair::TexturePair(Ogre::TexturePtr ogreTexture, const CEGUI::Image* textureImage, CEGUI::Imageset* textureImageset)
: mOgreTexture(ogreTexture)
// , mOgreCEGUITexture(ogreCEGUITexture)
, mTextureImage(textureImage)
, mTextureImageset(textureImageset)
{
}
	
TexturePair::TexturePair()
: mOgreTexture(0)
// , mOgreCEGUITexture(0)
, mTextureImage(0)
, mTextureImageset(0)
{
}
	
Ogre::TexturePtr TexturePair::getOgreTexture() const
{
	return mOgreTexture;
}

// CEGUI::Texture* TexturePair::getOgreCEGUITexture() const
// {
// 	return OgreCEGUITexture;
// }

const CEGUI::Image* TexturePair::getTextureImage() const
{
	return mTextureImage;
}

CEGUI::Imageset* TexturePair::getTextureImageset() const
{
	return mTextureImageset;
}

bool TexturePair::hasData()
{
	return mTextureImageset != 0;
}


}

}
