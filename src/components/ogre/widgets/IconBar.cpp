//
// C++ Implementation: IconBar
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2007
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

#include "IconBar.h"
#include "IconBase.h"

using namespace CEGUI;

namespace Ember {
namespace OgreView {

namespace Gui {

IconBar::IconBar(const std::string& name) :
        mWindow(WindowManager::getSingleton().createWindow("DefaultWindow", "iconbars/" + name)),
        mIconPadding(0) {

    mWindow->setUsingAutoRenderingSurface(true);
    /*	mWindow->setProperty("BackgroundEnabled", "false");
     mWindow->setProperty("FrameEnabled", "false");*/
}

IconBar::~IconBar() = default;

void IconBar::addIcon(IconBase* iconBase) {
    mIconBases.emplace_back(iconBase);
    mWindow->addChild(iconBase->getContainer());

    repositionIcons();

    iconBase->getContainer()->subscribeEvent(CEGUI::Window::EventShown, CEGUI::Event::Subscriber(&IconBar::iconVisibilityChanged, this));
    iconBase->getContainer()->subscribeEvent(CEGUI::Window::EventHidden, CEGUI::Event::Subscriber(&IconBar::iconVisibilityChanged, this));
}

void IconBar::removeIcon(IconBase* iconBase) {
    auto I = std::find_if(mIconBases.begin(), mIconBases.end(), [&](const std::unique_ptr<IconBase>& entry) {return entry.get() == iconBase;});
    if (I != mIconBases.end()) {
        mWindow->removeChild(iconBase->getContainer());
        I->release(); //When removing we'll also release the pointer, since the memory will be handled by other parts of the code.
        mIconBases.erase(I);
    }
    repositionIcons();
}

bool IconBar::iconVisibilityChanged(const CEGUI::EventArgs& e) {
    repositionIcons();
    return true;
}

CEGUI::Window* IconBar::getWindow() {
    return mWindow.get();
}

void IconBar::setIconPadding(int iconPadding) {
    mIconPadding = iconPadding;
}

void IconBar::repositionIcons() {
    float accumulatedWidth(0);
    float maxHeight(0);

    for (auto& icon : mIconBases) {
        if (icon->getContainer() && icon->getContainer()->isVisible()) {
            float absHeight = icon->getContainer()->getPixelSize().d_height;
            float absWidth = icon->getContainer()->getPixelSize().d_width;
            maxHeight = std::max<float>(maxHeight, absHeight);

            icon->getContainer()->setPosition(UVector2(UDim(0, accumulatedWidth), UDim(0, 0)));

            accumulatedWidth += absWidth + (float)mIconPadding;
        }
    }
    accumulatedWidth -= (float)mIconPadding;
    mWindow->setSize(USize(UDim(0, accumulatedWidth), UDim(0, maxHeight)));
    //We need to call this to guarantee that cegui correctly renders any newly added icons.
    mWindow->notifyScreenAreaChanged();
}

float IconBar::getAbsoluteHeight() {
    return mWindow->getPixelSize().d_height;
}

float IconBar::getAbsoluteWidth() {
    return mWindow->getPixelSize().d_width;
}

}

}
}
