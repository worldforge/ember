//
// C++ Implementation: Vector3Adapter
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

#include "Widget.h"

#include "../GUIManager.h"
#include "services/input/Input.h"
#include "../EmberOgre.h"

#include <CEGUIWindow.h>
#include <CEGUIExceptions.h>
#include <CEGUIWindowManager.h>
#include <elements/CEGUIFrameWindow.h>
#include <elements/CEGUIPushButton.h>

#include "framework/ConsoleBackend.h"

using namespace CEGUI;
namespace EmberOgre {
namespace Gui {

	const std::string Widget::DEFAULT_TAB_GROUP("default");

	Widget::Widget() : mCommandSuffix(""), mMainWindow(0), mActiveWindowIsOpaque(true), mFirstTabWindow(0), mLastTabWindow(0), mWindowHasBeenShown(false)
	{
	}

	void Widget::init(GUIManager* guiManager)
	{
		mGuiManager = guiManager;
		mWindowManager = &CEGUI::WindowManager::getSingleton();
	}


	Widget::~Widget()
	{
		if (mCommandSuffix != "") {
			Ember::ConsoleBackend::getSingletonPtr()->deregisterCommand("show_" + mCommandSuffix);
		}
		if (mMainWindow) {
			CEGUI::WindowManager::getSingleton().destroyWindow(mMainWindow);
		}
		//mGuiManager->removeWidget(this);
	}

	void Widget::frameStarted(const Ogre::FrameEvent& evt)
	{
		EventFrameStarted.emit(evt.timeSinceLastFrame);
	}

	void Widget::buildWidget()
	{}



	CEGUI::Window* Widget::getMainSheet() {
		return mGuiManager->getMainSheet();
	}


	CEGUI::Window* Widget::loadMainSheet(const std::string& filename, const std::string& prefix) {
		assert(mWindowManager && "You must call init() before you can call any other methods.");
		mPrefix = prefix;
		std::string finalFileName(mGuiManager->getLayoutDir() + filename);
		try {
			mMainWindow = mWindowManager->loadWindowLayout(finalFileName, prefix);
		} catch (const CEGUI::Exception& ex) {
			S_LOG_FAILURE("Error when loading from " << filename << ".\nMessage: " <<  ex.getMessage().c_str());
		} catch (const std::exception& ex) {
			S_LOG_FAILURE("Error when loading from " << filename << "." << ex);
		} catch (...) {
			S_LOG_FAILURE("Unknown error when loading from " << filename << ".");
		}
		if (mMainWindow) {
			mOriginalWindowAlpha = mMainWindow->getAlpha();
			getMainSheet()->addChildWindow(mMainWindow);
			BIND_CEGUI_EVENT(mMainWindow, CEGUI::FrameWindow::EventActivated, Widget::MainWindow_Activated);
			BIND_CEGUI_EVENT(mMainWindow, CEGUI::FrameWindow::EventDeactivated, Widget::MainWindow_Deactivated);
			///we want to catch all click events, so we'll listen for the mouse button down event
			BIND_CEGUI_EVENT(mMainWindow, CEGUI::Window::EventMouseButtonDown, Widget::MainWindow_MouseButtonDown);
			if (mMainWindow->isVisible()) {
				onEventFirstTimeShown();
			} else {
				///Set it up to listen for the first time the window is shown.
				BIND_CEGUI_EVENT(mMainWindow, CEGUI::Window::EventShown, Widget::MainWindow_Shown);
			}

		}
		return mMainWindow;
	}

	void  Widget::onEventFirstTimeShown()
	{
		mWindowHasBeenShown = true;
		EventFirstTimeShown.emit();
	}

	CEGUI::Window* Widget::getWindow(const std::string& windowName)
	{
		try {
			assert(mWindowManager && "You must call init() before you can call any other methods.");
			if (!mMainWindow) {
				S_LOG_WARNING("Trying to get a window ("+ windowName +") on widget that has no main sheet loaded (" << mPrefix << ").");
				return 0;
			}
			assert(mMainWindow && "You must call loadMainSheet(...) before you can call this method.");
			CEGUI::Window* window = mWindowManager->getWindow(mPrefix + windowName);
			if (!window) {
				S_LOG_WARNING("The window with id " << mPrefix << windowName << " does not exist.");
			}
			return window;
		} catch (const CEGUI::Exception&) {
			S_LOG_WARNING("The window " << windowName << " doesn't exist.");
			return 0;
		}

	}

	CEGUI::Window* Widget::createWindow(const std::string& windowType)
	{
		return mGuiManager->createWindow(windowType);
	}

	CEGUI::Window* Widget::createWindow(const std::string& windowType, const std::string& windowName)
	{
		return mGuiManager->createWindow(windowType, windowName);
	}



	Widget* WidgetLoader::createWidget(const std::string& name) {

		if (getFactories().find(name) == getFactories().end()) {
			return 0;
		}

		Widget* widget = getFactories()[name]();
		return widget;
	}

	WidgetLoader::WidgetLoader(const std::string& name, FactoryFunc functor)
	{
		getFactories().insert(WidgetFactoryMap::value_type(name, functor));

	}

	WidgetFactoryMap& WidgetLoader::getFactories()
	{
		static WidgetFactoryMap* factoryMap = new WidgetFactoryMap();
		return *factoryMap;
	}

	void WidgetLoader::registerWidgetFactory(const std::string& name, FactoryFunc functor)
	{
		getFactories().insert(WidgetFactoryMap::value_type(name, functor));
	}

