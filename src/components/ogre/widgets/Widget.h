#ifndef WIDGET_H
#define WIDGET_H


#include "components/ogre/EmberOgrePrerequisites.h"
// #include <CEGUI.h>
// #include <CEGUIEvent.h> 
// #include <CEGUIBase.h>
// #include <CEGUIExceptions.h>
// #include <CEGUISystem.h>
// #include <CEGUISchemeManager.h>
// #include <CEGUIWindow.h>
//#include <elements/CEGUIListboxItem.h> 
#include <elements/CEGUIListboxTextItem.h> 

#include <CEGUIWindowManager.h>
// #include <CEGUIImageset.h>
#include <boost/bind.hpp>

// #include <elements/CEGUIStaticImage.h>
// #include <elements/CEGUIPushButton.h>
// #include <elements/CEGUIGUISheet.h>
// #include <renderers/OgreGUIRenderer/ogrerenderer.h>

#if SIGC_MAJOR_VERSION == 1 && SIGC_MINOR_VERSION == 0
#include <sigc++/signal_system.h>
#else
#include <sigc++/object.h>
#include <sigc++/signal.h>
#include <sigc++/slot.h>
#include <sigc++/bind.h>
#include <sigc++/object_slot.h>
#endif


namespace EmberOgre {

#define BIND_CEGUI_EVENT(window, event, method) window->subscribeEvent(event, boost::bind(&method, this, _1)); 


class GUIManager;
class WidgetLoader;
class Widget;

/**se this template to register a widget in the global widget factory map
This is done by:
template<> WidgetLoader WidgetLoaderHolder<ASubClassOfWidget>::loader("associatedName", &createWidgetInstance);
*/
// template <typename T> class WidgetLoaderHolder
// {
// protected:
// 
// 	static WidgetLoader loader;
// 	static Widget* createWidgetInstance() { return new T; }
// };


/** Base class for all widgets
*/
class Widget : virtual public SigC::Object
{
public:

	Widget();
	virtual ~Widget();
	
	void init(GUIManager* guiManager);
	
	virtual void frameStarted(const Ogre::FrameEvent& evt);
	virtual void buildWidget();
protected:
	CEGUI::Window* mMainWindow;

	GUIManager* mGuiManager;
	
	/**
	accessor to the main sheet of the gui system
	*/
	CEGUI::Window* getMainSheet();

	CEGUI::WindowManager* mWindowManager;
	
	
	/**
	 *    Loads a widget definition from a file and sets the main sheet
	 * @param filename The name of the file to load
	 * @param prefix The prefix to use
	 * @return 
	 */
	CEGUI::Window* Widget::loadMainSheet(const std::string& filename, const std::string& prefix);
	
	/**
	 *    Gets the prefix used in the widget definition
	 * @return 
	 */
	inline const std::string& getPrefix() const { return mPrefix;}
	
private:
	std::string mPrefix;
	
};


typedef Widget* (*FactoryFunc)();
TYPEDEF_STL_MAP(std::string, FactoryFunc, WidgetFactoryMap);	

/**
Utility class for associating Widgets to strings.
See WidgetLoaderHolder on how to register Widgets.

Use createWidget("someName") to create widgets.
*/
class WidgetLoader
{
public:

	
	/**
	upon creation, an entry will be added to the internal WidgetFactoryMap
	*/
	WidgetLoader(const std::string& name, FactoryFunc functor);	
	
	/**
	map of all factories, we have to use a static function to avoid static initialization order fiasco
	*/
	static WidgetFactoryMap& GetFactories();
	
	/**creates and returns a widget associated to the submitted string
	if no widget can be found, a null pointer is returned
	*/
	static Widget* createWidget(const std::string& name);
	
	static void registerWidget(const std::string& name, FactoryFunc functor );

	
	template <typename T> static Widget* createWidgetInstance() { return new T; }

};


/**
	A standard ListboxTextItem, with the exeption that the selection will be colored.
*/
class ColoredListItem : public CEGUI::ListboxTextItem
{
public:
	ColoredListItem(const CEGUI::String& text); 
	ColoredListItem(const CEGUI::String& text, uint item_id, void *item_data);
};


}



#endif // WIDGET_H
