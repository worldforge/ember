//
// C++ Implementation: IconBase
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
#include "IconBase.h"

#include <CEGUIImagesetManager.h> 
#include <CEGUIImageset.h> 
#include <elements/CEGUIFrameWindow.h>
//#include "../EmberOgre.h"
#include "../GUIManager.h"


namespace EmberOgre {

IconBase::IconBase(const std::string& name, const CEGUI::Image* background, const CEGUI::Image* foreground, const CEGUI::Image* borderInactive, const CEGUI::Image* borderActive)
{

	mContainer = CEGUI::WindowManager::getSingleton().createWindow("DefaultGUISheet", "icons/" + name + "/container");
	mContainer->setSize(CEGUI::Absolute, CEGUI::Size(48, 48));
	mContainer->setVisible(true);
	mContainer->setEnabled(true);
//	mContainer->setFrameEnabled(false);
//	mContainer->setBackgroundEnabled(false);
//	mContainer->setBackgroundColours(CEGUI::colour(1,1,1,0));

	mButton = static_cast<CEGUI::PushButton*>(CEGUI::WindowManager::getSingleton().createWindow((CEGUI::utf8*)"TaharezLook/Button", "icons/" + name + "/button"));
	mButton->setSize(CEGUI::Relative, CEGUI::Size(1, 1));
	mButton->setPosition(CEGUI::Point(0.0f, 0.0f));
	mButton->setStandardImageryEnabled(false);
	
	//create the chain for the normal image
	mRenderNormalBack = makeRenderableImage(background);
	mRenderNormalFront = makeRenderableImage(foreground);
	mRenderNormalBorder = makeRenderableImage(borderInactive);
	mRenderNormalBack->setNextElement(mRenderNormalFront);
	mRenderNormalFront->setNextElement(mRenderNormalBorder);
	mButton->setNormalImage(mRenderNormalBack);
	
	//create the chain for the hover image
	mRenderActiveBack = makeRenderableImage(background);
	mRenderActiveFront = makeRenderableImage(foreground);
	mRenderActiveBorder = makeRenderableImage(borderActive);
	mRenderActiveBack->setNextElement(mRenderActiveFront);
	mRenderActiveFront->setNextElement(mRenderActiveBorder);
	mButton->setHoverImage(mRenderActiveBack);
	mButton->setPushedImage(mRenderActiveBack);
	
	mContainer->addChildWindow(mButton);
	
	mButton->render();


}

IconBase::~IconBase()
{
}

void IconBase::setForeground(const CEGUI::Image* image)
{
	mRenderNormalFront->setImage(image);
	mRenderActiveFront->setImage(image);
}


CEGUI::RenderableImage* IconBase::makeRenderableImage(const CEGUI::Image* image)
{
	CEGUI::RenderableImage* renderImage = new CEGUI::RenderableImage();
	renderImage->setImage(image);
	renderImage->setRect(CEGUI::Rect(0, 0, 48, 48));
	return renderImage;
}

CEGUI::Window* IconBase::getContainer()
{
	return mContainer;
}

CEGUI::PushButton * IconBase::getButton()
{
	return mButton;
}

const CEGUI::Image* IconBase::loadImageFromImageset(const std::string & imagesetName, const std::string & image)
{
	CEGUI::Imageset* imageSet;
	if (!CEGUI::ImagesetManager::getSingleton().isImagesetPresent(imagesetName)) {
		try {
			std::string imagesetFileName = "cegui/datafiles/imagesets/" + imagesetName + ".imageset";
			imageSet = CEGUI::ImagesetManager::getSingleton().createImageset(imagesetFileName);
		} catch (const Ogre::Exception&) {
			return 0;
		}
	} else {
		imageSet = CEGUI::ImagesetManager::getSingleton().getImageset(imagesetName);
	}
	
	return &imageSet->getImage(image);

}


}
