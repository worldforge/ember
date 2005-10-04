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
#include <OgreCEGUIRenderer.h>
#include "../EmberOgre.h"

namespace EmberOgre {




Input::Input(CEGUI::System *system, CEGUI::OgreCEGUIRenderer *renderer)
: mGuiSystem(system)
, mGuiRenderer(renderer)
, mCurrentInputMode(IM_GUI)
/*, mMouseMotionListener(0)*/
// , mInGUIMode(true)
// , mInLockedMovementMode(false)
, mTimeSinceLastRightMouseClick(1000)
{
	SDL_ShowCursor(0);
	SDL_EnableUNICODE(1);
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
	
	
	//lookup table for sdl scancodes and CEGUI keys
	mKeyMap[SDLK_BACKSPACE] = CEGUI::Key::Backspace;
	mKeyMap[SDLK_TAB] = CEGUI::Key::Tab;
/*	mKeyMap[SDLK_CLEAR] = CEGUI::Key::Clear;*/
	mKeyMap[SDLK_RETURN] = CEGUI::Key::Return;
	mKeyMap[SDLK_PAUSE] = CEGUI::Key::Pause;
	mKeyMap[SDLK_ESCAPE] = CEGUI::Key::Escape;
	mKeyMap[SDLK_SPACE] = CEGUI::Key::Space;
/*	mKeyMap[SDLK_EXCLAIM] = CEGUI::Key::Exclaim;*/
/*	mKeyMap[SDLK_QUOTEDBL] = CEGUI::Key::;
	mKeyMap[SDLK_HASH] = CEGUI::Key::;
	mKeyMap[SDLK_DOLLAR] = CEGUI::Key::;
	mKeyMap[SDLK_AMPERSAND] = CEGUI::Key::;
	mKeyMap[SDLK_QUOTE] = CEGUI::Key::;
	mKeyMap[SDLK_LEFTPAREN] = CEGUI::Key::;
	mKeyMap[SDLK_RIGHTPAREN] = CEGUI::Key::;
	mKeyMap[SDLK_ASTERISK] = CEGUI::Key::;*/
	mKeyMap[SDLK_PLUS] = CEGUI::Key::Add;
/*	mKeyMap[SDLK_COMMA] = CEGUI::Key::;*/
	mKeyMap[SDLK_MINUS] = CEGUI::Key::Minus;
	mKeyMap[SDLK_PERIOD] = CEGUI::Key::Period;
/*	mKeyMap[SDLK_SLASH] = CEGUI::Key::;*/
	mKeyMap[SDLK_0] = CEGUI::Key::One;
	mKeyMap[SDLK_1] = CEGUI::Key::Two;
	mKeyMap[SDLK_2] = CEGUI::Key::Two;
	mKeyMap[SDLK_3] = CEGUI::Key::Three;
	mKeyMap[SDLK_4] = CEGUI::Key::Four;
	mKeyMap[SDLK_5] = CEGUI::Key::Five;
	mKeyMap[SDLK_6] = CEGUI::Key::Six;
	mKeyMap[SDLK_7] = CEGUI::Key::Seven;
	mKeyMap[SDLK_8] = CEGUI::Key::Eight;
	mKeyMap[SDLK_9] = CEGUI::Key::Nine;
	mKeyMap[SDLK_COLON] = CEGUI::Key::Colon;
	mKeyMap[SDLK_SEMICOLON] = CEGUI::Key::Semicolon;
/*	mKeyMap[SDLK_LESS] = CEGUI::Key::;*/
/*	mKeyMap[SDLK_EQUALS] = CEGUI::Key::;
	mKeyMap[SDLK_GREATER] = CEGUI::Key::;
	mKeyMap[SDLK_QUESTION] = CEGUI::Key::;*/
/*	mKeyMap[SDLK_AT] = CEGUI::Key::;*/
/*	mKeyMap[SDLK_LEFTBRACKET] = CEGUI::Key::;*/
	mKeyMap[SDLK_BACKSLASH] = CEGUI::Key::Backslash;
/*	mKeyMap[SDLK_RIGHTBRACKET] = CEGUI::Key::;*/
/*	mKeyMap[SDLK_CARET] = CEGUI::Key::;
	mKeyMap[SDLK_UNDERSCORE] = CEGUI::Key::;
	mKeyMap[SDLK_BACKQUOTE] = CEGUI::Key::;*/
	mKeyMap[SDLK_a] = CEGUI::Key::A;
	mKeyMap[SDLK_b] = CEGUI::Key::B;
	mKeyMap[SDLK_c] = CEGUI::Key::C;
	mKeyMap[SDLK_d] = CEGUI::Key::D;
	mKeyMap[SDLK_e] = CEGUI::Key::E;
	mKeyMap[SDLK_f] = CEGUI::Key::F;
	mKeyMap[SDLK_g] = CEGUI::Key::G;
	mKeyMap[SDLK_h] = CEGUI::Key::H;
	mKeyMap[SDLK_i] = CEGUI::Key::I;
	mKeyMap[SDLK_j] = CEGUI::Key::J;
	mKeyMap[SDLK_k] = CEGUI::Key::K;
	mKeyMap[SDLK_l] = CEGUI::Key::L;
	mKeyMap[SDLK_m] = CEGUI::Key::M;
	mKeyMap[SDLK_n] = CEGUI::Key::N;
	mKeyMap[SDLK_o] = CEGUI::Key::O;
	mKeyMap[SDLK_p] = CEGUI::Key::P;
	mKeyMap[SDLK_q] = CEGUI::Key::Q;
	mKeyMap[SDLK_r] = CEGUI::Key::R;
	mKeyMap[SDLK_s] = CEGUI::Key::S;
	mKeyMap[SDLK_t] = CEGUI::Key::T;
	mKeyMap[SDLK_u] = CEGUI::Key::U;
	mKeyMap[SDLK_v] = CEGUI::Key::V;
	mKeyMap[SDLK_w] = CEGUI::Key::W;
	mKeyMap[SDLK_x] = CEGUI::Key::X;
	mKeyMap[SDLK_y] = CEGUI::Key::Y;
	mKeyMap[SDLK_z] = CEGUI::Key::Z;
	mKeyMap[SDLK_DELETE] = CEGUI::Key::Delete;
	mKeyMap[SDLK_UP] = CEGUI::Key::ArrowUp;
	mKeyMap[SDLK_DOWN] = CEGUI::Key::ArrowDown;
	mKeyMap[SDLK_RIGHT] = CEGUI::Key::ArrowRight;
	mKeyMap[SDLK_LEFT] = CEGUI::Key::ArrowLeft;
	mKeyMap[SDLK_INSERT] = CEGUI::Key::Insert;
	mKeyMap[SDLK_HOME] = CEGUI::Key::Home;
	mKeyMap[SDLK_END] = CEGUI::Key::End;
	mKeyMap[SDLK_PAGEUP] = CEGUI::Key::PageUp;
	mKeyMap[SDLK_PAGEDOWN] = CEGUI::Key::PageDown;
	mKeyMap[SDLK_F1] = CEGUI::Key::F1;
	mKeyMap[SDLK_F2] = CEGUI::Key::F2;
	mKeyMap[SDLK_F3] = CEGUI::Key::F3;
	mKeyMap[SDLK_F4] = CEGUI::Key::F4;
	mKeyMap[SDLK_F5] = CEGUI::Key::F5;
	mKeyMap[SDLK_F6] = CEGUI::Key::F6;
	mKeyMap[SDLK_F7] = CEGUI::Key::F7;
	mKeyMap[SDLK_F8] = CEGUI::Key::F8;
	mKeyMap[SDLK_F9] = CEGUI::Key::F9;
	mKeyMap[SDLK_F10] = CEGUI::Key::F10;
	mKeyMap[SDLK_F11] = CEGUI::Key::F11;
	mKeyMap[SDLK_F12] = CEGUI::Key::F12;
	
	
	//we don't want to send a injectChar to CEGUI for some keys, put them here
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

void Input::processInput(const Ogre::FrameEvent& evt)
{
	
	pollMouse(evt);
	pollKeyboard(evt);
    SDL_PumpEvents();
}

// inline bool Input::getIsInLockedMovementMode()
// {
// 	return mInLockedMovementMode;
// 
// }
// 
// inline void Input::setIsInLockedMovementMode(bool value)
// {
// 	mInLockedMovementMode = value;
// }


void Input::pollMouse(const Ogre::FrameEvent& evt)
{
	int mouseX, mouseY;
	unsigned int mouseState;
	
	mouseState = SDL_GetMouseState( &mouseX, &mouseY );
	
	mTimeSinceLastRightMouseClick += evt.timeSinceLastFrame;
	if (mouseState & SDL_BUTTON_RMASK) {
		if (!(mMouseState & SDL_BUTTON_RMASK)) {
			//right mouse button pressed
			
			//if the right mouse button is pressed, switch from gui mode
			
			//SDL_WM_GrabInput(SDL_GRAB_ON);
			toggleInputMode();
			EventMouseButtonPressed.emit(MouseButtonRight, mCurrentInputMode);
			
		}
	} else if (mMouseState & SDL_BUTTON_RMASK) {
		//right mouse button released
		//SDL_WM_GrabInput(SDL_GRAB_ON);
		
		//if there's two right mouse clicks withing 0.25 seconds from each others, it's a double click
		if (mTimeSinceLastRightMouseClick < 0.25) {
/*			std::stringstream ss;
			ss << "mouse: " << mTimeSinceLastRightMouseClick << "\n";
			fprintf(stderr, ss.str().c_str());*/
			toggleInputMode();
//			setIsInLockedMovementMode(!getIsInLockedMovementMode());
			
		}
		mTimeSinceLastRightMouseClick = 0;
		toggleInputMode();
		EventMouseButtonReleased.emit(MouseButtonRight, mCurrentInputMode);
	} 
	
	//if in locked movement mode, override the mInGUIMode setting
/*	if (getIsInLockedMovementMode()) {
		mInGUIMode = false;
	}*/
	
	
	
	if (mouseState & SDL_BUTTON_LMASK) {
		if (!(mMouseState & SDL_BUTTON_LMASK)) {
			//left mouse button pressed
			mGuiSystem->injectMouseButtonDown(CEGUI::LeftButton);
			EventMouseButtonPressed.emit(MouseButtonLeft, mCurrentInputMode);
		}
	} else if (mMouseState & SDL_BUTTON_LMASK) {
		//left mouse button released
		mGuiSystem->injectMouseButtonUp(CEGUI::LeftButton);
		EventMouseButtonReleased.emit(MouseButtonLeft, mCurrentInputMode);

	}
	
	if (mouseState & SDL_BUTTON_MMASK) {
		if (!(mMouseState & SDL_BUTTON_MMASK)) {
			//middle mouse button pressed
			mGuiSystem->injectMouseButtonDown(CEGUI::MiddleButton);
			EventMouseButtonPressed.emit(MouseButtonMiddle, mCurrentInputMode);
		}
	} else if (mMouseState & SDL_BUTTON_MMASK) {
		//middle mouse button released
		mGuiSystem->injectMouseButtonUp(CEGUI::MiddleButton);
		EventMouseButtonReleased.emit(MouseButtonMiddle, mCurrentInputMode);
	}
	
	mMouseState = mouseState;
	
	
	
	//has the mouse moved?
	if (mMouseX != mouseX || mMouseY != mouseY)
	{

		//we'll calculate the mouse movement difference and send the values to those
		//listening to the MouseMoved event
		Ogre::Real diffX, diffY;
		Ogre::Real width = mGuiRenderer->getWidth();
		Ogre::Real height = mGuiRenderer->getHeight();
		diffX =  (mMouseX - mouseX) / width;
		diffY = (mMouseY - mouseY) / height;
		MouseMotion motion;
		motion.xPosition = mouseX;
		motion.yPosition = mouseY;
		motion.xRelativeMovement = diffX;
		motion.yRelativeMovement = diffY;
		motion.xRelativeMovementInPixels = mMouseX - mouseX;
		motion.yRelativeMovementInPixels = mMouseY - mouseY;
		motion.timeSinceLastMovement = evt.timeSinceLastFrame;
		
		EventMouseMoved.emit(motion, mCurrentInputMode);
		
		//if we're in gui mode, we'll just send the mouse movement on to CEGUI
		if (mCurrentInputMode == IM_GUI) {
			
			CEGUI::MouseCursor::getSingleton().setPosition(CEGUI::Point((mouseX), (mouseY))); 
			mGuiSystem->injectMouseMove(0.0f, 0.0f);
			
		} else {
			//keep the cursor in place while in non-gui mode
			mouseX = mMouseX;
			mouseY = mMouseY;
			SDL_WarpMouse(mMouseX, mMouseY);
		}
	
	}
	mMouseX = mouseX;
	mMouseY = mouseY;

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
				EmberOgre::getSingleton().EventRequestQuit.emit();
				break;
			}
			
			
        }
    
/*	
	
	SDL_Event events[16];

	int count = SDL_PeepEvents(events, 16, SDL_GETEVENT,
			(SDL_KEYDOWNMASK | SDL_KEYUPMASK));

	for (int i = 0; i < count; i++)
	{
		keyChanged(events[i].key);
	}*/
     
}

