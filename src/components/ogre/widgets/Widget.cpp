#include "Widget.h"

#include "../GUIManager.h"
#include "../input/Input.h"
#include "../EmberOgre.h"

#include <CEGUIWindow.h>
#include <CEGUIExceptions.h>
#include <CEGUIWindowManager.h>
#include <elements/CEGUIFrameWindow.h>
#include <elements/CEGUIPushButton.h>

#include "framework/ConsoleBackend.h"

using namespace CEGUI;
namespace EmberOgre
{
	const std::string Widget::DEFAULT_TAB_GROUP("default");
	
	Widget::Widget() : mCommandSuffix(""), mMainWindow(0), mActiveWindowIsOpaque(true), mFirstTabWindow(0), mLastTabWindow(0)
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
			Ember::ConsoleBackend::getMainConsole()->deregisterCommand("show_" + mCommandSuffix);
		}
		if (mMainWindow) {
			CEGUI::WindowManager::getSingleton().destroyWindow(mMainWindow);
		}
		mGuiManager->removeWidget(this);
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
		mMainWindow = mWindowManager->loadWindowLayout(finalFileName, prefix);
		mOriginalWindowAlpha = mMainWindow->getAlpha();
		if (mMainWindow) {
			getMainSheet()->addChildWindow(mMainWindow); 
			BIND_CEGUI_EVENT(mMainWindow, CEGUI::FrameWindow::EventActivated, Widget::MainWindow_Activated);
			BIND_CEGUI_EVENT(mMainWindow, CEGUI::FrameWindow::EventDeactivated, Widget::MainWindow_Deactivated);
			///we want to catch all click events, so we'll listen for the mouse button down event
			BIND_CEGUI_EVENT(mMainWindow, CEGUI::Window::EventMouseButtonDown, Widget::MainWindow_MouseButtonDown);
			
		}
		return mMainWindow;
	}

	CEGUI::Window* Widget::getWindow(const std::string& windowName)
	{
		assert(mWindowManager && "You must call init() before you can call any other methods.");
		assert(mMainWindow && "You must call loadMainSheet(...) before you can call this method.");
		CEGUI::Window* window = mWindowManager->getWindow(mPrefix + windowName);
		if (!window) {
			S_LOG_WARNING("The window with id " << mPrefix << windowName << " does not exist.");
		}
		return 	window;

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

		if (GetFactories().find(name) == GetFactories().end()) {
			return 0;
		}
		
		Widget* widget = GetFactories()[name]();
		return widget;
	}
	
	WidgetLoader::WidgetLoader(const std::string& name, FactoryFunc functor)
	{
		GetFactories().insert(WidgetFactoryMap::value_type(name, functor));
		
	}

	WidgetFactoryMap& WidgetLoader::GetFactories()
	{
		static WidgetFactoryMap* factoryMap = new WidgetFactoryMap();
		return *factoryMap;
	}
	
	void WidgetLoader::registerWidget(const std::string& name, FactoryFunc functor)
	{
		GetFactories().insert(WidgetFactoryMap::value_type(name, functor));
	
	}
	
	void Widget::registerConsoleVisibilityToggleCommand(const std::string & commandSuffix)
	{
		mCommandSuffix = commandSuffix;
		Ember::ConsoleBackend::getMainConsole()->registerCommand("show_" + commandSuffix, this, std::string("Shows the ") + mCommandSuffix + " window.");
		Ember::ConsoleBackend::getMainConsole()->registerCommand("hide_" + commandSuffix, this, std::string("Hides the ") + mCommandSuffix + " window.");
	
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
		if (mMainWindow) {
			mMainWindow->setVisible(true);
		}
	}
	
	void  Widget::hide()
	{
		if (mMainWindow) {
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
					Input::getSingleton().suppressFurtherHandlingOfCurrentEvent();
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
					window->fireEvent(ButtonBase::EventMouseClick, args, PushButton::EventNamespace);
					break;
				}
			}
		}
		return true;
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
