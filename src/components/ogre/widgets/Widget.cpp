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

#include "framework/ConsoleBackend.h"

//#include "ServerBrowserWidget.h"

namespace EmberOgre
{
// 	class ServerBrowserWidget;
// 	WidgetLoader loader("ServerBrowserWidget", &WidgetLoader::createWidgetInstance<ServerBrowserWidget>);

	ColoredListItem::ColoredListItem(const CEGUI::String& text) : ListboxTextItem(text)
	{
		setSelectionBrushImage((CEGUI::utf8*)"TaharezLook", (CEGUI::utf8*)"MultiListSelectionBrush");
	}
	
	ColoredListItem::ColoredListItem(const CEGUI::String& text, uint item_id, void *item_data) : ListboxTextItem(text, item_id, item_data)
	{
		setSelectionBrushImage((CEGUI::utf8*)"TaharezLook", (CEGUI::utf8*)"MultiListSelectionBrush");
	}

	
	Widget::Widget()
	{
	}
	
	void Widget::init(GUIManager* guiManager)
	{
		mGuiManager = guiManager;
		mWindowManager = &CEGUI::WindowManager::getSingleton();
	}
	
	
	Widget::~Widget()
	{
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
		mPrefix = prefix;
		mMainWindow = CEGUI::WindowManager::getSingleton().loadWindowLayout(std::string("cegui/widgets/") + filename, prefix);
		if (mMainWindow) {
			getMainSheet()->addChildWindow(mMainWindow); 
		}
		return mMainWindow;
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
	}
	
	void Widget::enableCloseButton()
	{
		assert(mMainWindow);
		BIND_CEGUI_EVENT(mMainWindow, CEGUI::FrameWindow::EventCloseClicked, Widget::MainWindow_CloseClick)
	}
	

}
