/*
    Copyright (C) 2004  Miguel Guzman (Aganor)

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

#include "widgets/Widget.h"
#include "widgets/ConsoleWidget.h"
#include "widgets/ChatWidget.h"
#include "MousePicker.h"
#include "GUIManager.h"


namespace DimeOgre {


GUIManager::GUIManager(Ogre::RenderWindow* window, Ogre::SceneManager* sceneMgr) 
: mWindow(window)
, mQuit(false)
, mMouseMotionListener(0)
, mMouseListener(0)
, mKeyListener(0)
, mInGUIMode(true)

{

	mEventProcessor = new Ogre::EventProcessor();
	mEventProcessor->initialise(Ogre::Root::getSingleton().getAutoCreatedWindow());
	mEventProcessor->startProcessingEvents();

/*	
	try {
*/	
		fprintf(stderr, "CEGUI - ENTERING CTOR\n");
		
		mGuiRenderer = new CEGUI::OgreRenderer(window, Ogre::RENDER_QUEUE_OVERLAY, false, 3000, sceneMgr);
		fprintf(stderr, "CEGUI - RENDERER CREATED\n");
		
		mGuiRenderer->setTargetSceneManager(sceneMgr);
		fprintf(stderr, "CEGUI - AND SCENE MANAGER SET\n");
		
		mGuiSystem = new CEGUI::System(mGuiRenderer); 
		fprintf(stderr, "CEGUI - SYSTEM CREATED\n");

		CEGUI::SchemeManager::getSingleton().loadScheme((CEGUI::utf8*)"cegui/guischeme.xml");
		fprintf(stderr, "CEGUI - TEST SCHEME LOADED\n");
		
		mGuiSystem->setDefaultMouseCursor((CEGUI::utf8*)"TaharezImagery", (CEGUI::utf8*)"MouseArrow");
		mGuiSystem->setDefaultFont((CEGUI::utf8*)"Tahoma-14"); 
		fprintf(stderr, "CEGUI - DEFAULTS SET\n");
		
		mSheet = CEGUI::WindowManager::getSingleton().createWindow((CEGUI::utf8*)"DefaultGUISheet", (CEGUI::utf8*)"root_wnd");
		mGuiSystem->setGUISheet(mSheet); 
		mSheet->subscribeEvent(CEGUI::ButtonBase::MouseButtonDownEvent, 
			boost::bind(&GUIManager::mSheet_MouseButtonDown, this, _1));
		mSheet->subscribeEvent(CEGUI::Window::CaptureLostEvent, 
			boost::bind(&GUIManager::mSheet_CaptureLost, this, _1));
			

		fprintf(stderr, "CEGUI - SHEET CREATED\n");


	mEventProcessor->addKeyListener(this);
	mEventProcessor->addMouseMotionListener(this);
	mEventProcessor->addMouseListener(this);
	Ogre::Root::getSingleton().addFrameListener(this);

}


GUIManager::~GUIManager()
{
	mEventProcessor->removeKeyListener(this);
	mEventProcessor->removeMouseMotionListener(this);
	mEventProcessor->removeMouseListener(this);
	delete mEventProcessor;
	delete mGuiSystem;
	delete mGuiRenderer;

}

void GUIManager::initialize()
{
		fprintf(stderr, "CEGUI - CREATING CONSOLE\n");
	
		mConsoleWidget = new ConsoleWidget(this);
		mConsoleWidget->buildWidget();
		addWidget(mConsoleWidget);
		fprintf(stderr, "CEGUI - CREATED CONSOLE\n");
		
		ChatWidget* chatWidget = new ChatWidget(this);
		chatWidget->buildWidget();
		addWidget(chatWidget);		
	
}

CEGUI::Window* GUIManager::getMainSheet() 
{ 
	return mSheet; 
}

void GUIManager::removeWidget(Widget* widget)
{
	mWidgets.erase(widget);
}

void GUIManager::addWidget(Widget* widget)
{
	mWidgets.insert(widget);
}


bool GUIManager::frameStarted(const Ogre::FrameEvent& evt)
{
	//iterate over all widgets and send them a frameStarted event
	std::set<Widget*>::iterator I = mWidgets.begin();
	std::set<Widget*>::iterator I_end = mWidgets.end();
	
	for (; I != I_end; ++I) {
		Widget* aWidget = *I;
		aWidget->frameStarted(evt);
	}
	
	if (mQuit) {
		mEventProcessor->removeKeyListener(this);
		mEventProcessor->removeMouseMotionListener(this);
		mEventProcessor->removeMouseListener(this);
		delete mEventProcessor;
	
		return false;
	} else {
//		updateStats();
		return true;
	}

}

