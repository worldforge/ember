#include <OgreKeyEvent.h>
#include <OgreInput.h>
#include <OgreRoot.h>
#include <OgreNoMemoryMacros.h>

#include "InputManager.h"
#include "Console.h"


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
	timer=600;

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

	mInputReader->capture();
	while(mEventQueue.getSize() > 0)
	{
		Ogre::InputEvent* e = mEventQueue.pop();

		fprintf(stderr, "TRACE - EVENT EVENT EVENT EVENT\n");

		if(e->getID() == Ogre::KeyEvent::KE_KEY_PRESSED)
		{
			fprintf(stderr, "TRACE - KEY event\n");
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


