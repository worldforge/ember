//
// C++ Implementation: StackableContainer
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
#include "StackableContainer.h"

using namespace CEGUI;

namespace EmberOgre {

namespace Gui {

StackableContainer::StackableContainer(CEGUI::Window* window)
: mWindow(window), mInnerContainerWindow(0), mPadding(0), mFlowDirection(Vertical)
{
	if (window) {
		window->subscribeEvent(CEGUI::Window::EventChildAdded, CEGUI::Event::Subscriber(&StackableContainer::window_ChildAdded, this));
		//window->subscribeEvent(CEGUI::Window::EventChildRemoved, CEGUI::Event::Subscriber(&StackableContainer::window_ChildRemoved, this));
	} 
}


StackableContainer::~StackableContainer()
{
}


// void StackableContainer::addWindow(CEGUI::Window* window)
// {
// 	mWindows.push_back(window);
// 	mWindow->addChildWindow(iconBase->getContainer());
// 	repositionIcons();
// }
// void StackableContainer::removeIcon(IconBase* iconBase)
// {
// 	IconBaseStore::iterator I = std::find(mIconBases.begin(), mIconBases.end(), iconBase);
// 	if (I != mIconBases.end()) {
// 		mWindow->removeChildWindow(iconBase->getContainer());
// 		mIconBases.erase(I);
// 	}
// 	repositionIcons();
// }

CEGUI::Window* StackableContainer::getWindow()
{
	return mWindow;
}

void StackableContainer::setPadding(int padding)
{
	mPadding = padding;
}


void StackableContainer::repositionWindows()
{
	float accumulatedWidth(0);
	float accumulatedHeight(0);
	float maxHeight(0);
	float maxWidth(0);
	size_t childCount = mInnerContainerWindow->getChildCount();
 	for(size_t i = 0; i < childCount; ++i) {
// 	for (WindowStore::iterator I = mWindows.begin(); I != mWindows.end(); ++I) {
		CEGUI::Window* childWindow = mInnerContainerWindow->getChildAtIdx(i);
// 		CEGUI::Window* childWindow = *I;
		float absHeight = childWindow->getHeight().asAbsolute(0);
		float absWidth = childWindow->getWidth().asAbsolute(0);
		if (mFlowDirection == Horizontal) {
			maxHeight = std::max<float>(maxHeight, absHeight);
			childWindow->setPosition(UVector2(UDim(0, accumulatedWidth), UDim(0,0)));
			accumulatedWidth += absWidth + mPadding;
		} else {
			maxWidth= std::max<float>(maxWidth, absWidth);
			childWindow->setPosition(UVector2(UDim(0, 0), UDim(0,accumulatedHeight)));
			accumulatedHeight += absHeight + mPadding;
		}
	}
	if (mFlowDirection == Horizontal) {
		accumulatedWidth -= mPadding;
		mInnerContainerWindow->setSize(UVector2(UDim(0, accumulatedWidth), UDim(0,maxHeight)));
	} else {
		accumulatedHeight -= mPadding;
		mInnerContainerWindow->setSize(UVector2(UDim(0, maxWidth), UDim(0,accumulatedHeight)));
	}
	if (mInnerContainerWindow->getParent()) {
		mInnerContainerWindow->getParent()->performChildWindowLayout();
	}
}

float StackableContainer::getAbsoluteHeight()
{
	return mWindow->getSize().d_y.asAbsolute(0);
}

float StackableContainer::getAbsoluteWidth()
{
	return mWindow->getSize().d_x.asAbsolute(0);
}

void StackableContainer::setFlowDirection(StackableContainer::FlowDirection flowDirection)
{
	mFlowDirection = flowDirection;
}

StackableContainer::FlowDirection StackableContainer::getFlowDirection() const
{
	return mFlowDirection;
}


bool StackableContainer::window_ChildAdded(const CEGUI::EventArgs& e)
{
	const WindowEventArgs& windowEventArg = static_cast<const WindowEventArgs&>(e);
	if (!mInnerContainerWindow) {
		mInnerContainerWindow = windowEventArg.window->getParent();
		mInnerContainerWindow->subscribeEvent(CEGUI::Window::EventChildRemoved, CEGUI::Event::Subscriber(&StackableContainer::window_ChildRemoved, this));
	}
	repositionWindows();
	return true;
}

bool StackableContainer::window_ChildRemoved(const CEGUI::EventArgs& e)
{
// 	const WindowEventArgs& windowEventArg = static_cast<const WindowEventArgs&>(e);
	repositionWindows();
	return true;
}



}

}
