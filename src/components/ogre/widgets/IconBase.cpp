//
// C++ Implementation: IconBase
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2005
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

#include "IconBase.h"

#include <CEGUIImagesetManager.h> 
#include <CEGUIImageset.h> 
#include <elements/CEGUIFrameWindow.h>
#include "../GUIManager.h"

using namespace CEGUI;
namespace EmberOgre {
namespace Gui {

IconBase::IconBase(const std::string& name, const Image* background, const Image* foreground, const Image* borderInactive, const Image* borderActive, UVector2 size)
: mContainer(0), mButton(0)
{

	mContainer = WindowManager::getSingleton().createWindow("DefaultGUISheet", "icons/" + name + "/container");
	mContainer->setSize(size);
	mContainer->setVisible(true);
	mContainer->setEnabled(true);
//	mContainer->setFrameEnabled(false);
//	mContainer->setBackgroundEnabled(false);
//	mContainer->setBackgroundColours(colour(1,1,1,0));

	mButton = static_cast<PushButton*>(WindowManager::getSingleton().createWindow(EmberOgre::GUIManager::getSingleton().getDefaultScheme() + "/BorderIconButton", "icons/" + name + "/button"));
	mButton->setSize(UVector2(UDim(1, 0), UDim(1, 0)));
	mButton->setPosition(UVector2(UDim(0, 0), UDim(0, 0)));
	mButton->setVisible(true);
	mButton->setEnabled(true);
	
	mButton->setProperty("BackImage", PropertyHelper::imageToString(background));
	mButton->setProperty("FrontImage", PropertyHelper::imageToString(foreground));
	mButton->setProperty("BorderNormalImage", PropertyHelper::imageToString(borderInactive));
	mButton->setProperty("BorderHoverImage", PropertyHelper::imageToString(borderActive));
	
	mContainer->addChildWindow(mButton);
	
	mButton->render();


}

IconBase::~IconBase()
{
	CEGUI::WindowManager::getSingleton().destroyWindow(mButton);
	CEGUI::WindowManager::getSingleton().destroyWindow(mContainer);
}

Window* IconBase::getContainer()
{
	return mContainer;
}

PushButton * IconBase::getButton()
{
	return mButton;
}

void IconBase::setForeground(const Image* image)
{
	mButton->setProperty("FrontImage", PropertyHelper::imageToString(image));
}


const Image* IconBase::loadImageFromImageset(const std::string & imagesetName, const std::string & image)
{
	Imageset* imageSet;
	if (!ImagesetManager::getSingleton().isImagesetPresent(imagesetName)) {
		try {
			std::string imagesetFileName("cegui/datafiles/imagesets/" + imagesetName + ".imageset");
			imageSet = ImagesetManager::getSingleton().createImageset(imagesetFileName);
		} catch (const std::exception& ex) {
			S_LOG_WARNING("Error when loading imageset " << imagesetName << ": " << ex.what());
			return 0;
		} catch (const CEGUI::Exception& ex) {
			S_LOG_WARNING("Error when loading imageset " << imagesetName << ": " << ex.getMessage().c_str());
			return 0;
		}
	} else {
		imageSet = ImagesetManager::getSingleton().getImageset(imagesetName);
	}
	
	return &imageSet->getImage(image);

}
}

}
