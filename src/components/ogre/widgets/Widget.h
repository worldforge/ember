#ifndef WIDGET_H
#define WIDGET_H


#include "components/ogre/EmberOgrePrerequisites.h"
#include <elements/CEGUIListboxTextItem.h> 

//#include <CEGUIWindowManager.h>



#include <sigc++/trackable.h>
//#include <sigc++/signal.h>
//#include <sigc++/slot.h>
//#include <sigc++/bind.h>
//#include <sigc++/object_slot.h>



#include "framework/ConsoleObject.h"

namespace CEGUI
{
	class WindowManager;
	class Listbox;
	class ListboxItem;
	class ListboxTextItem;
	class Slider;
	class Editbox;
	class PushButton;
	class MultiLineEditbox;
	class Combobox;
	class RadioButton;

}

namespace EmberOgre {

/**
Utility define for binding CEGUI elements to methods. 
*/
#define BIND_CEGUI_EVENT(window, event, method) window->subscribeEvent(event, CEGUI::Event::Subscriber(&method, this)); 


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



/**
Because CEGUI::Listbox can't hold const objects in its
*/
template<typename T> class ConstWrapper
{
public:
	ConstWrapper(T value) : mValue(value) {}
	T mValue;
};


/** 

Base class for all widgets.
Put all widget set up code in the buildWidget() method.


NOTE: Perhaps we should provide another base class for widgets that have a single movable window?


When creating a new Widget class, make sure you also add it to WidgetDefinitions.
@see WidgetDefinitions

*/
class Widget : 
public sigc::trackable,
public Ember::ConsoleObject
{
public:

	static CEGUI::ListboxItem* createColoredListItem(const CEGUI::String& text); 
	static CEGUI::ListboxItem* createColoredListItem(const CEGUI::String& text, unsigned int item_id);
	static CEGUI::ListboxItem* createColoredListItem(const CEGUI::String& text, unsigned int item_id, void *item_data);
	
	
	Widget();
	virtual ~Widget();
	
	
	/**
	 *    Sets up the widget, called upon creation.
	 * @param guiManager 
	 */
	void init(GUIManager* guiManager);
	
	
	/**
	 *    Called each frame.
	 * @param evt 
	 */
	virtual void frameStarted(const Ogre::FrameEvent& evt);
	
	
	/**
	 *    Called by the GUIManager to tell the widget to create all needed GUI elements.
	 *    Override this in your subclass.
	 *    Remember to call getMainSheet()->addChildWindow(mMainWindow); to add the main window to the gui system, else it won't be shown.
 	 */
	virtual void buildWidget();
	
	/**
	 *    Reimplements the ConsoleObject::runCommand method
	 * @param command 
	 * @param args 
	 */
	virtual	void runCommand(const std::string &command, const std::string &args);

	
	/**
	 *    Show the widget.
	 */
	virtual void show();
	
	/**
	 *    Hides the widget.
	 */
	virtual void hide();
	
	/**
	 *    Get's the window with the supplied name from the WindowManager. Note that you shouldn't supply the prefix (as defined in loadMainSheet) since that will be added by the method.
	 * @param windowName The name of the window, without the suffix.
	 * @return A valid Window pointer of 0.
	 */
	CEGUI::Window* getWindow(const std::string& windowName);
	
	
	/**
	 *    Call this method upon creation of the widget (for example in buildWidget) to enable the close button and wire it to the correct methods.
	 */
	void enableCloseButton();
	
	/**
	 *    Call this method upon creation of the widget (for example in buildWidget) to register show and hide commands with the console.
	 *    The command you choose will be prefixed by "show_" and "hide_". So if you have a widget which shows a map and you call this method with the
	 *    parameter "map" the commands "show_map" and "hide_map" will be registered with the console.
	 * @param commandSuffix a string to be prefixed by "show_" and "hide_"
	 */
	void registerConsoleVisibilityToggleCommand(const std::string & commandSuffix);
	
	/**
	accessor to the main sheet of the gui system
	*/
	CEGUI::Window* getMainSheet();
	
	/**
	 *    Accessor to the main window of the widget.
	 * @return 
	 */
	CEGUI::Window* getMainWindow();

	/**
	 *    Loads a widget definition from a file and sets the main sheet
	 * @param filename The name of the file to load
	 * @param prefix The prefix to use
	 * @return 
	 */
	CEGUI::Window* loadMainSheet(const std::string& filename, const std::string& prefix);
	
	/**
	 *    Gets the prefix used in the widget definition
	 * @return 
	 */
	inline const std::string& getPrefix() const { return mPrefix;}
	
	/**
	 *    Gets whether the window when being activated should become fully opaque, to return to it's preset alpha value when being deactivated.
	 Defaults to true.
	 * @return 
	 */
	bool getIsActiveWindowOpaque() const;
	/**
	 *    Sets whether the window when being activated should become fully opaque, to return to it's preset alpha value when being deactivated.
	 Defaults to true.
	 * @param isOpaque 
	 */
	void setIsActiveWindowOpaque(bool isOpaque);
	
	/**
	 *    Gets the name of the default scheme used (such as "EmberLook" or "EmberLook")
	 * @return 
	 */
	const std::string& getDefaultScheme() const;
	

protected:

	

	bool MainWindow_CloseClick(const CEGUI::EventArgs& args);

	bool MainWindow_Activated(const CEGUI::EventArgs& args);
	bool MainWindow_Deactivated(const CEGUI::EventArgs& args);

	/**
	*    The suffixed used by registerConsoleVisibilityToggleCommand
	* @see registerConsoleVisibilityToggleCommand
	*/
	std::string mCommandSuffix;
	
	
	
	/**
	*     The main window for the widget.
	*/
	CEGUI::Window* mMainWindow;

	GUIManager* mGuiManager;
	

	CEGUI::WindowManager* mWindowManager;
	
	/**
	The original alpha value of the window.
	*/
	float mOriginalWindowAlpha;
	
	/**
	If true, when activated the window will become fully opaque.
	*/
	bool mActiveWindowIsOpaque;
	
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
	
	/**
	 *    Registers a widget (which functor points at) with a string. The widget can later be loaded through the createWidget method and the same string.
	 * @param name The name of the widget, which can later be used in createWidget
	 * @param functor A functor to a Widget, for example "&WidgetLoader::createWidgetInstance<Help>"
	 */
	static void registerWidget(const std::string& name, FactoryFunc functor );

	
	/**
	 *    Use this together with registerWidget, which requires a functor to a widget as a parameter.
	 * @return A functor to a Widget.
	 */
	template <typename T> static Widget* createWidgetInstance() { return new T; }

};


/**
	A standard ListboxTextItem, with the exeption that the selection will be colored.
*/
class ColoredListItem : public CEGUI::ListboxTextItem
{
public:
	ColoredListItem(const CEGUI::String& text); 
	ColoredListItem(const CEGUI::String& text, unsigned int item_id);
	ColoredListItem(const CEGUI::String& text, unsigned int item_id, void *item_data);
private:
	void setColours();
};


}



#endif // WIDGET_H
