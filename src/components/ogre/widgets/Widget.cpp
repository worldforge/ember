//
// C++ Implementation: Vector3Adapter
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

#include "Widget.h"

#include "../GUIManager.h"
#include "../EmberOgre.h"

#include "framework/ConsoleBackend.h"

#include <CEGUI/Window.h>
#include <CEGUI/WindowManager.h>
#include <CEGUI/widgets/FrameWindow.h>
#include <CEGUI/widgets/PushButton.h>


using namespace CEGUI;
namespace Ember {
namespace OgreView {
namespace Gui {

Widget::Widget(GUIManager& guiManager) : mCommandSuffix(""),
										 mMainWindow(nullptr),
										 mGuiManager(guiManager),
										 mOriginalWindowAlpha(1.0f),
										 mActiveWindowIsOpaque(true),
										 mFirstTabWindow(nullptr),
										 mLastTabWindow(nullptr),
										 mWindowHasBeenShown(false) {
}


Widget::~Widget() {
	if (!mCommandSuffix.empty()) {
		ConsoleBackend::getSingletonPtr()->deregisterCommand("show_" + mCommandSuffix);
		ConsoleBackend::getSingletonPtr()->deregisterCommand("hide_" + mCommandSuffix);
		ConsoleBackend::getSingletonPtr()->deregisterCommand("toggleVisibility_" + mCommandSuffix);
	}
}

void Widget::frameStarted(const Ogre::FrameEvent& evt) {
	EventFrameStarted.emit(evt.timeSinceLastFrame);
}

CEGUI::Window* Widget::getMainSheet() {
	return mGuiManager.getMainSheet();
}

CEGUI::Window* Widget::loadMainSheet(const std::string& filename, const std::string& prefix) {
	mPrefix = prefix;
	std::string finalFileName(mGuiManager.getLayoutDir() + filename);
	try {
		mMainWindow = UniqueWindowPtr<CEGUI::Window>(CEGUI::WindowManager::getSingleton().loadLayoutFromFile(finalFileName));
	} catch (const std::exception& ex) {
		S_LOG_FAILURE("Error when loading from " << filename << "." << ex);
		throw ex;
	} catch (...) {
		S_LOG_FAILURE("Unknown error when loading from " << filename << ".");
		throw;
	}
	//Main window is handled by ourselves.
	mMainWindow->setDestroyedByParent(false);
	mMainWindow->setName(prefix);
	mOriginalWindowAlpha = mMainWindow->getAlpha();
	getMainSheet()->addChild(mMainWindow.get());
	BIND_CEGUI_EVENT(mMainWindow, CEGUI::FrameWindow::EventActivated, Widget::MainWindow_Activated)
	BIND_CEGUI_EVENT(mMainWindow, CEGUI::FrameWindow::EventDeactivated, Widget::MainWindow_Deactivated)
	//we want to catch all click events, so we'll listen for the mouse button down event
	BIND_CEGUI_EVENT(mMainWindow, CEGUI::Window::EventMouseButtonDown, Widget::MainWindow_MouseButtonDown)
	if (mMainWindow->isVisible()) {
		onEventFirstTimeShown();
	} else {
		//Set it up to listen for the first time the window is shown.
		BIND_CEGUI_EVENT(mMainWindow, CEGUI::Window::EventShown, Widget::MainWindow_Shown)
	}

	return mMainWindow.get();
}

void Widget::onEventFirstTimeShown() {
	mWindowHasBeenShown = true;
	EventFirstTimeShown.emit();
}

CEGUI::Window* Widget::getWindow(const std::string& windowName, bool throwIfNotFound) {
	try {
		if (!mMainWindow) {
			S_LOG_WARNING("Trying to get a window (" + windowName + ") on widget that has no main sheet loaded (" << mPrefix << ").");
			return nullptr;
		}
		assert(mMainWindow && "You must call loadMainSheet(...) before you can call this method.");
		CEGUI::Window* window = mMainWindow->getChildRecursive(windowName);
		if (!window) {
			S_LOG_WARNING("The window with id " << windowName << ", located under widget " << mPrefix << " does not exist.");
		}
		return window;
	} catch (const CEGUI::Exception&) {
		S_LOG_WARNING("The window " << windowName << " doesn't exist.");
		if (throwIfNotFound) {
			throw Exception("The window '" + windowName + "' could not be found.");
		}
		return nullptr;
	}

}

CEGUI::Window* Widget::createWindow(const std::string& windowType) {
	return mGuiManager.createWindow(windowType);
}

CEGUI::Window* Widget::createWindow(const std::string& windowType, const std::string& windowName) {
	return mGuiManager.createWindow(windowType, windowName);
}

void Widget::registerConsoleVisibilityToggleCommand(const std::string& commandSuffix) {
	mCommandSuffix = commandSuffix;
	ConsoleBackend::getSingletonPtr()->registerCommand("show_" + commandSuffix, this, std::string("Shows the ") + mCommandSuffix + " window.");
	ConsoleBackend::getSingletonPtr()->registerCommand("hide_" + commandSuffix, this, std::string("Hides the ") + mCommandSuffix + " window.");
	ConsoleBackend::getSingletonPtr()->registerCommand("toggleVisibility_" + commandSuffix, this, std::string("Toggles visibility of the ") + mCommandSuffix + " window.");
}

void Widget::runCommand(const std::string& command, const std::string& args) {
	if (command == "show_" + mCommandSuffix) {
		show();
	} else if (command == "hide_" + mCommandSuffix) {
		hide();
	} else if (command == "toggleVisibility_" + mCommandSuffix) {
		toggleVisibility();
	}
}

void Widget::show() {
	//removing and attaching the window is probably more efficient when it's hidden (i.e. it won't get any events at all and so on)
	if (mMainWindow) {
		getMainSheet()->addChild(mMainWindow.get());
		mMainWindow->setVisible(true);
	}
}

void Widget::hide() {
	//see comment in show()
	if (mMainWindow) {
		getMainSheet()->removeChild(mMainWindow.get());
		mMainWindow->setVisible(false);
	}
}

void Widget::toggleVisibility() {
	if (!isVisible()) {
		show();
	} else {
		hide();
	}
}

bool Widget::isVisible() {
	if (mMainWindow) {
		return mMainWindow->isVisible();
	}
	return false;
}

bool Widget::isActive() {
	if (mMainWindow) {
		return mMainWindow->isActive();
	}
	return false;
}

bool Widget::MainWindow_CloseClick(const CEGUI::EventArgs& args) {
	hide();
	return true;
}

void Widget::enableCloseButton() {
	assert(mMainWindow);
	BIND_CEGUI_EVENT(mMainWindow, CEGUI::FrameWindow::EventCloseClicked, Widget::MainWindow_CloseClick)
}

CEGUI::Window* Widget::getMainWindow() {
	return mMainWindow.get();
}

bool Widget::MainWindow_MouseButtonDown(const CEGUI::EventArgs& args) {
	//we'll return true here to prevent the event from propagating to other windows
	return true;
}

bool Widget::MainWindow_Activated(const CEGUI::EventArgs& args) {
	if (mMainWindow && mActiveWindowIsOpaque) {
		mMainWindow->setAlpha(1.0);

	}
	return true;
}

bool Widget::MainWindow_Deactivated(const CEGUI::EventArgs& args) {
	if (mMainWindow && mActiveWindowIsOpaque) {
		mMainWindow->setAlpha(mOriginalWindowAlpha);
	}
	return true;
}

bool Widget::MainWindow_Shown(const CEGUI::EventArgs& args) {
	if (!mWindowHasBeenShown) {
		onEventFirstTimeShown();
	}
	return true;
}


bool Widget::getIsActiveWindowOpaque() const {
	return mActiveWindowIsOpaque;
}

void Widget::setIsActiveWindowOpaque(bool isOpaque) {
	mActiveWindowIsOpaque = isOpaque;
}

const std::string& Widget::getDefaultScheme() const {
	return mGuiManager.getDefaultScheme();
}


bool Widget::TabbableWindow_KeyDown(const CEGUI::EventArgs& args) {
	auto& keyEventArgs = dynamic_cast<const CEGUI::KeyEventArgs&>(args);
	if (keyEventArgs.scancode == CEGUI::Key::Tab) {
		//find the window in the list of tabbable windows
		CEGUI::Window* activeWindow = mMainWindow->getActiveChild();
		if (activeWindow) {
//				WindowMap::iterator I = std::find(mTabOrder.begin(), mTabOrder.end(), activeWindow);
			auto I = mTabOrder.find(activeWindow);
			if (I != mTabOrder.end()) {
				I->second->activate();
				//we don't want to process the event any more, in case something else will try to interpret the tab event to also change the focus
				Input::getSingleton().suppressFurtherHandlingOfCurrentEvent();
				return true;
			}
		}
	} else if (keyEventArgs.scancode == CEGUI::Key::Return) {
		//iterate through all enter buttons, and if anyone is visible, activate it
		for (auto& enterButton : mEnterButtons) {
			if (enterButton->isVisible()) {
				CEGUI::Window* window = enterButton;
				WindowEventArgs eventArgs(window);
				window->fireEvent(PushButton::EventClicked, eventArgs, PushButton::EventNamespace);
				break;
			}
		}
	}
	return false;
}

void Widget::addTabbableWindow(CEGUI::Window* window) {
	if (!mFirstTabWindow) {
		mFirstTabWindow = window;
	}
	if (mLastTabWindow) {
		mTabOrder.insert(WindowMap::value_type(mLastTabWindow, window));
	}
	mLastTabWindow = window;
	BIND_CEGUI_EVENT(window, CEGUI::Window::EventKeyDown, Widget::TabbableWindow_KeyDown)
}

void Widget::addEnterButton(CEGUI::Window* window) {
	mEnterButtons.push_back(window);
}

void Widget::closeTabGroup() {
	if (mLastTabWindow && mFirstTabWindow) {
		mTabOrder.insert(WindowMap::value_type(mLastTabWindow, mFirstTabWindow));
	}
	mFirstTabWindow = nullptr;
	mLastTabWindow = nullptr;
}

// 	void addTabbableWindow(CEGUI::Window* window, const std::string& tabGroup)
// 	{
// 		WindowStore* store;
// 		WindowStoreMap::iterator I = mTabOrders.find(tabGroup);
// 		if (I == mTabOrders.end()) {
// 			/could not find group, lets create it
// 			store = new WindowStore();
// 			mTabOrders.insert(WindowStoreMap::value_type(tabGroup, store));
// 		} else {
// 			store = I->second;
// 		}
// 		store->push_back(window);
// 		BIND_CEGUI_EVENT(window, CEGUI::Window::EventKeyUp, Widget::TabbableWindow_KeyUp);
// 	}


}
}
}
