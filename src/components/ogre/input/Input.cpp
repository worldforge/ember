//
// C++ Implementation: Input
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2005
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//
#include "Input.h"
#include "../EmberOgre.h"

#include "framework/scrap.h"
#include "IInputAdapter.h"
namespace EmberOgre {




Input::Input()
:
mCurrentInputMode(IM_GUI)
, mMouseState(0)
, mTimeSinceLastRightMouseClick(1000)
{
	
	
	//we don't want to send a injectChar to the gui for some keys, put them here
	mNonCharKeys.insert(SDLK_ESCAPE);
	mNonCharKeys.insert(SDLK_F1);
	mNonCharKeys.insert(SDLK_F2);
	mNonCharKeys.insert(SDLK_F3);
	mNonCharKeys.insert(SDLK_F4);
	mNonCharKeys.insert(SDLK_F5);
	mNonCharKeys.insert(SDLK_F6);
	mNonCharKeys.insert(SDLK_F7);
	mNonCharKeys.insert(SDLK_F8);
	mNonCharKeys.insert(SDLK_F9);
	mNonCharKeys.insert(SDLK_F10);
	mNonCharKeys.insert(SDLK_F11);
	mNonCharKeys.insert(SDLK_F12);
	mNonCharKeys.insert(SDLK_BACKSPACE);
	mNonCharKeys.insert(SDLK_TAB);
	mNonCharKeys.insert(SDLK_RETURN);
	mNonCharKeys.insert(SDLK_DELETE);


	
	
}


Input::~Input()
{

}

void Input::initialize()
{
	SDL_ShowCursor(0);
	SDL_EnableUNICODE(1);
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
	
	///must initialize the clipboard support
	if ( init_scrap() < 0 ) {
		S_LOG_FAILURE("Couldn't init clipboard: \n" << SDL_GetError());
	}
}

void Input::setGeometry(int width, int height)
{
	mScreenWidth = width;
	mScreenHeight = height;
}


void Input::processInput(const Ogre::FrameEvent& evt)
{
	
	pollMouse(evt);
	pollKeyboard(evt);
    SDL_PumpEvents();
}


void Input::pollMouse(const Ogre::FrameEvent& evt)
{
	int mouseX, mouseY;
	unsigned int mouseState;
	
	mouseState = SDL_GetMouseState( &mouseX, &mouseY );
	
	mTimeSinceLastRightMouseClick += evt.timeSinceLastFrame;
	
	if (mouseState & SDL_BUTTON(SDL_BUTTON_WHEELUP)) {
		EventMouseButtonPressed.emit(MouseWheelUp, mCurrentInputMode);
	}
	if (mouseState & SDL_BUTTON(SDL_BUTTON_WHEELDOWN)) {
		EventMouseButtonPressed.emit(MouseWheelDown, mCurrentInputMode);
	}

	if (mouseState & SDL_BUTTON_RMASK) {
		if (!(mMouseState & SDL_BUTTON_RMASK)) {
			//right mouse button pressed
			
			//if the right mouse button is pressed, switch from gui mode
			
			toggleInputMode();
			EventMouseButtonPressed.emit(MouseButtonRight, mCurrentInputMode);
			
		}
	} else if (mMouseState & SDL_BUTTON_RMASK) {
		//right mouse button released
		
		//if there's two right mouse clicks withing 0.25 seconds from each others, it's a double click
/*		if (mTimeSinceLastRightMouseClick < 0.25) {
			toggleInputMode();
			
		}*/
		mTimeSinceLastRightMouseClick = 0;
		//toggleInputMode();
		EventMouseButtonReleased.emit(MouseButtonRight, mCurrentInputMode);
	} 
	
	
	
	
	if (mouseState & SDL_BUTTON_LMASK) {
		if (!(mMouseState & SDL_BUTTON_LMASK)) {
			//left mouse button pressed
			
			for (IInputAdapterStore::reverse_iterator I = mAdapters.rbegin(); I != mAdapters.rend(); ++I) {
				if (*I) {
					if (!(*I)->injectMouseButtonDown(Input::MouseButtonLeft)) break;
				}
			}


			EventMouseButtonPressed.emit(MouseButtonLeft, mCurrentInputMode);
		}
	} else if (mMouseState & SDL_BUTTON_LMASK) {
		//left mouse button released
		for (IInputAdapterStore::reverse_iterator I = mAdapters.rbegin(); I != mAdapters.rend(); ++I) {
			if (!(*I)->injectMouseButtonUp(Input::MouseButtonLeft)) break;
		}
			
		EventMouseButtonReleased.emit(MouseButtonLeft, mCurrentInputMode);

	}
	
	if (mouseState & SDL_BUTTON_MMASK) {
		if (!(mMouseState & SDL_BUTTON_MMASK)) {
			//middle mouse button pressed
			for (IInputAdapterStore::reverse_iterator I = mAdapters.rbegin(); I != mAdapters.rend(); ++I) {
				if (!(*I)->injectMouseButtonDown(Input::MouseButtonMiddle)) break;
			}
				
			EventMouseButtonPressed.emit(MouseButtonMiddle, mCurrentInputMode);
		}
	} else if (mMouseState & SDL_BUTTON_MMASK) {
		//middle mouse button released
		for (IInputAdapterStore::reverse_iterator I = mAdapters.rbegin(); I != mAdapters.rend(); ++I) {
			if (!(*I)->injectMouseButtonUp(Input::MouseButtonMiddle)) break;
		}
			
		EventMouseButtonReleased.emit(MouseButtonMiddle, mCurrentInputMode);
	}
	
	mMouseState = mouseState;
	
	
	
	//has the mouse moved?
	Uint8 appState = SDL_GetAppState();
//	S_LOG_INFO((appState & SDL_APPMOUSEFOCUS));
	if (appState & SDL_APPMOUSEFOCUS) {
		if (mMouseX != mouseX || mMouseY != mouseY)
		{
	
			//we'll calculate the mouse movement difference and send the values to those
			//listening to the MouseMoved event
			Ogre::Real diffX, diffY;
			diffX =  (mMouseX - mouseX) / mScreenWidth;
			diffY = (mMouseY - mouseY) / mScreenHeight;
			MouseMotion motion;
			motion.xPosition = mouseX;
			motion.yPosition = mouseY;
			motion.xRelativeMovement = diffX;
			motion.yRelativeMovement = diffY;
			motion.xRelativeMovementInPixels = mMouseX - mouseX;
			motion.yRelativeMovementInPixels = mMouseY - mouseY;
			motion.timeSinceLastMovement = evt.timeSinceLastFrame;
			
			EventMouseMoved.emit(motion, mCurrentInputMode);
			
			bool freezeMouse = false;
			//if we're in gui mode, we'll just send the mouse movement on to CEGUI
			if (mCurrentInputMode == IM_GUI) {
				
				for (IInputAdapterStore::reverse_iterator I = mAdapters.rbegin(); I != mAdapters.rend(); ++I) {
					if (!(*I)->injectMouseMove(motion, freezeMouse)) break;
				}
				
				
			} else {
				freezeMouse = true;
			}
			
			
			if (freezeMouse) {
				SDL_WarpMouse(mMouseX, mMouseY);
			} else {
				mMouseX = mouseX;
				mMouseY = mouseY;
			}
		
		}
	}

}



void Input::pollKeyboard(const Ogre::FrameEvent& evt)
{
  SDL_Event event;
    while( SDL_PollEvent( &event ) ){
        switch( event.type ){
            /* Look for a keypress */
            case SDL_KEYDOWN:
			case SDL_KEYUP:
				keyChanged(event.key);
				break;
			case SDL_QUIT:
				EmberOgre::getSingleton().requestQuit();
				break;
		}
	}
}

void Input::pasteFromClipboard()
{
	static char *scrap = 0;
	int scraplen;

	get_scrap(T('T','E','X','T'), &scraplen, &scrap);
	for (int i = 0; i < scraplen; ++i) {
		for (IInputAdapterStore::reverse_iterator I = mAdapters.rbegin(); I != mAdapters.rend(); ++I) {
			if (!(*I)->injectChar(scrap[i])) break;
		}
	}
}

void Input::keyChanged(const SDL_KeyboardEvent &keyEvent)
{
	//catch paste key presses
	
	//check for paste actions
	if ((keyEvent.keysym.mod & KMOD_CTRL || keyEvent.keysym.mod & KMOD_LCTRL || keyEvent.keysym.mod & KMOD_RCTRL) && keyEvent.keysym.sym == SDLK_v) {
		if (keyEvent.type == SDL_KEYDOWN) {
			pasteFromClipboard();
		}
	} else {
		if (keyEvent.type == SDL_KEYDOWN) {
			mKeysPressed.insert(keyEvent.keysym.sym);
			keyPressed(keyEvent);
		} else {
			mKeysPressed.erase(keyEvent.keysym.sym);
			keyReleased(keyEvent);
		}
	}
	
}

const bool Input::isKeyDown(const SDLKey &key) const
{
	return mKeysPressed.find(key) != mKeysPressed.end();
}

void Input::keyPressed (const SDL_KeyboardEvent &keyEvent)
{
	if (mCurrentInputMode == IM_GUI) {
		// do event injection
		// key down
		for (IInputAdapterStore::reverse_iterator I = mAdapters.rbegin(); I != mAdapters.rend(); ++I) {
			if (!(*I)->injectKeyDown(keyEvent.keysym.sym)) break;
		}
		

		// now character
		if (mNonCharKeys.find(keyEvent.keysym.sym) == mNonCharKeys.end()) {
			for (IInputAdapterStore::reverse_iterator I = mAdapters.rbegin(); I != mAdapters.rend(); ++I) {
				if (!(*I)->injectChar(keyEvent.keysym.unicode)) break;
			}
		}
	}
	EventKeyPressed(keyEvent.keysym, mCurrentInputMode);
	
}




void Input::keyReleased (const SDL_KeyboardEvent &keyEvent)
{
	if (mCurrentInputMode == IM_GUI) {
		for (IInputAdapterStore::reverse_iterator I = mAdapters.rbegin(); I != mAdapters.rend(); ++I) {
			if (!(*I)->injectKeyUp(keyEvent.keysym.sym)) break;
		}
	} 
	EventKeyReleased(keyEvent.keysym, mCurrentInputMode);
}


void Input::setInputMode(InputMode mode)
{
	mCurrentInputMode = mode;
	EventChangedInputMode.emit(mode);
}
	
Input::InputMode Input::getInputMode() const
{
	return mCurrentInputMode;
}
	
	
Input::InputMode Input::toggleInputMode()
{
	if (mCurrentInputMode == IM_GUI)
	{
		setInputMode(IM_MOVEMENT);
		return IM_MOVEMENT;
	} else {
		setInputMode(IM_GUI);
		return IM_GUI;
	}		
		
}

void Input::addAdapter(IInputAdapter* adapter)
{
	mAdapters.push_back(adapter);
}
	
	
void Input::removeAdapter(IInputAdapter* adapter)
{
	for(IInputAdapterStore::iterator I = mAdapters.begin(); I != mAdapters.end(); ++I) {
		if (*I == adapter) {
			mAdapters.erase(I);
			return;
		}
	}
}

};
