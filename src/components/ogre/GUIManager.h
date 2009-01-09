/*
    Copyright (C) 2004  Miguel Guzman (Aglanor)
    Copyright (C) 2006  Erik Hjortsberg <erik.hjortsberg@iteam.se>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef GUIMANAGER_H
#define GUIMANAGER_H


#include "EmberOgrePrerequisites.h"

#include <CEGUIBase.h>
#include <OgreCEGUIRenderer.h>

#include <sigc++/trackable.h>

#include "framework/Singleton.h"

#include <SDL.h>
#include <stack>

#include "framework/ConsoleObject.h"

#include "services/input/InputCommandMapper.h"

namespace CEGUI
{
class GUISheet;
class LuaScriptModule;
class Window;
}

namespace Ember {
class IScriptingProvider;
class Input;
}

namespace EmberOgre {

class EmberEntity;
class TerrainGenerator;
class MousePicker;
class AvatarEmberEntity;
class GUICEGUIAdapter;
class EntityWorldPickListener;
class AvatarController;

namespace Gui {
class Widget;
class EntityIconManager;
class ActiveWidgetHandler;
class CEGUILogger;
namespace Icons {
class IconManager;
}
}

/**
 * @brief Responsible for all gui handling.
 
 This acts as the base and hub for all gui handling. It's responsible for both handling all widgets, setting up the CEGUI system and routing input event to it.
 
 It's a singleton so it can be accessed from all parts of the system.
 The root window of the CEGUI system can be accessed by the @see getMainSheet method.
 
 */
