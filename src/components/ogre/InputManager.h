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
#include <OgreInput.h>
#include <OgreNoMemoryMacros.h>

#include <list>

#include "AvatarKeyboardListener.h"  // TODO: should be "controller" instead of listener



// TODO this should probably be an abstract class and there
// should be specialised versions of it
//
// For now we use a simple implementation that powers a
// KeyboardCarController and a Console
class InputManager : public Ogre::FrameListener
{
	public:
		class MouseListener
		{
			public:
				virtual void mouseMoved(short newX, short newY, short oldX, short oldY) = 0;
				virtual void mousePressed(unsigned char button) = 0;
				virtual void mouseReleased(unsigned char button) = 0;
		};
		static InputManager & getSingleton(void);

		bool frameStarted(const Ogre::FrameEvent & evt);
		bool frameEnded(const Ogre::FrameEvent & evt) {}

		bool isKeyDown(Ogre::KeyCode kc) { return mKeyDown[kc]; };

		unsigned short getMouseX(void) { return mMouseX; };
		unsigned short getMouseY(void) { return mMouseY; };

		void addKeyListener(Ogre::KeyListener *l);
		void removeKeyListener(Ogre::KeyListener *l);

		void addMouseListener(MouseListener *l);
		void removeMouseListener(MouseListener *l);

	private:
		static InputManager* _instance;
		bool worldConnected; // UGLY HACK!!

		InputManager(void);
		~InputManager();

		Ogre::InputReader* mInputReader;
		Ogre::EventQueue mEventQueue;

		enum { NUM_KEYS=256 };
		bool mKeyDown[NUM_KEYS];

		std::list<Ogre::KeyListener*> mKeyListenerList;
		std::list<MouseListener*> mMouseListenerList;

		unsigned short mMouseX;
		unsigned short mMouseY;

		unsigned short mScreenX;
		unsigned short mScreenY;

		int timer;
};


#endif

