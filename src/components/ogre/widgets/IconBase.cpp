//
// C++ Implementation: IconBase
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2005
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "IconBase.h"

#include "../GUIManager.h"

using namespace CEGUI;
namespace Ember {
namespace OgreView {
namespace Gui {

IconBase::IconBase(const std::string& name, const Image* background, const Image* foreground, const Image* borderInactive, const Image* borderActive, USize size)
: mContainer(0), mButton(0)
{

	mContainer = WindowManager::getSingleton().createWindow("DefaultWindow", "icons/" + name + "/container");
	mContainer->setSize(size);
	mContainer->setVisible(true);
	mContainer->setEnabled(true);
//	mContainer->setFrameEnabled(false);
//	mContainer->setBackgroundEnabled(false);
//	mContainer->setBackgroundColours(colour(1,1,1,0));

	mButton = static_cast<PushButton*>(WindowManager::getSingleton().createWindow(OgreView::GUIManager::getSingleton().getDefaultScheme() + "/BorderIconButton", "icons/" + name + "/button"));
	mButton->setSize(USize(UDim(1, 0), UDim(1, 0)));
	mButton->setPosition(UVector2(UDim(0, 0), UDim(0, 0)));
	mButton->setVisible(true);
	mButton->setEnabled(true);

	mButton->setProperty("BackImage", PropertyHelper<CEGUI::Image*>::toString(background));
	mButton->setProperty("FrontImage", PropertyHelper<CEGUI::Image*>::toString(foreground));
	mButton->setProperty("BorderNormalImage", PropertyHelper<CEGUI::Image*>::toString(borderInactive));
	mButton->setProperty("BorderHoverImage", PropertyHelper<CEGUI::Image*>::toString(borderActive));

	mContainer->addChild(mButton);

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
	mButton->setProperty("FrontImage", PropertyHelper<CEGUI::Image*>::toString(image));
}


const Image* IconBase::loadImageFromImageset(const std::string & imagesetName, const std::string & image)
{
	if (ImageManager::getSingleton().isDefined(imagesetName + "/" + image)) {
		try {
			std::string imagesetFileName("cegui/datafiles/imagesets/" + imagesetName + ".imageset");
			ImageManager::getSingleton().loadImageset(imagesetFileName);
		} catch (const std::exception& ex) {
			S_LOG_WARNING("Error when loading imageset " << imagesetName << "." << ex);
			return 0;
		}
	}
	return &ImageManager::getSingleton().get(imagesetName + "/" + image);
}
}

}
}
