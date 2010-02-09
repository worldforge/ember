//
// C++ Implementation: Input
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2005
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "Input.h"
// #include "../EmberOgre.h"
#include "InputCommandMapper.h"

#include "main/Application.h"
#ifndef WITHOUT_SCRAP
#include "framework/scrap.h"
#endif // WITHOUT_SCRAP
#include "IInputAdapter.h"

#include "framework/Tokeniser.h"
#include "framework/ConsoleBackend.h"
#include "framework/LoggingInstance.h"

template<> Ember::Input* Ember::Singleton<Ember::Input>::ms_Singleton = 0;

namespace Ember
{

const std::string Input::BINDCOMMAND("bind");
const std::string Input::UNBINDCOMMAND("unbind");

Input::Input() :
	mCurrentInputMode(IM_GUI), mMouseState(0), mTimeSinceLastRightMouseClick(0), mSuppressForCurrentEvent(false), mMovementModeEnabled(false)
{
	mMousePosition.xPixelPosition = 0;
	mMousePosition.yPixelPosition = 0;
	mMousePosition.xRelativePosition = 0.0f;
	mMousePosition.yRelativePosition = 0.0f;

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

	mLastTick = SDL_GetTicks();

}

Input::~Input()
{

}

void Input::initialize(int width, int height)
{
	SDL_ShowCursor(0);
	SDL_EnableUNICODE(1);
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

	///must initialize the clipboard support
#ifndef WITHOUT_SCRAP
	if (init_scrap() < 0) {
		S_LOG_FAILURE("Couldn't init clipboard: \n" << SDL_GetError());
	}
#endif // WITHOUT_SCRAP
	Ember::ConsoleBackend::getSingletonPtr()->registerCommand(BINDCOMMAND, this);
	Ember::ConsoleBackend::getSingletonPtr()->registerCommand(UNBINDCOMMAND, this);

	setGeometry(width, height);

}

void Input::setGeometry(int width, int height)
{
	mScreenWidth = width;
	mScreenHeight = height;
}

void Input::runCommand(const std::string &command, const std::string &args)
{
	if (command == BINDCOMMAND) {
		Ember::Tokeniser tokeniser;
		tokeniser.initTokens(args);
		std::string state("general");
		std::string key = tokeniser.nextToken();
		if (key != "") {
			std::string command(tokeniser.nextToken());
			if (command != "") {
				if (tokeniser.nextToken() != "") {
					state = tokeniser.nextToken();
				}
				InputCommandMapperStore::iterator I = mInputCommandMappers.find(state);
				if (I != mInputCommandMappers.end()) {
					I->second->bindCommand(key, command);
				}
			}
		}
	} else if (command == UNBINDCOMMAND) {
		Ember::Tokeniser tokeniser;
		tokeniser.initTokens(args);
		std::string state("general");
		std::string key(tokeniser.nextToken());
		if (key != "") {
			if (tokeniser.nextToken() != "") {
				state = tokeniser.nextToken();
			}
			InputCommandMapperStore::iterator I = mInputCommandMappers.find(state);
			if (I != mInputCommandMappers.end()) {
				I->second->unbindCommand(key);
			}
		}
	}
}

void Input::suppressFurtherHandlingOfCurrentEvent()
{
	mSuppressForCurrentEvent = true;
}

bool Input::getMovementModeEnabled() const
{
	return mMovementModeEnabled;
}

void Input::setMovementModeEnabled(bool value)
{
	mMovementModeEnabled = value;
}

void Input::writeToClipboard(const std::string& text)
{
#ifndef WITHOUT_SCRAP
	char basicString[text.length() + 1];
	strcpy(basicString, text.c_str());
	put_scrap(T('T','E','X','T'), text.length(), basicString);
#endif // WITHOUT_SCRAP
}

void Input::registerCommandMapper(InputCommandMapper* mapper)
{
	mInputCommandMappers[mapper->getState()] = mapper;
}

void Input::deregisterCommandMapper(InputCommandMapper* mapper)
{
	mInputCommandMappers.erase(mInputCommandMappers.find(mapper->getState()));
}

bool Input::isApplicationVisible()
{
	return SDL_GetAppState() & SDL_APPACTIVE;
}

void Input::processInput()
{
	uint32_t ticks = SDL_GetTicks();
	float secondsSinceLast = (ticks - mLastTick) * 1000.0f;
	mLastTick = ticks;
	pollMouse(secondsSinceLast);
	pollEvents(secondsSinceLast);
}

void Input::pollMouse(float secondsSinceLast)
{
	int mouseX, mouseY;
	mMouseState = SDL_GetMouseState(&mouseX, &mouseY);

	//has the mouse moved?
	Uint8 appState = SDL_GetAppState();
	//	S_LOG_INFO((appState & SDL_APPMOUSEFOCUS));
	if (appState & SDL_APPMOUSEFOCUS) {
		if (mMousePosition.xPixelPosition != mouseX || mMousePosition.yPixelPosition != mouseY) {

			//we'll calculate the mouse movement difference and send the values to those
			//listening to the MouseMoved event
			float diffX, diffY;
			diffX = (mMousePosition.xPixelPosition - mouseX) / mScreenWidth;
			diffY = (mMousePosition.yPixelPosition - mouseY) / mScreenHeight;
			MouseMotion motion;
			motion.xPosition = mouseX;
			motion.yPosition = mouseY;
			motion.xRelativeMovement = diffX;
			motion.yRelativeMovement = diffY;
			motion.xRelativeMovementInPixels = mMousePosition.xPixelPosition - mouseX;
			motion.yRelativeMovementInPixels = mMousePosition.yPixelPosition - mouseY;
			motion.timeSinceLastMovement = secondsSinceLast;

			EventMouseMoved.emit(motion, mCurrentInputMode);

			bool freezeMouse = false;
			//if we're in gui mode, we'll just send the mouse movement on to CEGUI
			if (mCurrentInputMode == IM_GUI) {

				for (IInputAdapterStore::reverse_iterator I = mAdapters.rbegin(); I != mAdapters.rend(); ++I) {
					if (!(*I)->injectMouseMove(motion, freezeMouse))
						break;
				}

			} else {
				freezeMouse = true;
			}

			if (freezeMouse) {
				SDL_WarpMouse(mMousePosition.xPixelPosition, mMousePosition.yPixelPosition);
			} else {
				mMousePosition.xPixelPosition = mouseX;
				mMousePosition.yPixelPosition = mouseY;
				mMousePosition.xRelativePosition = mouseX / mScreenWidth;
				mMousePosition.yRelativePosition = mouseY / mScreenHeight;
			}

		}
	}

}

void Input::pollEvents(float secondsSinceLast)
{
	mTimeSinceLastRightMouseClick += secondsSinceLast;
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		/* Look for a keypress */
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			keyChanged(event.key);
			break;
		case SDL_QUIT:
			Ember::Application::getSingleton().requestQuit();
			break;
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			if (event.button.state == SDL_RELEASED) {
				if (event.button.button == SDL_BUTTON_RIGHT) {
					//right mouse button released

					//if there's two right mouse clicks withing 0.25 seconds from each others, it's a double click
					/*		if (mTimeSinceLastRightMouseClick < 0.25) {
					 toggleInputMode();

					 }*/
					mTimeSinceLastRightMouseClick = 0.0f;
					//toggleInputMode();
					EventMouseButtonReleased.emit(MouseButtonRight, mCurrentInputMode);

				} else if (event.button.button == SDL_BUTTON_LEFT) {
					//left mouse button released
					for (IInputAdapterStore::reverse_iterator I = mAdapters.rbegin(); I != mAdapters.rend(); ++I) {
						if (!(*I)->injectMouseButtonUp(Input::MouseButtonLeft))
							break;
					}

					EventMouseButtonReleased.emit(MouseButtonLeft, mCurrentInputMode);

				} else if (event.button.button == SDL_BUTTON_MIDDLE) {
					//middle mouse button released
					for (IInputAdapterStore::reverse_iterator I = mAdapters.rbegin(); I != mAdapters.rend(); ++I) {
						if (!(*I)->injectMouseButtonUp(Input::MouseButtonMiddle))
							break;
					}

					EventMouseButtonReleased.emit(MouseButtonMiddle, mCurrentInputMode);
				}
			} else {

				if (event.button.button == SDL_BUTTON_RIGHT) {
					//right mouse button pressed

					//if the right mouse button is pressed, switch from gui mode

					if (mMovementModeEnabled) {
						toggleInputMode();
					}
					EventMouseButtonPressed.emit(MouseButtonRight, mCurrentInputMode);

				} else if (event.button.button == SDL_BUTTON_LEFT) {
					//left mouse button pressed

					for (IInputAdapterStore::reverse_iterator I = mAdapters.rbegin(); I != mAdapters.rend(); ++I) {
						if (*I) {
							if (!(*I)->injectMouseButtonDown(Input::MouseButtonLeft))
								break;
						}
					}

					EventMouseButtonPressed.emit(MouseButtonLeft, mCurrentInputMode);
				} else if (event.button.button == SDL_BUTTON_MIDDLE) {
					//middle mouse button pressed
					for (IInputAdapterStore::reverse_iterator I = mAdapters.rbegin(); I != mAdapters.rend(); ++I) {
						if (!(*I)->injectMouseButtonDown(Input::MouseButtonMiddle))
							break;
					}

					EventMouseButtonPressed.emit(MouseButtonMiddle, mCurrentInputMode);

				} else if (event.button.button == SDL_BUTTON_WHEELUP) {
					EventMouseButtonPressed.emit(MouseWheelUp, mCurrentInputMode);
					for (IInputAdapterStore::reverse_iterator I = mAdapters.rbegin(); I != mAdapters.rend(); ++I) {
						if (*I) {
							if (!(*I)->injectMouseButtonDown(Input::MouseWheelUp))
								break;
						}
					}
				} else if (event.button.button == SDL_BUTTON_WHEELDOWN) {
					EventMouseButtonPressed.emit(MouseWheelDown, mCurrentInputMode);
					for (IInputAdapterStore::reverse_iterator I = mAdapters.rbegin(); I != mAdapters.rend(); ++I) {
						if (*I) {
							if (!(*I)->injectMouseButtonDown(Input::MouseWheelDown))
								break;
						}
					}
				}
			}
			break;
		case SDL_ACTIVEEVENT:
			if (event.active.state & SDL_APPACTIVE) {
				EventWindowActive.emit(event.active.gain);
			}
			break;
		}
	}
}