class GUIManager : 
public Ember::Singleton<GUIManager>, 
Ogre::FrameListener,
public sigc::trackable,
public Ember::ConsoleObject
{
public:

	typedef std::vector<Gui::Widget*> WidgetStore;

	static const std::string SCREENSHOT;
	static const std::string TOGGLEINPUTMODE;


	GUIManager(Ogre::RenderWindow* window, Ogre::SceneManager* sceneMgr);
	virtual ~GUIManager();
	
	sigc::signal<void, const std::string&, EmberEntity*> AppendIGChatLine;
	sigc::signal<void, const std::string&, EmberEntity*> AppendOOGChatLine;
	sigc::signal<void, const std::string&> AppendAvatarImaginary;
	
	sigc::signal<void, const std::string&, EmberEntity*> EventEntityAction;
	
	/**
	@brief Emitted every frame.
	*/
	sigc::signal<void, float> EventFrameStarted;
	
	/**
	 *    @brief Emits an action for a certain entity.
	 *    An action could be something like "touch" or "inspect". This is a rather undefined mechanism for easily letting different parts of the gui interact and react on common entity actions.
	 * @param action The name of the action.
	 * @param entity The entity for which we want to perform a certain action.
	 */
	void EmitEntityAction(const std::string& action, EmberEntity* entity);
	
	/**
	 *    Removed a widget from the system.
	 * @param widget 
	 */
	void removeWidget(Gui::Widget* widget);
	
	/**
	 *    Adds a new widget to the system. This means it will recieve FrameStarted events.
	 * @param widget 
	 */
	void addWidget(Gui::Widget* widget);

	/**
	 *    Called by Ogre each frame.
	 * @param evt 
	 * @return 
	 */
	bool frameStarted(const Ogre::FrameEvent& evt);

	/**
	 *    @brief Gets the root sheet of the CEGUI windowing system.
	 * @return 
	 */
	CEGUI::Window* getMainSheet() const;
	
	
	/**
	 * @brief Initializes the gui system.
	 * Be sure to call this before you use the gui system.
	 */
	void initialize();
	
	
	/**
	 * @brief Sets a text to be shown somewhere on the screen, used for debugging purposes.
	 * @param text The text to show.
	 */
	void setDebugText(const std::string& text);

	/**
	 *    true if we're in GUI mode, which means that input events will be sent to the CEGUI system instead of the "world"
	 * @return 
	 */
	const bool isInGUIMode() const;
	
	
	/**
	 *    true if keyboard input should make the avatar move
	 *    this happens when wither 1) we're not in gui mode 2) the background sheet has the input control (thus, when something else, like an edit box has input control, that edit box will recieve key strokes
	 * @return 
	 */
	const bool isInMovementKeysMode() const;
	
	/**
	 *    Gets the currently active MousePicker instance.
	 * @return 
	 */
	MousePicker* getMousePicker();
	
	
	/**
	 *    accessor for the Input instance object
	 * @return 
	 */
	Ember::Input& getInput() const;
	
	/**
	 *    Pushes a new mouse picker onto the stack, "pushing down" the current mouse picker.
	 * @param mousePicker 
	 */
// 	void pushMousePicker(MousePicker* mousePicker);
	
	/**
	 *    Pops the current mouse picker from the stack and returns the next in line.
	 *    It's not possible to empty the stack. If there's only one picker left, no popping will be done, and the last picker will be returned.
	 * @return 
	 */
// 	MousePicker* popMousePicker();
	
	CEGUI::OgreCEGUIRenderer* getGuiRenderer() const;

	/**
	 *    Reimplements the ConsoleObject::runCommand method
	 * @param command 
	 * @param args 
	 */
	virtual	void runCommand(const std::string &command, const std::string &args);


	/**
	 *    returns the path to the directory where all layouts are stored
	 * @return 
	 */
	const std::string& getLayoutDir() const;
	
	/**
	Creates a new window of the supplied type, giving it an autogenerated, unique name.
	*/
	CEGUI::Window* createWindow(const std::string& windowType);
	
	/**
	Creates a new window of the supplied type with the supplied name.
	*/
	CEGUI::Window* createWindow(const std::string& windowType, const std::string& windowName);
	
	/**
	 *    Creates a new Widget
	 * @return 
	 */
	Gui::Widget* createWidget();
	
	/**
	 *    creates a widget 
	 *    @see WidgetLoader
	 * @param name the type of widget to create
	 * @return 
	 */
	Gui::Widget* createWidget(const std::string& name);
	
	/**
	* Destroys a widget previously created by createWidget
	* @param widget The widget to destroy.
	*/
	void destroyWidget(Gui::Widget* widget);

	/**
	 *    Gets the name of the default scheme used (such as "EmberLook" or "WindowsLook")
	 * @return 
	 */
	const std::string& getDefaultScheme() const;
	
	EntityWorldPickListener* getEntityPickListener() const;
	
	/**
	Command for toggling between the input modes.
	*/
	const Ember::ConsoleCommandWrapper ToggleInputMode;
	
	/**
	Command for reloading the gui.
	*/
	const Ember::ConsoleCommandWrapper ReloadGui;
	
	/**
	 * Command for Hiding/Showing the gui
	 */
	const Ember::ConsoleCommandWrapper ToggleGui;
	
	
	/**
	 *    Accessor for the IconManager which handles low level icons.
	 * @return The main IconManager
	 */
	Gui::Icons::IconManager* getIconManager();
	
	/**
	 *    Accessor for the EntityIconManager, which handles entity icons and slots.
	 * @return The main EntityIconManager
	 */
	Gui::EntityIconManager* getEntityIconManager();
	
protected:

	/**
	Counter for autonaming of windows.
	*/
	static unsigned long msAutoGenId;
	
	Ember::InputCommandMapper mGuiCommandMapper;

	MousePicker* mPicker;

	EntityWorldPickListener* mEntityWorldPickListener;

	CEGUI::Window* mSheet;
	CEGUI::WindowManager* mWindowManager;
	CEGUI::GUISheet* mDebugText;
	
	Ogre::RenderWindow* mWindow;
	CEGUI::System* mGuiSystem;
	CEGUI::OgreCEGUIRenderer* mGuiRenderer;
	
	std::string mDefaultScheme;

	/**
	all loaded widgets are stored here
	*/
	WidgetStore mWidgets;

	/**
	A stack of the mouse pickers used. This allows for a component to "push down" the current mouse picker in favor of its own
	*/
	std::stack<MousePicker*> mMousePickers;

	//events
	bool mSheet_MouseButtonDown(const CEGUI::EventArgs& args);
	bool mSheet_MouseDoubleClick(const CEGUI::EventArgs& args);
	bool mSheet_CaptureLost(const CEGUI::EventArgs& args);
	
	/**
	 *    hooked to EmberOgre::EventCreatedAvatarEntity, switches the input mode to movement mode
	 * @param entity 
	 */
	void EmberOgre_CreatedAvatarEntity(AvatarEmberEntity* entity);
	
	/**
	 *    hooked to EmberOgre::EventAvatarControllerCreated, connects the mEntityWorldPickListener to the main AvatarCamera
	 * @param controller 
	 */
	void EmberOgre_AvatarControllerCreated(AvatarController& controller);

// 	InputMode mPreviousInputMode;
	void pressedKey(const SDL_keysym& key, Ember::Input::InputMode inputMode);
	
	/**
	Adapter for CEGUI which will send input events to CEGUI
	*/
	GUICEGUIAdapter* mCEGUIAdapter;
	
	CEGUI::LuaScriptModule* mLuaScriptModule;

	/**
	 * @brief Stops the CEGUI scripting service.
	 * Be sure to call this when the gui system is shutting down in order to clean up the lua environment.
	 */
	void scriptingServiceStopping();
	
	Gui::Icons::IconManager* mIconManager;
	Gui::EntityIconManager* mEntityIconManager;
	Gui::ActiveWidgetHandler* mActiveWidgetHandler;
	
	/**
	We'll provide our own CEGUI logger instance, which will route all cegui log messages to the main ember log.
	@note We will not delete this instance at desctruction, because the main CEGUI System instance will do that itself in its desctructor, even though it didn't create the instance itself.
	*/
	Gui::CEGUILogger* mCEGUILogger;
};

inline MousePicker* GUIManager::getMousePicker() { return  mMousePickers.top(); }
inline CEGUI::OgreCEGUIRenderer* GUIManager::getGuiRenderer() const {return mGuiRenderer;}

}


#endif 
