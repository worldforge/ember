/*
	InputManager.h by Wolfman8k
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

#ifndef InputManager_H__
#define InputManager_H__

#include <OgreEventQueue.h>
#include <OgreFrameListener.h>
#include <OgreEventListeners.h>
#include <OgreInput.h>
#include <OgreNoMemoryMacros.h>

#include <list>

namespace DimeOgre {

class DimeEventProcessor;
class DebugListener;
class AvatarController;


// TODO this should probably be an abstract class and there
// should be specialised versions of it
//
// For now we use a simple implementation that powers a
// KeyboardCarController and a Console
class InputManager : public Ogre::FrameListener, public Ogre::KeyListener, public Ogre::MouseListener, public Ogre::MouseMotionListener
{
	public:
/*		class MouseListener
		{
			public:
				virtual ~MouseListener() {}; // to avoid warnings complaining about non-virtual dtors in children classes
				virtual void mouseMoved(short newX, short newY, short oldX, short oldY) = 0;
				virtual void mousePressed(unsigned char button) = 0;
				virtual void mouseReleased(unsigned char button) = 0;
		};
*/
		//static InputManager & getSingleton(void);

		bool frameStarted(const Ogre::FrameEvent & evt);
		bool frameEnded(const Ogre::FrameEvent & evt)
		{
			return true;
		}

		bool isKeyDown(Ogre::KeyCode kc) { return mKeyDown[kc]; };

		Ogre::Real getMouseX(void) { return mMouseX; };
		Ogre::Real getMouseY(void) { return mMouseY; };

/*		void addKeyListener(Ogre::KeyListener *l);
		void removeKeyListener(Ogre::KeyListener *l);

		void addMouseListener(MouseListener *l);
		void removeMouseListener(MouseListener *l);
*/
		//these are for Ogre::KeyListener
		virtual void keyClicked(Ogre::KeyEvent* e); 
		virtual void keyPressed(Ogre::KeyEvent* e); 
		virtual void keyReleased(Ogre::KeyEvent* e);
		
		//these are for Ogre::MouseListener
		virtual void mouseClicked(Ogre::MouseEvent* e);
 		virtual void mouseEntered(Ogre::MouseEvent* e);
 		virtual void mouseExited(Ogre::MouseEvent* e);
  		virtual void mousePressed(Ogre::MouseEvent* e);
 		virtual void mouseReleased(Ogre::MouseEvent* e);
 		
 		//these are for Ogre::MouseMotionListener
 		virtual void mouseMoved(Ogre::MouseEvent* e);
		virtual void mouseDragged(Ogre::MouseEvent* e);
		virtual void mouseDragMoved(Ogre::MouseEvent* e);
 		
		void setAvatarController(AvatarController* avatarController);

		
		//toggle between making the application grab the mouse and not
		void toggleMouse();
		bool isMouseUsed();
		
		InputManager(void);
		~InputManager();
		
		Ogre::EventProcessor* getEventProcessor() { return mEventProcessor; }

	protected:
//		virtual void connectMouseListeners() ;
		
		//creates a new DimeEventProcessor which will take care of all input processing
		//this (mEventProcessor) is hooked to methods such as keyPressed and so on
		virtual void createEventProcessor();		
		
		//creates a mouse cursor which can be moved around the scene
		//virtual void createMouseCursor();

		Ogre::EventProcessor* mEventProcessor;

		AvatarController* mAvatarController;
		
		
	private:
		static InputManager* _instance;
		



//		Ogre::InputReader* mInputReader;
//		Ogre::EventQueue mEventQueue;


		enum { NUM_KEYS=256 };
		bool mKeyDown[NUM_KEYS];

		std::list<Ogre::KeyListener*> mKeyListenerList;
		std::list<MouseListener*> mMouseListenerList;

		Ogre::Real mMouseX;
		Ogre::Real mMouseY;

		unsigned short mScreenX;
		unsigned short mScreenY;

		
		
		
};
}



#endif

