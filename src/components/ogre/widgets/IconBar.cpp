//
// C++ Implementation: IconBar
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2007
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

#include "IconBar.h"
#include <CEGUI.h>
#include "IconBase.h"

using namespace CEGUI;

namespace EmberOgre {

namespace Gui {

IconBar::IconBar(const std::string& name)
: mIconPadding(0)
{
	mWindow = WindowManager::getSingleton().createWindow("DefaultGUISheet", "iconbars/" + name);
/*	mWindow->setProperty("BackgroundEnabled", "false");
	mWindow->setProperty("FrameEnabled", "false");*/
}

IconBar::~IconBar()
{
	for (std::vector<IconBase*>::iterator I = mIconBases.begin(); I != mIconBases.end(); ++I) {
		delete *I;
	}
	CEGUI::WindowManager::getSingleton().destroyWindow(mWindow);
}


void IconBar::addIcon(IconBase* iconBase)
{
	mIconBases.push_back(iconBase);
	mWindow->addChildWindow(iconBase->getContainer());
	repositionIcons();
}
void IconBar::removeIcon(IconBase* iconBase)
{
	IconBaseStore::iterator I = std::find(mIconBases.begin(), mIconBases.end(), iconBase);
	if (I != mIconBases.end()) {
		mWindow->removeChildWindow(iconBase->getContainer());
		mIconBases.erase(I);
	}
	repositionIcons();
}

CEGUI::Window* IconBar::getWindow()
{
	return mWindow;
}

void IconBar::setIconPadding(int iconPadding)
{
	mIconPadding = iconPadding;
}


void IconBar::repositionIcons()
{
	float accumulatedWidth(0);
	float maxHeight(0);
	
	for(IconBaseStore::iterator I(mIconBases.begin()); I != mIconBases.end(); ++I) {
		IconBase* icon = (*I);
		const UVector2& size = icon->getContainer()->getSize();
		float absHeight = size.d_y.asAbsolute(0);
		float absWidth = size.d_x.asAbsolute(0);
		maxHeight = std::max<float>(maxHeight, absHeight);
		
		icon->getContainer()->setPosition(UVector2(UDim(0, accumulatedWidth), UDim(0,0)));
		
		accumulatedWidth += absWidth + mIconPadding;
	}
	accumulatedWidth -= mIconPadding;
	mWindow->setSize(UVector2(UDim(0, accumulatedWidth), UDim(0,maxHeight)));
}

float IconBar::getAbsoluteHeight()
{
	return mWindow->getSize().d_y.asAbsolute(0);
}

float IconBar::getAbsoluteWidth()
{
	return mWindow->getSize().d_x.asAbsolute(0);
}


}

}
