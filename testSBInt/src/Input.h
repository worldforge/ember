#ifndef INPUT_H
#define INPUT_H

	#include <Ogre.h>
	#include <OIS/OIS.h>

class InputListener : public Ogre::FrameListener, public Ogre::WindowEventListener
{
public:
	InputListener(Ogre::RenderWindow *wnd, Ogre::Camera *camera);
	~InputListener();

	bool frameRenderingQueued(const Ogre::FrameEvent& evt);

	virtual void windowResized(Ogre::RenderWindow* rw);
	virtual void windowClosed(Ogre::RenderWindow* rw);

	void startOIS();


private:
	Ogre::RenderWindow*	mWindow;
	Ogre::Camera*		mCamera;

	OIS::InputManager*	mInputManager;
	OIS::Mouse*			mMouse;
	OIS::Keyboard*		mKeyboard;
};

#endif