/*
	InputManager.cpp by Wolfman8k
	Adapted to Worldforge by Miguel Guzman (Aglanor)
	with permission and lots of help from Wolfman8k

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

#include <OgreKeyEvent.h>
#include <OgreInput.h>
#include <OgreRoot.h>
#include <OgreNoMemoryMacros.h>
#include <OgreCursorGuiElement.h>
#include <OgreOverlayManager.h>



#include <Eris/PollDefault.h>
#include <Eris/Log.h>
#include <Eris/World.h>

// ------------------------------
// Include dime header files
// ------------------------------
#include "services/DimeServices.h"
#include "services/server/ServerService.h"
#include "services/logging/LoggingService.h"

#include "EntityListener.h"
#include "DimeEventProcessor.h"
#include "DebugListener.h"
#include "AvatarController.h" 

#include "DimeEntityFactory.h"
#include "EntityListener.h"

#include "Console.h"

#include "InputManager.h"

using dime::LoggingService;

InputManager* InputManager::_instance = 0;

InputManager & InputManager::getSingleton(void)
{
	if(_instance == 0)
		_instance = new InputManager;
	return *_instance;
}

InputManager::InputManager(void)
{
	

	createEventProcessor();

//	mEventQueue = mEventProcessor->
	
	createMouseCursor();
	
	Ogre::Root::getSingleton().addFrameListener(this);
	
	//DEBUG
	Ogre::Root::getSingleton().addFrameListener(&(DebugListener::getSingleton()));

	int i;
	for(i=0; i<NUM_KEYS; i++)
		mKeyDown[i] = false;

	mScreenX = Ogre::Root::getSingleton().getAutoCreatedWindow()->getWidth();
	mScreenY = Ogre::Root::getSingleton().getAutoCreatedWindow()->getHeight();
	

	// hack: fallback in case there's no way to exit the app;
	timer=6000;
	worldConnected = false; // UGLY HACK
	
}

InputManager::~InputManager()
{
	//mEventQueue.activateEventQueue(false);
	Ogre::Root::getSingleton().removeFrameListener(this);
	//mInputReader->useBufferedInput(NULL, false, false);
	//mInputReader->setBufferedInput(false, false);
	delete mEventProcessor;
}


void InputManager::toggleMouse()
{
	mEventProcessor->toggleMouse();
	connectMouseListeners();
}

bool InputManager::isMouseUsed()
{
	return mEventProcessor->isMouseUsed();	
}


void InputManager::createMouseCursor()
{
    Ogre::OverlayManager::getSingleton().createCursorOverlay();
	Ogre::GuiContainer* pCursorGui = Ogre::OverlayManager::getSingleton().getCursorGui();
	pCursorGui->setMaterialName("Cursor/default");
	pCursorGui->setDimensions(32.0/640.0, 32.0/480.0);	
	pCursorGui->setPosition (0.5, 0.5);
}

void InputManager::connectMouseListeners() 
{
	if (mEventProcessor->isMouseUsed()) {
		Ogre::OverlayManager::getSingleton().addMouseMotionListener(this);
		Ogre::OverlayManager::getSingleton().addMouseListener(this);
	} else {
		Ogre::OverlayManager::getSingleton().removeMouseMotionListener(this);
		Ogre::OverlayManager::getSingleton().removeMouseListener(this);
	}
	
}

void InputManager::createEventProcessor() 
{
	mEventProcessor = new DimeEventProcessor();
	mEventProcessor->initialise(Ogre::Root::getSingleton().getAutoCreatedWindow());
	mEventProcessor->startProcessingEvents();
	mEventProcessor->addKeyListener(this);
	connectMouseListeners();
}

void InputManager::keyClicked(Ogre::KeyEvent* e) {}
	
void InputManager::keyPressed(Ogre::KeyEvent* e) 
{
	//fprintf(stderr, ((Ogre::KeyEvent*)e)->getKey());
	mKeyDown[e->getKey()]=true;
	
	//allow the user to release the mouse from the application
	//we'll put this here because it's such a central function
	if(e->getKey() == Ogre::KC_F9) 
	{
        S_LOG_VERBOSE() << "Toggle mouse\n";
        toggleMouse();
	}			
	
	std::list<Ogre::KeyListener*>::iterator i;
	for(i=mKeyListenerList.begin(); i!=mKeyListenerList.end(); i++)
		(*i)->keyPressed((Ogre::KeyEvent*)e);
}

void InputManager::keyReleased(Ogre::KeyEvent* e)
{
	mKeyDown[e->getKey()]=false;
	std::list<Ogre::KeyListener*>::iterator i;
	for(i=mKeyListenerList.begin(); i!=mKeyListenerList.end(); i++)
		(*i)->keyReleased(e);
}


 void InputManager::mouseClicked(Ogre::MouseEvent* e) {
 
 }
 void InputManager::mouseEntered(Ogre::MouseEvent* e) {}
 void InputManager::mouseExited(Ogre::MouseEvent* e) {}
 
 void InputManager::mousePressed(Ogre::MouseEvent* e)
 {
 	//debug stuff
	//Ogre::SceneManager* sceneManager = EntityListener::getSingleton().getSceneManager();
 	//sceneManager->getSceneNode("Debug_1_0_0_Node")->setScale(1,1,1);
 	
 	
 	
	std::list<MouseListener*>::iterator i;
	for(i=mMouseListenerList.begin(); i!=mMouseListenerList.end(); i++)
	{
		if(e->getButtonID() == 16)
			(*i)->mousePressed(0);
		else if(e->getButtonID() == 32)
			(*i)->mousePressed(1);
		else if(e->getButtonID() == 64)
			(*i)->mousePressed(2);
	}	
 }
 
 void InputManager::mouseReleased(Ogre::MouseEvent* e) {
	//debug stuff
	//Ogre::SceneManager* sceneManager = EntityListener::getSingleton().getSceneManager();
 	//sceneManager->getSceneNode("Debug_1_0_0_Node")->setScale(OGRESCALER);

	std::list<MouseListener*>::iterator i;
	for(i=mMouseListenerList.begin(); i!=mMouseListenerList.end(); i++)
	{
		if(e->getButtonID() == 16)
			(*i)->mouseReleased(0);
		else if(e->getButtonID() == 32)
			(*i)->mouseReleased(1);
		else if(e->getButtonID() == 64)
			(*i)->mouseReleased(2);
	}	
 }
 
 
 void InputManager::mouseMoved(Ogre::MouseEvent* e)
 {
 	mouseMovedOrDragged(e);
 }
 	
 void InputManager::mouseDragged(Ogre::MouseEvent* e) {
 	mouseMovedOrDragged(e);
 }
 
 void InputManager::mouseMovedOrDragged(Ogre::MouseEvent* e)
 {
 			std::list<MouseListener*>::iterator i;
			for(i=mMouseListenerList.begin(); i!=mMouseListenerList.end(); i++)
			{
				(*i)->mouseMoved((short)(e->getX()*mScreenX),
								(short)(e->getY()*mScreenY),
								(short)((e->getX()-e->getRelX())*mScreenX),
								(short)((e->getY()-e->getRelY())*mScreenY));
			}
 	
 }


bool InputManager::frameStarted(const Ogre::FrameEvent & evt)
{
	

	// hack: fallback in case there's no way to exit the app;
	//in linux there's always a way /erik
	/*timer--;
	if(timer<=0)
	{
		return false;
	}
	*/
	mMouseX = Ogre::OverlayManager::getSingleton().getMouseX();
	mMouseY = Ogre::OverlayManager::getSingleton().getMouseY();
	// Eris poll. It might remain here or be moved to a better place. We'll see.
	Eris::PollDefault::poll();

	// VERY UGLY HACK
	// Grab world. If it's set, connect world signals on the entity thing...
	if(!worldConnected) // to connect only once
	{
		// if world is initialized, connect the signals (once)
		if((dime::DimeServices::getInstance()->getServerService()->getWorld())!=0) {
		    DimeEntityFactory* dimeEntityFactory = new DimeEntityFactory(EntityListener::getSingleton().getSceneManager());
		    dime::DimeServices::getInstance()->getServerService()->getWorld()->registerFactory(dimeEntityFactory, 10);
			EntityListener::getSingleton().connectWorldSignals();
		    
			worldConnected = true;
			
		}
	}

	// Call to the AvatarListener methods for the unbuffered input
	AvatarController::getSingleton().frameStarted(evt, mEventProcessor->getInputReader());

	return true;
}



void InputManager::addKeyListener(Ogre::KeyListener *l)
{
	mKeyListenerList.push_back(l);
}

void InputManager::removeKeyListener(Ogre::KeyListener *l)
{
	// TODO do this...i am lazy now
}

void InputManager::addMouseListener(MouseListener *l)
{
	mMouseListenerList.push_back(l);
}

void InputManager::removeMouseListener(MouseListener *l)
{
	// TODO do this...i am lazy now
}


