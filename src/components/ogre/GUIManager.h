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

/*#include <CEGUIBase.h>
#include <CEGUIExceptions.h>
#include <CEGUISystem.h>
#include <CEGUISchemeManager.h>
#include <CEGUIWindow.h>
#include <CEGUIWindowManager.h>
#include <CEGUIImageset.h>
#include <elements/CEGUIStaticImage.h>*/
#include "EmberOgrePrerequisites.h"
#include <elements/CEGUIPushButton.h>
#include <OgreCEGUIRenderer.h>
// #include <boost/bind.hpp>

#if SIGC_MAJOR_VERSION == 1 && SIGC_MINOR_VERSION == 0
#include <sigc++/signal_system.h>
#else
#include <sigc++/object.h>
#include <sigc++/signal.h>
#include <sigc++/slot.h>
#include <sigc++/bind.h>
#include <sigc++/object_slot.h>
#endif

#include "EmberOgrePrerequisites.h"
#include <OgreKeyEvent.h> 
//#include <OgrePredefinedControllers.h> 
#include "framework/Singleton.h"

#include <SDL.h>
#include <stack>

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


/*
 * This class will be responsible for all the GUI related things
 */
class GUIManager : 
public Ember::Singleton<GUIManager>, 
/*Ogre::MouseMotionListener, 
Ogre::MouseListener,
Ogre::KeyListener,*/
Ogre::FrameListener,
virtual public SigC::Object
{
public:

	GUIManager(Ogre::RenderWindow* window, Ogre::SceneManager* sceneMgr);
	virtual ~GUIManager();
	//static MotionManager & getSingleton(void);
	
	SigC::Signal2<void, const std::string&, EmberEntity*> AppendIGChatLine;
	SigC::Signal2<void, const std::string&, EmberEntity* > AppendOOGChatLine;
	
	SigC::Signal2<void, const std::string&, EmberEntity*> EventEntityAction;
	
	void testClick(const CEGUI::EventArgs& args);
	
	void removeWidget(Widget* widget);
	void addWidget(Widget* widget);

	bool frameStarted(const Ogre::FrameEvent& evt);

	CEGUI::Window* getMainSheet();
	
	void pressedKey(const SDL_keysym& key, bool isInGuimode);
	//EmberEventProcessor* getEventProcessor() { return mEventProcessor; }
	
	void initialize();
	
/*	void setKeyListener(Ogre::KeyListener* listener) {mKeyListener; }
	void setMouseListener(Ogre::MouseListener* listener) {mMouseListener = listener; }*/
	
	void setDebugText(std::string text);

	const bool isInGUIMode() const;
	const bool isInMovementKeysMode() const;
	
	inline MousePicker* getMousePicker() { return  mMousePickers.top(); }
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
protected:


	CEGUI::Window* mChat;
	
	CEGUI::Window* mSheet;
	CEGUI::WindowManager* mWindowManager;
	CEGUI::StaticText* mDebugText;
	
/*ServerBrowserWidget* t;*/
	ConsoleWidget* mConsoleWidget;
	//EmberEventProcessor* mEventProcessor;
	Input *mInput;
	Ogre::RenderWindow* mWindow;
	CEGUI::System* mGuiSystem;
	CEGUI::OgreCEGUIRenderer* mGuiRenderer;

	std::set<Widget*> mWidgets;
	
	Widget* createWidget(const std::string& name);

	void updateStats(void);


	//Ogre::KeyListener* mKeyListener;
	

	/**
	A stack of the mouse pickers used. This allows for a component to "push down" the current mouse picker in favor of its own
	*/
	std::stack<MousePicker*> mMousePickers;
		
	//MousePicker* mMousePicker;

	//events
	bool mSheet_MouseButtonDown(const CEGUI::EventArgs& args);
	bool mSheet_CaptureLost(const CEGUI::EventArgs& args);
	
	//takes a screen shot and writes it to disk
	const std::string takeScreenshot();

	
	

};
}


#endif 
