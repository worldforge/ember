//
// C++ Implementation: EntityCEGUITexture
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2005
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
#include "EntityCEGUITexture.h"
#include <CEGUIWindowManager.h>
#include <CEGUIImagesetManager.h> 
#include <CEGUIImageset.h> 

#include "../EmberOgre.h"
#include "../GUIManager.h"
#include "../model/Model.h"
#include "../SimpleRenderContext.h"

#include "framework/Exception.h"
#include <OgreBitwise.h>


namespace EmberOgre {
namespace Gui {



EntityCEGUITexture::EntityCEGUITexture(const std::string& imageSetName, int width, int height)
: mImage(0), mImageSet(0), mWidth(width), mHeight(height), mRenderContext(new SimpleRenderContext(imageSetName, width, height)), mCeguiTexture(0)
{
	createImage(imageSetName);
}

EntityCEGUITexture::~EntityCEGUITexture()
{
}


const CEGUI::Image* EntityCEGUITexture::getImage() const
{
	return mImage;
}

void EntityCEGUITexture::createImage(const std::string& imageSetName)
{
	///create a CEGUI texture from our Ogre texture
	S_LOG_VERBOSE("Creating new CEGUI texture from Ogre texture.");
	Ogre::TexturePtr texturePtr(mRenderContext->getTexture());
	mCeguiTexture = GUIManager::getSingleton().getGuiRenderer()->createTexture(texturePtr);
	
	///we need a imageset in order to create GUI elements from the ceguiTexture
	S_LOG_VERBOSE("Creating new CEGUI imageset with name " << imageSetName + "_EntityCEGUITextureImageset");
	mImageSet = CEGUI::ImagesetManager::getSingleton().createImageset(imageSetName + "_EntityCEGUITextureImageset", mCeguiTexture);
	
	///we only want one element: the whole texture
	///the width and height of the texture differs from the supplied width of this instance since it will have been adjusted to a power-of-two size
	mImageSet->defineImage("full_image", CEGUI::Rect(0, 0, texturePtr->getWidth(), texturePtr->getHeight()), CEGUI::Point(0,0));

	///assign our image element to the StaticImage widget
	mImage = &mImageSet->getImage("full_image");
	

}


SimpleRenderContext* EntityCEGUITexture::getRenderContext()
{
	return mRenderContext.get();
}




}
}
