#include "Widget.h"

#include "../GUIManager.h"
#include <CEGUIWindow.h>

#if SIGC_MAJOR_VERSION == 1 && SIGC_MINOR_VERSION == 0
#include <sigc++/signal_system.h>
#else
#include <sigc++/object.h>
#include <sigc++/signal.h>
#include <sigc++/slot.h>
#include <sigc++/bind.h>
#include <sigc++/object_slot.h>
#endif

#include <elements/CEGUIFrameWindow.h>
#include <CEGUIWindowManager.h>

#include "framework/ConsoleBackend.h"

//#include "ServerBrowserWidget.h"

namespace EmberOgre
{
// 	class ServerBrowserWidget;
// 	WidgetLoader loader("ServerBrowserWidget", &WidgetLoader::createWidgetInstance<ServerBrowserWidget>);

	ColoredListItem::ColoredListItem(const CEGUI::String& text) : ListboxTextItem(text)
	{
		setColours();
	}
	
	ColoredListItem::ColoredListItem(const CEGUI::String& text, unsigned int item_id) : ListboxTextItem(text, item_id)
	{
		setColours();
	}
	
	ColoredListItem::ColoredListItem(const CEGUI::String& text, unsigned int item_id, void *item_data) : ListboxTextItem(text, item_id, item_data)
	{
		setColours();
	}
	
	void ColoredListItem::setColours()
	{
 		setSelectionColours(CEGUI::colour(50,50,50));
		setTextColours(CEGUI::colour(0,0,0));
		try {
			setSelectionBrushImage((CEGUI::utf8*)"EmberLook", (CEGUI::utf8*)"SelectionBrush");
		} catch (...) {}
	
	}

	CEGUI::ListboxItem* Widget::createColoredListItem(const CEGUI::String& text)
	{
		return new ColoredListItem(text);
	}
	
	CEGUI::ListboxItem* Widget::createColoredListItem(const CEGUI::String& text, unsigned int item_id)
	{
		return new ColoredListItem(text, item_id);
	}
	
	CEGUI::ListboxItem* Widget::createColoredListItem(const CEGUI::String& text, unsigned int item_id, void *item_data)
	{
		return new ColoredListItem(text, item_id, item_data);
	}
	
	
	
	
	
	
	
	
	Widget::Widget() : mMainWindow(0), mCommandSuffix(""), mActiveWindowIsOpaque(true)
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
	}
	
	void Widget::buildWidget()
	{}



	CEGUI::Window* Widget::getMainSheet() { 
		return mGuiManager->getMainSheet(); 
	}

		
	CEGUI::Window* Widget::loadMainSheet(const std::string& filename, const std::string& prefix) { 
		assert(mWindowManager && "You must call init() before you can call any other methods.");
		mPrefix = prefix;
		mMainWindow = mWindowManager->loadWindowLayout(mGuiManager->getLayoutDir() + filename, prefix);
		mOriginalWindowAlpha = mMainWindow->getAlpha();
		if (mMainWindow) {
			getMainSheet()->addChildWindow(mMainWindow); 
			BIND_CEGUI_EVENT(mMainWindow, CEGUI::FrameWindow::EventActivated, Widget::MainWindow_Activated);
			BIND_CEGUI_EVENT(mMainWindow, CEGUI::FrameWindow::EventDeactivated, Widget::MainWindow_Deactivated);
		}
		return mMainWindow;
	}

	CEGUI::Window* Widget::getWindow(const std::string& windowName)
	{
		assert(mWindowManager && "You must call init() before you can call any other methods.");
		assert(mMainWindow && "You must call loadMainSheet(...) before you can call this method.");
		return 	mWindowManager->getWindow(mPrefix + windowName);

	}

	
//	WidgetLoader::WidgetLoaderMap WidgetLoader::sWidgetLoaders;
//	WidgetFactoryMap sFactories;
	
/*	void WidgetLoader::addWidgetDef(std::string name, Widget* (*)() ptr) {
		sWidgetDefs[name] =ptr;
	}*/
	
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
		Ember::ConsoleBackend::getMainConsole()->registerCommand("show_" + commandSuffix, this);
		Ember::ConsoleBackend::getMainConsole()->registerCommand("hide_" + commandSuffix, this);
	
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

}
