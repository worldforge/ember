//
// C++ Implementation: StackableContainer
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

#include "StackableContainer.h"

using namespace CEGUI;

namespace Ember
{
namespace OgreView
{

namespace Gui
{

StackableContainer::StackableContainer(CEGUI::Window* window) :
		mWindow(window),
		mInnerContainerWindow(nullptr),
		mPadding(0),
		mFlowDirection(Vertical)
{
	if (mWindow) {
		mChildAddedConnection = mWindow->subscribeEvent(CEGUI::Window::EventChildAdded, CEGUI::Event::Subscriber(&StackableContainer::window_ChildAdded, this));
	}
}

StackableContainer::~StackableContainer()
{
	cleanup();
	if (mWindowDestructionConnection.isValid()) {
		mWindowDestructionConnection->disconnect();
		mWindowDestructionConnection = CEGUI::Event::Connection();
	}
}

void StackableContainer::cleanup()
{
	for (auto & childConnection : mChildConnections) {
		childConnection.second->disconnect();
	}
	mChildConnections.clear();
	if (mChildRemovedConnection.isValid()) {
		mChildRemovedConnection->disconnect();
		mChildRemovedConnection = CEGUI::Event::Connection();
	}
	if (mChildAddedConnection.isValid()) {
		mChildAddedConnection->disconnect();
		mChildAddedConnection = CEGUI::Event::Connection();
	}
}

CEGUI::Window* StackableContainer::getWindow()
{
	return mWindow;
}

void StackableContainer::setPadding(int padding)
{
	mPadding = padding;
}

int StackableContainer::getPadding() const
{
	return mPadding;
}

void StackableContainer::repositionWindows()
{
	if (mInnerContainerWindow) {
		float accumulatedWidth(0);
		float accumulatedHeight(0);
		float maxHeight(0);
		float maxWidth(0);

		//iterate over all child window and rearrange them
		size_t childCount = mInnerContainerWindow->getChildCount();
		for (size_t i = 0; i < childCount; ++i) {
			CEGUI::Window* childWindow = mInnerContainerWindow->getChildAtIdx(i);
			//only use those windows that are visible
			if (childWindow->isVisible()) {
				float absHeight = childWindow->getPixelSize().d_height;
				float absWidth = childWindow->getPixelSize().d_width;
				if (mFlowDirection == Horizontal) {
					maxHeight = std::max<float>(maxHeight, absHeight);
					childWindow->setPosition(UVector2(UDim(0, accumulatedWidth), UDim(0, 0)));
					accumulatedWidth += absWidth + mPadding;
				} else {
					maxWidth = std::max<float>(maxWidth, absWidth);
					childWindow->setPosition(UVector2(UDim(0, 0), UDim(0, accumulatedHeight)));
					accumulatedHeight += absHeight + mPadding;
				}
			}
		}
		if (mFlowDirection == Horizontal) {
			accumulatedWidth -= mPadding;
			if (mInnerContainerWindow->getWidth().d_scale != 1) {
				mInnerContainerWindow->setWidth(UDim(0, accumulatedWidth));
			}
		} else {
			accumulatedHeight -= mPadding;
			if (mInnerContainerWindow->getHeight().d_scale != 1) {
				mInnerContainerWindow->setHeight(UDim(0, accumulatedHeight));
			}
		}
		if (mInnerContainerWindow->getParent()) {
			mInnerContainerWindow->getParent()->performChildWindowLayout();
		}
	}
}

float StackableContainer::getAbsoluteHeight()
{
	return mWindow->getPixelSize().d_height;
}

float StackableContainer::getAbsoluteWidth()
{
	return mWindow->getPixelSize().d_width;
}

void StackableContainer::setFlowDirection(StackableContainer::FlowDirection flowDirection)
{
	mFlowDirection = flowDirection;
}

StackableContainer::FlowDirection StackableContainer::getFlowDirection() const
{
	return mFlowDirection;
}

void StackableContainer::setInnerContainerWindow(CEGUI::Window* window)
{
	mInnerContainerWindow = window;
	mChildRemovedConnection = mInnerContainerWindow->subscribeEvent(CEGUI::Window::EventChildRemoved, CEGUI::Event::Subscriber(&StackableContainer::window_ChildRemoved, this));
	mWindowDestructionConnection = mInnerContainerWindow->subscribeEvent(CEGUI::Window::EventDestructionStarted, CEGUI::Event::Subscriber(&StackableContainer::window_DestructionStarted, this));

	size_t childCount = mInnerContainerWindow->getChildCount();
	for (size_t i = 0; i < childCount; ++i) {
		CEGUI::Window* childWindow = mInnerContainerWindow->getChildAtIdx(i);
		mChildConnections.insert(ConnectorStore::value_type(childWindow, childWindow->subscribeEvent(CEGUI::Window::EventSized, CEGUI::Event::Subscriber(&StackableContainer::childwindow_Sized, this))));
		mChildConnections.insert(ConnectorStore::value_type(childWindow, childWindow->subscribeEvent(CEGUI::Window::EventShown, CEGUI::Event::Subscriber(&StackableContainer::childwindow_Sized, this))));
		mChildConnections.insert(ConnectorStore::value_type(childWindow, childWindow->subscribeEvent(CEGUI::Window::EventHidden, CEGUI::Event::Subscriber(&StackableContainer::childwindow_Sized, this))));
	}
}

bool StackableContainer::window_ChildAdded(const CEGUI::EventArgs& e)
{
	const auto& windowEventArg = static_cast<const WindowEventArgs&>(e);
	if (!mInnerContainerWindow) {
		setInnerContainerWindow(windowEventArg.window->getParent());
	} else {
		//if we've called setInnerContainerWindow we would already have connected the new child window
		mChildConnections.insert(ConnectorStore::value_type(windowEventArg.window, windowEventArg.window->subscribeEvent(CEGUI::Window::EventSized, CEGUI::Event::Subscriber(&StackableContainer::childwindow_Sized, this))));
		mChildConnections.insert(ConnectorStore::value_type(windowEventArg.window, windowEventArg.window->subscribeEvent(CEGUI::Window::EventShown, CEGUI::Event::Subscriber(&StackableContainer::childwindow_Sized, this))));
		mChildConnections.insert(ConnectorStore::value_type(windowEventArg.window, windowEventArg.window->subscribeEvent(CEGUI::Window::EventHidden, CEGUI::Event::Subscriber(&StackableContainer::childwindow_Sized, this))));
	}
	repositionWindows();
	return true;
}

bool StackableContainer::window_ChildRemoved(const CEGUI::EventArgs& e)
{
	const auto& windowEventArg = static_cast<const WindowEventArgs&>(e);
	auto ret = mChildConnections.equal_range(windowEventArg.window);

	for (auto I = ret.first; I != ret.second; ++I) {
		I->second->disconnect();
	}

	mChildConnections.erase(ret.first, ret.second);

	repositionWindows();
	return true;
}

bool StackableContainer::childwindow_Sized(const CEGUI::EventArgs& e)
{
	repositionWindows();
	return true;
}

bool StackableContainer::window_DestructionStarted(const CEGUI::EventArgs& e)
{
	cleanup();
	return true;
}

void StackableContainer::disconnect()
{
	cleanup();
}

}

}
}