void Input::keyChanged(const SDL_KeyboardEvent &keyEvent)
{
	if (keyEvent.type == SDL_KEYDOWN) {
		mKeysPressed.insert(keyEvent.keysym.sym);
		keyPressed(keyEvent);
	} else {
		mKeysPressed.erase(keyEvent.keysym.sym);
		keyReleased(keyEvent);
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
		mGuiSystem->injectKeyDown(mKeyMap[keyEvent.keysym.sym]);
	
		// now character
		if (mNonCharKeys.find(keyEvent.keysym.sym) == mNonCharKeys.end()) {
			mGuiSystem->injectChar(keyEvent.keysym.unicode);
		}
	
	}
	EventKeyPressed(keyEvent.keysym, mCurrentInputMode);
	
}




void Input::keyReleased (const SDL_KeyboardEvent &keyEvent)
{

//NOTE: We have to change the way KeyReleased and KeyPressed works.
//they should always be emitted, with a flag which tells if we're in gui mode or not
	if (mCurrentInputMode == IM_GUI) {
		//toggle the console
		//we've put it here because we wan't the console to always be available
/*		if(keyEvent.keysym.sym == SDLK_F12)
		{
			mConsoleWidget->toggleActive();
		}

		//take screenshot		
		if(keyEvent.keysym.sym == SDLK_F8)
		{
			setDebugText("Wrote image: " +takeScreenshot());
		}
		
		//switch render mode
		if(keyEvent.keysym.sym == SDLK_F7)
		{
			setDebugText("Switching rendermode.");
			Ogre::Camera* ogreCamera = EmberOgre::getSingleton().getMainCamera()->getCamera();
			if (ogreCamera->getDetailLevel() == Ogre::SDL_SOLID) {
				ogreCamera->setDetailLevel(Ogre::SDL_WIREFRAME);
			} else {
				ogreCamera->setDetailLevel(Ogre::SDL_SOLID);
			}
		}*/
		
		//switch between full screen
/*		if(e->getKey() == Ogre::KC_F6)
		{
			mWindow->
		}*/
		
		
		mGuiSystem->injectKeyUp(mKeyMap[keyEvent.keysym.sym]);
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

// Input::pollEvents()
// {
// 	int mouseX, mouseY;
// 	
// 	SDL_GetMouseState( &mouseX, &mouseY );
// 	if (mMouseX != mouseX || mMouseY != mouseY)
// 	{
// 	
// 		if (mInGUIMode) {
// 			
// 			CEGUI::MouseCursor::getSingleton().setPosition(CEGUI::Point((mouseX), (mouseY))); 
// 			mGuiSystem->injectMouseMove(0.0f, 0.0f);
// 			
// 			
// /*			std::stringstream ss;
// 	//		ss << (e->getX() * mGuiRenderer->getWidth()) << ":" << e->getY() * mGuiRenderer->getHeight();
// 			ss << (e->getX() * mGuiRenderer->getWidth()) << " ( " << e->getX() << " ) :" << (e->getY() * mGuiRenderer->getHeight()) << " ( " << e->getY() << " )";
// 			setDebugText(ss.str());*/
// 			
// 			//mGuiSystem->injectMouseMove(e->getRelX() * mGuiRenderer->getWidth(), e->getRelY() * mGuiRenderer->getHeight());
// 		} else {
// 			Ogre::Real diffX, diffY;
// 			int width = mGuiRenderer->getWidth();
// 			int height = mGuiRenderer->getHeight();
// 			diffX = ( width / mMouseX) - (width / mouseX);
// 			diffY = ( height / mMouseY) - (height / mouseY);
// 			Ogre::MouseEvent e = new Ogre::MouseEvent(
// 			if (mMouseMotionListener) {
// 				mMouseMotionListener->mouseMoved(e);
// 			}
// 		}
// 	
// 	}
// 
// }

};