	void WidgetLoader::removeAllWidgetFactories()
	{
		WidgetFactoryMap& factoryMap(getFactories());
/*		for (WidgetFactoryMap::iterator I = factoryMap.begin(); I != factoryMap.end(); ++I) {
			delete I->second;
		}*/
		factoryMap.clear();
	}

	void Widget::registerConsoleVisibilityToggleCommand(const std::string & commandSuffix)
	{
		mCommandSuffix = commandSuffix;
		Ember::ConsoleBackend::getSingletonPtr()->registerCommand("show_" + commandSuffix, this, std::string("Shows the ") + mCommandSuffix + " window.");
		Ember::ConsoleBackend::getSingletonPtr()->registerCommand("hide_" + commandSuffix, this, std::string("Hides the ") + mCommandSuffix + " window.");

	}

	void Widget::runCommand(const std::string &command, const std::string &args)
	{
		if(command == "show_" + mCommandSuffix)
		{
			show();
		}
		else if(command == "hide_" + mCommandSuffix)
		{
			hide();
		}
	}

	void  Widget::show()
	{
		///removing and attaching the window is probably more efficient when it's hidden (i.e. it won't get any events at all and so on)
		if (mMainWindow) {
			getMainSheet()->addChildWindow(mMainWindow);
			mMainWindow->setVisible(true);
		}
	}

	void  Widget::hide()
	{
		///see comment in show()
		if (mMainWindow) {
			getMainSheet()->removeChildWindow(mMainWindow);
			mMainWindow->setVisible(false);
		}
	}

	bool Widget::MainWindow_CloseClick(const CEGUI::EventArgs& args)
	{
		hide();
		return true;
	}

	void Widget::enableCloseButton()
	{
		assert(mMainWindow);
		BIND_CEGUI_EVENT(mMainWindow, CEGUI::FrameWindow::EventCloseClicked, Widget::MainWindow_CloseClick);
	}

	CEGUI::Window* Widget::getMainWindow()
	{
		return mMainWindow;
	}

	bool Widget::MainWindow_MouseButtonDown(const CEGUI::EventArgs& args)
	{
		///we'll return true here to prevent the event from propagating to other windows
		return true;
	}

	bool Widget::MainWindow_Activated(const CEGUI::EventArgs& args)
	{
		if (mMainWindow && mActiveWindowIsOpaque) {
			mMainWindow->setAlpha(1.0);

		}
		return true;
	}

	bool Widget::MainWindow_Deactivated(const CEGUI::EventArgs& args)
	{
		if (mMainWindow && mActiveWindowIsOpaque) {
			mMainWindow->setAlpha(mOriginalWindowAlpha);
		}
		return true;
	}

	bool Widget::MainWindow_Shown(const CEGUI::EventArgs& args)
	{
		if (!mWindowHasBeenShown) {
			onEventFirstTimeShown();
		}
		return true;
	}


	bool Widget::getIsActiveWindowOpaque() const
	{
		return mActiveWindowIsOpaque;
	}

	void Widget::setIsActiveWindowOpaque(bool isOpaque)
	{
		mActiveWindowIsOpaque = isOpaque;
	}

	const std::string& Widget::getDefaultScheme() const
	{
		return mGuiManager->getDefaultScheme();
	}



	bool Widget::TabbableWindow_KeyDown(const CEGUI::EventArgs& args)
	{
		const CEGUI::KeyEventArgs& keyEventArgs = static_cast<const CEGUI::KeyEventArgs&>(args);
		if (keyEventArgs.scancode == CEGUI::Key::Tab)
		{
			///find the window in the list of tabbable windows
			CEGUI::Window* activeWindow = mMainWindow->getActiveChild();
			if (activeWindow) {
//				WindowMap::iterator I = std::find(mTabOrder.begin(), mTabOrder.end(), activeWindow);
				WindowMap::iterator I = mTabOrder.find(activeWindow);
				if (I != mTabOrder.end()) {
					I->second->activate();
					///we don't want to process the event any more, in case something else will try to interpret the tab event to also change the focus
					Ember::Input::getSingleton().suppressFurtherHandlingOfCurrentEvent();
					return true;
				}
			}
		} else if (keyEventArgs.scancode == CEGUI::Key::Return)
		{
			///iterate through all enter buttons, and if anyone is visible, activate it
			for (WindowStore::iterator I = mEnterButtons.begin(); I != mEnterButtons.end(); ++I) {
				if ((*I)->isVisible()) {
					CEGUI::Window* window = *I;
					WindowEventArgs args(window);
					window->fireEvent(PushButton::EventClicked, args, PushButton::EventNamespace);
					break;
				}
			}
		}
		return false;
	}

	void Widget::addTabbableWindow(CEGUI::Window* window)
	{
		if (!mFirstTabWindow) {
			mFirstTabWindow = window;
		}
		if (mLastTabWindow) {
			mTabOrder.insert(WindowMap::value_type(mLastTabWindow, window));
		}
		mLastTabWindow = window;
		BIND_CEGUI_EVENT(window, CEGUI::Window::EventKeyDown, Widget::TabbableWindow_KeyDown);
	}

	void Widget::addEnterButton(CEGUI::Window* window)
	{
		mEnterButtons.push_back(window);
	}

	void Widget::closeTabGroup()
	{
		if (mLastTabWindow && mFirstTabWindow) {
			mTabOrder.insert(WindowMap::value_type(mLastTabWindow, mFirstTabWindow));
		}
		mFirstTabWindow = 0;
		mLastTabWindow = 0;
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
