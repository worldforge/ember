/*
    Copyright (C) 2004  Miguel Guzman (Aglanor)

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

// #if SIGC_MAJOR_VERSION == 1 && SIGC_MINOR_VERSION == 0
// #include <sigc++/signal_system.h>
// #else
// #include <sigc++/object.h>
// #include <sigc++/signal.h>
// #include <sigc++/slot.h>
// #include <sigc++/bind.h>
// #include <sigc++/object_slot.h>
// #endif

#include <sigc++/trackable.h>

#include <OgreKeyEvent.h> 
#include "framework/Singleton.h"

#include <SDL.h>
#include <stack>

#include "framework/ConsoleObject.h"

#include "input/Input.h"

namespace EmberOgre {

class EmberEntity;
class TerrainGenerator;
class CEGUI::Window;
class Widget;
/*class ServerBrowserWidget;*/
class ConsoleWidget;
class MousePicker;
class EmberEventProcessor;
class Input;
class AvatarEmberEntity;
class GUIScriptManager;


/**
 * This class will be responsible for all the GUI related things
 */
class GUIManager : 
public Ember::Singleton<GUIManager>, 
Ogre::FrameListener,
public sigc::trackable,
public Ember::ConsoleObject
{
public:

	/**
	The mode of input.
	*/
// 	enum InputMode
// 	{
// 		IM_MOVEMENT = 1,
// 		IM_GUI = 2
// 	};

	static const std::string SCREENSHOT;
	static const std::string TOGGLEINPUTMODE;


	GUIManager(Ogre::RenderWindow* window, Ogre::SceneManager* sceneMgr);
	virtual ~GUIManager();
	
	sigc::signal<void, const std::string&, EmberEntity*> AppendIGChatLine;
	sigc::signal<void, const std::string&, EmberEntity*> AppendOOGChatLine;
	
	sigc::signal<void, const std::string&, EmberEntity*> EventEntityAction;
	
	/**
	Emitted when the input mode changes between gui and movment mode.
	*/
// 	SigC::Signal1<void, InputMode> EventInputModeChanged;

	
	void removeWidget(Widget* widget);
	void addWidget(Widget* widget);

	bool frameStarted(const Ogre::FrameEvent& evt);

	CEGUI::Window* getMainSheet();
	
	
	void initialize();
	
	
	/**
	 *    sets a text to be shown somewhere on the screen, used for debugging purposes
	 * @param text 
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
	
	inline MousePicker* getMousePicker() { return  mMousePickers.top(); }
	
	
	/**
	 *    accessor for the Input instance object
	 * @return 
	 */
	Input* getInput() const;
	
	/**
	 *    Pushes a new mouse picker onto the stack, "pushing down" the current mouse picker.
	 * @param mousePicker 
	 */
	void pushMousePicker(MousePicker* mousePicker);
	
	/**
	 *    Pops the current mouse picker from the stack and returns the next in line.
	 *    It's not possible to empty the stack. If there's only one picker left, no popping will be done, and the last picker will be returned.
	 * @return 
	 */
	MousePicker* popMousePicker();
	
	inline CEGUI::OgreCEGUIRenderer* getGuiRenderer() const {return mGuiRenderer;}

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

protected:

	CEGUI::Window* mSheet;
	CEGUI::WindowManager* mWindowManager;
	CEGUI::StaticText* mDebugText;
	
	ConsoleWidget* mConsoleWidget;
	//EmberEventProcessor* mEventProcessor;
	Input *mInput;
	Ogre::RenderWindow* mWindow;
	CEGUI::System* mGuiSystem;
	CEGUI::OgreCEGUIRenderer* mGuiRenderer;

	
	/**
	all loaded widgets are stored here
	*/
	std::set<Widget*> mWidgets;
	
	
	/**
	 *    creates a widget 
	 *    @see WidgetLoader
	 * @param name the type of widget to create
	 * @return 
	 */
	Widget* createWidget(const std::string& name);

	

	/**
	A stack of the mouse pickers used. This allows for a component to "push down" the current mouse picker in favor of its own
	*/
	std::stack<MousePicker*> mMousePickers;
		

	//events
	bool mSheet_MouseButtonDown(const CEGUI::EventArgs& args);
	bool mSheet_CaptureLost(const CEGUI::EventArgs& args);
	
	
	/**
	 * takes a screen shot and writes it to disk 
	 */
	void takeScreenshot();
	const std::string _takeScreenshot();

	
	/**
	 *    hooked to EmberOgre::EventCreatedAvatarEntity, switches the input mode to movement mode
	 * @param entity 
	 */
	void EmberOgre_CreatedAvatarEntity(AvatarEmberEntity* entity);

// 	InputMode mPreviousInputMode;
	void pressedKey(const SDL_keysym& key, Input::InputMode inputMode);
	
// 	GUIScriptManager* mScriptManager;

};
}


#endif 
