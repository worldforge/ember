#ifndef InputManager_H__
#define InputManager_H__

#include <OgreEventQueue.h>
#include <OgreFrameListener.h>
#include <OgreInput.h>
#include <OgreNoMemoryMacros.h>

#include <list>

//#include "KeyboardCarController.h"



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

