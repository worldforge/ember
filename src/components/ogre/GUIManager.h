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
#include <renderers/OgreGUIRenderer/ogrerenderer.h>
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

namespace EmberOgre {

class EmberEntity;
class TerrainGenerator;
class CEGUI::Window;
class Widget;
class ConsoleWidget;
class MousePicker;

/*
 * This class will be responsible for all the GUI related things
 */
class GUIManager : 
public Ember::Singleton<GUIManager>, 
Ogre::MouseMotionListener, 
Ogre::MouseListener,
Ogre::KeyListener,
Ogre::FrameListener,
virtual public SigC::Object
{
public:

	GUIManager(Ogre::RenderWindow* window, Ogre::SceneManager* sceneMgr);
	virtual ~GUIManager();
	//static MotionManager & getSingleton(void);
	
	SigC::Signal1<void, const std::string& > AppendIGChatLine;
	SigC::Signal1<void, const std::string& > AppendOOGChatLine;
	
	SigC::Signal2<void, const std::string&, EmberEntity*> EventEntityAction;
	
	void testClick(const CEGUI::EventArgs& args);
	
	void removeWidget(Widget* widget);
	void addWidget(Widget* widget);



	virtual void 	mouseMoved (Ogre::MouseEvent *e);
	virtual void 	mouseDragged (Ogre::MouseEvent *e);
	virtual void 	keyPressed (Ogre::KeyEvent *e);
	virtual void 	keyReleased (Ogre::KeyEvent *e);
	virtual void 	mousePressed (Ogre::MouseEvent *e);
	virtual void 	mouseReleased (Ogre::MouseEvent *e);

	// do-nothing events
	virtual void 	keyClicked (Ogre::KeyEvent *e) {}
	virtual void 	mouseClicked (Ogre::MouseEvent *e) {}
	virtual void 	mouseEntered (Ogre::MouseEvent *e) {}
	virtual void 	mouseExited (Ogre::MouseEvent *e) {}


	bool frameStarted(const Ogre::FrameEvent& evt);

	CEGUI::Window* getMainSheet();
	
	Ogre::EventProcessor* getEventProcessor() { return mEventProcessor; }
	
	void initialize();
	
	void setKeyListener(Ogre::KeyListener* listener) {mKeyListener; }
	void setMouseListener(Ogre::MouseListener* listener) {mMouseListener = listener; }
	void setMouseMotionListener(Ogre::MouseMotionListener* listener) {mMouseMotionListener = listener;}
	
	void setDebugText(std::string text);

	bool isInGUIMode() { return mInGUIMode; }
	bool isInMovementKeysMode();
	
	inline MousePicker* getMousePicker() { return mMousePicker; }
protected:


	CEGUI::Window* mChat;
	
	CEGUI::Window* mSheet;
	CEGUI::WindowManager* mWindowManager;
	CEGUI::StaticText* mDebugText;
	
	ConsoleWidget* mConsoleWidget;

	Ogre::EventProcessor* mEventProcessor;
	Ogre::RenderWindow* mWindow;
	CEGUI::System* mGuiSystem;
	CEGUI::OgreRenderer* mGuiRenderer;

	std::set<Widget*> mWidgets;



	CEGUI::MouseButton convertOgreButtonToCegui(int ogre_button_id);
	void updateStats(void);


//	float	mSkipCount;
//	float	mUpdateFreq;

		
	bool mInGUIMode;
	
	//these will recieve input when we're not in GUI mode
	Ogre::MouseMotionListener* mMouseMotionListener;
	Ogre::MouseListener* mMouseListener;
	Ogre::KeyListener* mKeyListener;
	
	Ogre::MouseEvent* mMouseReleasedOgreEvent;
	Ogre::MouseEvent* mMousePressedOgreEvent;
	
	MousePicker* mMousePicker;

	//events
	bool mSheet_MouseButtonDown(const CEGUI::EventArgs& args);
	bool mSheet_CaptureLost(const CEGUI::EventArgs& args);
	
	//takes a screen shot and writes it to disk
	const std::string takeScreenshot();

	
	

};
}


#endif 