void Input::pasteFromClipboard()
{
#ifndef WITHOUT_SCRAP
	static char *scrap = 0;
	int scraplen;

	get_scrap(T('T','E','X','T'), &scraplen, &scrap);
	for (int i = 0; i < scraplen; ++i) {
		for (IInputAdapterStore::reverse_iterator I = mAdapters.rbegin(); I != mAdapters.rend(); ++I) {
			if (!(*I)->injectChar(scrap[i]))
				break;
		}
	}
#endif // WITHOUT_SCRAP
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

void Input::keyPressed(const SDL_KeyboardEvent &keyEvent)
{
	mSuppressForCurrentEvent = false;
	if (mCurrentInputMode == IM_GUI) {
		// do event injection
		// key down
		for (IInputAdapterStore::reverse_iterator I = mAdapters.rbegin(); I != mAdapters.rend() && !mSuppressForCurrentEvent; ++I) {
			if (!(*I)->injectKeyDown(keyEvent.keysym.sym))
				break;
		}

		// now character
		if (mNonCharKeys.find(keyEvent.keysym.sym) == mNonCharKeys.end()) {
			for (IInputAdapterStore::reverse_iterator I = mAdapters.rbegin(); I != mAdapters.rend() && !mSuppressForCurrentEvent; ++I) {
				if (!(*I)->injectChar(keyEvent.keysym.unicode))
					break;
			}
		}
	}
	if (!mSuppressForCurrentEvent) {
		EventKeyPressed(keyEvent.keysym, mCurrentInputMode);
	}
	mSuppressForCurrentEvent = false;

}

void Input::keyReleased(const SDL_KeyboardEvent &keyEvent)
{
	mSuppressForCurrentEvent = false;
	if (mCurrentInputMode == IM_GUI) {
		for (IInputAdapterStore::reverse_iterator I = mAdapters.rbegin(); I != mAdapters.rend() && !mSuppressForCurrentEvent; ++I) {
			if (!(*I)->injectKeyUp(keyEvent.keysym.sym))
				break;
		}
	}
	if (!mSuppressForCurrentEvent) {
		EventKeyReleased(keyEvent.keysym, mCurrentInputMode);
	}
	mSuppressForCurrentEvent = false;
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
	if (mCurrentInputMode == IM_GUI) {
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
	for (IInputAdapterStore::iterator I = mAdapters.begin(); I != mAdapters.end(); ++I) {
		if (*I == adapter) {
			mAdapters.erase(I);
			return;
		}
	}
}

const MousePosition& Input::getMousePosition() const
{
	return mMousePosition;
}

void Input::sleep(unsigned int milliseconds)
{
	SDL_Delay(milliseconds);
}


}
;
