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


#include <Eris/PollDefault.h>
#include <Eris/Log.h>
#include <Eris/World.h>

// ------------------------------
// Include dime header files
// ------------------------------
#include "services/DimeServices.h"
#include "services/server/ServerService.h"

#include "InputManager.h"
#include "Console.h"
#include "EntityListener.h"


InputManager* InputManager::_instance = 0;

InputManager & InputManager::getSingleton(void)
{
	fprintf(stderr, "TRACE - INPUT MANAGER - SINGLETON ENTERING\n");
	if(_instance == 0)
		_instance = new InputManager;
	return *_instance;
}

InputManager::InputManager(void)
{
	fprintf(stderr, "TRACE - INPUT MANAGER - CONTRUCTOR ENTERING\n");
	mEventQueue.activateEventQueue(true);

	mInputReader = Ogre::PlatformManager::getSingleton().createInputReader();
	mInputReader->useBufferedInput(&mEventQueue, true, true);
	mInputReader->initialise(Ogre::Root::getSingleton().getAutoCreatedWindow(), true, true);

	Ogre::Root::getSingleton().addFrameListener(this);
	fprintf(stderr, "TRACE - INPUT MANAGER - CONTRUCTOR - ADDED FRAME LISTENER\n");

	int i;
	for(i=0; i<NUM_KEYS; i++)
		mKeyDown[i] = false;

	mScreenX = Ogre::Root::getSingleton().getAutoCreatedWindow()->getWidth();
	mScreenY = Ogre::Root::getSingleton().getAutoCreatedWindow()->getHeight();

	// hack: fallback in case there's no way to exit the app;
	timer=6000;
	worldConnected = false; // UGLY HACK

	fprintf(stderr, "TRACE - INPUT MANAGER - CONTRUCTOR DONE\n");
}

InputManager::~InputManager()
{
	mEventQueue.activateEventQueue(false);
	Ogre::Root::getSingleton().removeFrameListener(this);
	mInputReader->useBufferedInput(NULL, false, false);
	mInputReader->setBufferedInput(false, false);
}

bool InputManager::frameStarted(const Ogre::FrameEvent & evt)
{

	// hack: fallback in case there's no way to exit the app;
	timer--;
	if(timer<=0)
	{
		return false;
	}

	// Eris poll. It might remain here or be moved to a better place. We'll see.
	Eris::PollDefault::poll();

	// VERY UGLY HACK
	// Grab world. If it's set, connect world signals on the entity thing...
	if(!worldConnected) // to connect only once
	{
		// if world is initialized, connect the signals (once)
		if((dime::DimeServices::getInstance()->getServerService()->getWorld())!=0) {
			EntityListener::getSingleton().connectWorldSignals();
			worldConnected = true;
		}
	}

	mInputReader->capture();
	while(mEventQueue.getSize() > 0)
	{
		Ogre::InputEvent* e = mEventQueue.pop();

		if(e->getID() == Ogre::KeyEvent::KE_KEY_PRESSED)
		{
			//fprintf(stderr, ((Ogre::KeyEvent*)e)->getKey());
			mKeyDown[((Ogre::KeyEvent*)e)->getKey()]=true;
			std::list<Ogre::KeyListener*>::iterator i;
			for(i=mKeyListenerList.begin(); i!=mKeyListenerList.end(); i++)
				(*i)->keyPressed((Ogre::KeyEvent*)e);
		}
		else if(e->getID() == Ogre::KeyEvent::KE_KEY_RELEASED)
		{
			mKeyDown[((Ogre::KeyEvent*)e)->getKey()]=false;
			std::list<Ogre::KeyListener*>::iterator i;
			for(i=mKeyListenerList.begin(); i!=mKeyListenerList.end(); i++)
				(*i)->keyReleased((Ogre::KeyEvent*)e);
		}
		else if(e->getID() == Ogre::MouseEvent::ME_MOUSE_PRESSED)
		{
			std::list<MouseListener*>::iterator i;
			for(i=mMouseListenerList.begin(); i!=mMouseListenerList.end(); i++)
			{
				if(((Ogre::MouseEvent*)e)->getButtonID() == 16)
					(*i)->mousePressed(0);
				else if(((Ogre::MouseEvent*)e)->getButtonID() == 32)
					(*i)->mousePressed(1);
				else if(((Ogre::MouseEvent*)e)->getButtonID() == 64)
					(*i)->mousePressed(2);
			}

		}
		else if(e->getID() == Ogre::MouseEvent::ME_MOUSE_RELEASED)
		{
			std::list<MouseListener*>::iterator i;
			for(i=mMouseListenerList.begin(); i!=mMouseListenerList.end(); i++)
			{
				if(((Ogre::MouseEvent*)e)->getButtonID() == 16)
					(*i)->mouseReleased(0);
				else if(((Ogre::MouseEvent*)e)->getButtonID() == 32)
					(*i)->mouseReleased(1);
				else if(((Ogre::MouseEvent*)e)->getButtonID() == 64)
					(*i)->mouseReleased(2);
			}

		}
		else if(e->getID() == Ogre::MouseEvent::ME_MOUSE_MOVED || e->getID() == Ogre::MouseEvent::ME_MOUSE_DRAGGED)
		{
			std::list<MouseListener*>::iterator i;
			for(i=mMouseListenerList.begin(); i!=mMouseListenerList.end(); i++)
			{
				(*i)->mouseMoved((short)(((Ogre::MouseEvent*)e)->getX()*mScreenX),
								(short)(((Ogre::MouseEvent*)e)->getY()*mScreenY),
								(short)((((Ogre::MouseEvent*)e)->getX()-((Ogre::MouseEvent*)e)->getRelX())*mScreenX),
								(short)((((Ogre::MouseEvent*)e)->getY()-((Ogre::MouseEvent*)e)->getRelY())*mScreenY));
			}
		}

		delete e;
	}

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