void GUIManager::mSheet_MouseButtonDown(const CEGUI::EventArgs& args)
{
	fprintf(stderr, "CEGUI - MAIN SHEET CAPTURING INPUT\n");
	CEGUI::Window* aWindow = CEGUI::Window::getCaptureWindow();
	if (aWindow) {
		aWindow->releaseInput();
		aWindow->deactivate();
	}
	mSheet->activate();
	mSheet->captureInput();

//TODO: implement this
/*
	if (mMousePicker) {
		mMousePicker->performMousePicking(mMousePressedOgreEvent, args);
	}
*/
}

void GUIManager::mSheet_CaptureLost(const CEGUI::EventArgs& args)
{
	fprintf(stderr, "CEGUI - MAIN SHEET RELEASE INPUT\n");
}

bool GUIManager::isInMovementKeysMode() {
	return mSheet->isCapturedByThis() || !mInGUIMode; 
}



void GUIManager::mouseMoved (Ogre::MouseEvent *e)
{
	if (mInGUIMode) {
		mGuiSystem->injectMouseMove(e->getRelX() * mGuiRenderer->getWidth(), e->getRelY() * mGuiRenderer->getHeight());
		e->consume();
	} else {
		if (mMouseMotionListener) {
			mMouseMotionListener->mouseMoved(e);
		}
	}
}


void GUIManager::mouseDragged (Ogre::MouseEvent *e)
{
	mouseMoved(e);
}


void GUIManager::keyPressed (Ogre::KeyEvent *e)
{
	if (mInGUIMode) {
		// do event injection
		// key down
		mGuiSystem->injectKeyDown(e->getKey());
	
		// now character
		mGuiSystem->injectChar(e->getKeyChar());
	
		e->consume();
	} else {
		if (mKeyListener) {
			mKeyListener->keyPressed(e);
		}
	}
	
}


void GUIManager::keyReleased (Ogre::KeyEvent *e)
{

	if (mInGUIMode) {
		//toggle the console
		//we've put it here because we wan't the console to always be available
		if(e->getKey() == Ogre::KC_GRAVE || e->getKey() == Ogre::KC_F12)
		{
			mConsoleWidget->toggleActive();
		}
			
		mGuiSystem->injectKeyUp(e->getKey());
		e->consume();
	} else {
		if (mKeyListener) {
			mKeyListener->keyReleased(e);
		}
	}


}



void GUIManager::mousePressed (Ogre::MouseEvent *e)
{
	mMousePressedOgreEvent = e;
	//if the right mouse button is pressed, switch from gui mode
	if (e->getButtonID() == Ogre::MouseEvent::BUTTON1_MASK) {
		mInGUIMode = false;
	}
	if (mInGUIMode) {
		mGuiSystem->injectMouseButtonDown(convertOgreButtonToCegui(e->getButtonID()));
		e->consume();
	} else {
		if (mMouseListener) {
			mMouseListener->mousePressed(e);
		}
	}

}


void GUIManager::mouseReleased (Ogre::MouseEvent *e)
{
	mMouseReleasedOgreEvent = e;
	//if the right mouse button is released, switch to gui mode
	if (e->getButtonID() == Ogre::MouseEvent::BUTTON1_MASK) {
		mInGUIMode = true;
	}
	if (mInGUIMode) {
		mGuiSystem->injectMouseButtonUp(convertOgreButtonToCegui(e->getButtonID()));
		e->consume();
	} else {
		if (mMouseListener) {
			mMouseListener->mouseReleased(e);
		}
	}

}

CEGUI::MouseButton GUIManager::convertOgreButtonToCegui(int ogre_button_id)
{
	switch (ogre_button_id)
	{
	case Ogre::MouseEvent::BUTTON0_MASK:
		return CEGUI::LeftButton;
		break;

	case Ogre::MouseEvent::BUTTON1_MASK:
		return CEGUI::RightButton;
		break;

	case Ogre::MouseEvent::BUTTON2_MASK:
		return CEGUI::MiddleButton;
		break;

	case Ogre::MouseEvent::BUTTON3_MASK:
		return CEGUI::X1Button;
		break;

	default:
		return CEGUI::LeftButton;
		break;
	}

}




template<> GUIManager* dime::Singleton<GUIManager>::ms_Singleton = 0;

}
