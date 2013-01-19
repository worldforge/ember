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

#ifdef _WIN32
//Necessary to tell the mouse events to go to this window
#if (_WIN32_WINNT < 0x0501)
#undef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif
// Necessary to get the Window Handle of the window
// OGRE created, so SDL can grab its input.
#include "windows.h"
#include "SDL_getenv.h"
#endif

#include "Input.h"
#include "IWindowProvider.h"
#include "InputCommandMapper.h"
#include "EmberIcon.h"

#include "IInputAdapter.h"

#include "services/config/ConfigListenerContainer.h"

#include "framework/Tokeniser.h"
#include "framework/ConsoleBackend.h"
#include "framework/LoggingInstance.h"
#include "framework/MainLoopController.h"

#ifdef _WIN32
#include "platform/platform_windows.h"
#endif

#ifndef WITHOUT_SCRAP
#include "framework/scrap.h"
#endif // WITHOUT_SCRAP
#ifdef _MSC_VER
#include <SDL.h>
#include <SDL_syswm.h>
#include <SDL_keyboard.h>
#else
#include <SDL/SDL.h>
#include <SDL/SDL_syswm.h>
#include <SDL/SDL_keyboard.h>
#endif

#ifndef __APPLE__
#ifndef _WIN32
#include <X11/Xlib.h>
#endif
#endif

#include <sstream>

template<> Ember::Input* Ember::Singleton<Ember::Input>::ms_Singleton = 0;

namespace Ember
{

const std::string Input::BINDCOMMAND("bind");
const std::string Input::UNBINDCOMMAND("unbind");

Input::Input() :
		ToggleFullscreen(0), mCurrentInputMode(IM_GUI), mMouseState(0), mTimeSinceLastRightMouseClick(0), mSuppressForCurrentEvent(false), mMovementModeEnabled(false), mConfigListenerContainer(new ConfigListenerContainer()), mMouseGrabbingRequested(false), mMouseGrab(false), mMainLoopController(0), mWindowProvider(NULL), mScreenWidth(0), mScreenHeight(0), mIconSurface(0), mMainVideoSurface(0), mInvertMouse(1), mHandleOpenGL(false)
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

	//If building WebEmber on Linux we shouldn't initialize SDL here, as that's done separately in another thread.
#if !defined(BUILD_WEBEMBER) || defined(_WIN32) || defined(__APPLE__)
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE);
#endif
	mLastTick = SDL_GetTicks();

	//this is a failsafe which guarantees that SDL is correctly shut down (returning the screen to correct resolution, releasing mouse etc.) if there's a crash.
	atexit(SDL_Quit);

}

Input::~Input()
{
	delete mConfigListenerContainer;
	shutdownInteraction();
}

std::string Input::createWindow(unsigned int width, unsigned int height, bool fullscreen, bool resizable, bool centered, bool handleOpenGL)
{
	mHandleOpenGL = handleOpenGL;
	int flags = SDL_HWSURFACE;

	if (resizable) {
		flags |= SDL_RESIZABLE;
	}

	if (mHandleOpenGL) {
		flags |= SDL_OPENGL;
	}

	if (fullscreen) {
		flags |= SDL_FULLSCREEN;
	}

	//On Windows we'll use another method to center the window once it has been created.
	//In addition, if the environment variable is set on windows, it will never be cleared.
	//As a result, every time the window is resized it will be adjusted to be centered.
#ifndef _WIN32
	if (centered) {
		//Some versions of SDL_putenv accepts only a non-const char array.
		char centered[] = "SDL_VIDEO_CENTERED=center";
		SDL_putenv(centered);
	}
#endif

	mMainVideoSurface = SDL_SetVideoMode(width, height, 0, flags); // create an SDL window

	SDL_SysWMinfo info;
	SDL_VERSION(&info.version);

	SDL_GetWMInfo(&info);
	
#ifdef _WIN32
	if (centered) {
		HWND hwnd = info.window;
		HMONITOR hMonitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
		if (hMonitor) {

			MONITORINFO mon;
			memset(&mon, 0, sizeof(MONITORINFO));
			mon.cbSize = sizeof(MONITORINFO);
			GetMonitorInfo(hMonitor, &mon);

			RECT r;
			GetWindowRect(hwnd, &r);

			int winWidth = r.right - r.left;
			int winLeft = mon.rcWork.left + (mon.rcWork.right - mon.rcWork.left) / 2 - winWidth/2;

			int winHeight = r.bottom - r.top;
			int winTop = mon.rcWork.top + (mon.rcWork.bottom - mon.rcWork.top) / 2 - winHeight/2;
			winTop = std::max(winTop, 0);

			SetWindowPos(hwnd, 0, winLeft, winTop, 0, 0, SWP_NOSIZE);
		}
	}
#endif	
	
	SDL_WM_SetCaption("Ember", "ember");

	SDL_ShowCursor(0);
	SDL_EnableUNICODE(1);
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

	std::stringstream ss;
#ifdef __APPLE__
	//On OSX we'll tell Ogre to use the current OpenGL context; thus we don't need to return the window id
#elif _WIN32
	ss << (size_t)info.window;
#else
	ss << info.info.x11.window;
#endif

	//must initialize the clipboard support
#ifndef WITHOUT_SCRAP
	if (init_scrap() < 0) {
		S_LOG_FAILURE("Couldn't init clipboard: \n" << SDL_GetError());
	}
#endif // WITHOUT_SCRAP
	ConsoleBackend& console = ConsoleBackend::getSingleton();
	console.registerCommand(BINDCOMMAND, this);
	console.registerCommand(UNBINDCOMMAND, this);

	setGeometry(width, height);
	createIcon();

	if (!ToggleFullscreen) {
		ToggleFullscreen = new ConsoleCommandWrapper("toggle_fullscreen", this, "Switch between windowed and full screen mode.");
	}

	return ss.str();
}

void Input::shutdownInteraction()
{
	mWindowProvider = 0;

	if (mIconSurface) {
		SDL_FreeSurface(mIconSurface);
		mIconSurface = 0;
	}

	// according to http://www.libsdl.org/cgi/docwiki.cgi/SDL_SetVideoMode
	// never free the surface returned from SDL_SetVideoMode
	mMainVideoSurface = 0;

	//Release the mouse for safety's sake.
	SDL_WM_GrabInput(SDL_GRAB_OFF);
	S_LOG_INFO("Shutting down SDL.");
	SDL_Quit();
	S_LOG_INFO("SDL shut down.");

	ConsoleBackend& console = ConsoleBackend::getSingleton();
	console.deregisterCommand(BINDCOMMAND);
	console.deregisterCommand(UNBINDCOMMAND);
	delete ToggleFullscreen;
}

void Input::createIcon()
{
#ifndef BUILD_WEBEMBER
	//set the icon of the window
	Uint32 rmask, gmask, bmask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
#else
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
#endif

	//We'll use the emberIcon struct. This isn't needed on WIN32 or OSX as the icon is provided through different means.
	mIconSurface = SDL_CreateRGBSurfaceFrom(emberIcon.pixel_data, 64, 64, 24, 64 * 3, rmask, gmask, bmask, 0);
	if (mIconSurface) {
		SDL_WM_SetIcon(mIconSurface, 0);
	}

#endif // !BUILD_WEBEMBER
}

void Input::attach(IWindowProvider* windowProvider)
{

	//The windowProvider should not be NULL.
	assert(windowProvider);
	mWindowProvider = windowProvider;

	char tmp[64];
	// Set the SDL_WINDOWID environment variable
	sprintf(tmp, "SDL_WINDOWID=%s", mWindowProvider->getWindowHandle().c_str());
	putenv(tmp);

	createIcon();
	unsigned int width, height;
	mWindowProvider->getWindowSize(width, height);
	setGeometry(width, height);

	SDL_ShowCursor(0);
	SDL_EnableUNICODE(1);
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

#ifdef _WIN32
	if (width && height)
	{
		// if width = 0 and height = 0, the window is fullscreen

		// This is necessary to allow the window to move
		//  on WIN32 systems. Without this, the window resets
		//  to the smallest possible size after moving.
		SDL_SetVideoMode(width, height, 0, 0);// first 0: BitPerPixel,
											  // second 0: flags (fullscreen/...)
											  // neither are needed as Ogre sets these
	}

	static SDL_SysWMinfo pInfo;
	SDL_VERSION(&pInfo.version);
	SDL_GetWMInfo(&pInfo);

	// Also, SDL keeps an internal record of the window size
	//  and position. Because SDL does not own the window, it
	//  missed the WM_WINDOWPOSCHANGED message and has no record of
	//  either size or position. It defaults to {0, 0, 0, 0},
	//  which is then used to trap the mouse "inside the
	//  window". We have to fake a window-move to allow SDL
	//  to catch up, after which we can safely grab input.
	//  Note that the WM_WINDOWPOSCHANGED seems only to be sent if the
	//  position of the window actually changed. Thus we have to first move
	//  it one pixel to the right, and then back again.
	RECT r;
	GetWindowRect(pInfo.window, &r);
	SetWindowPos(pInfo.window, 0, r.left + 1, r.top, 0, 0, SWP_NOSIZE);
	SetWindowPos(pInfo.window, 0, r.left, r.top, 0, 0, SWP_NOSIZE);

	RAWINPUTDEVICE Rid;
	/* we're telling the window, we want it to report raw input events from mice (needed for SDL-1.3) */
	Rid.usUsagePage = 0x01;
	Rid.usUsage = 0x02;
	Rid.dwFlags = RIDEV_INPUTSINK;
	Rid.hwndTarget = (HWND)pInfo.window;
	RegisterRawInputDevices(&Rid, 1, sizeof(Rid));
#endif

#ifndef __APPLE__
#ifndef _WIN32

	//The SDL_WINDOWID hack will result in no X events being caught on X11; we need to inform X11 that we want them anyway.
	SDL_SysWMinfo info;
	XSetWindowAttributes attributes;

	SDL_VERSION(&info.version);
	// this is important!
	SDL_GetWMInfo(&info);

	info.info.x11.lock_func();

	attributes.event_mask = KeyPressMask | KeyReleaseMask | PointerMotionMask | ButtonPressMask | ButtonReleaseMask | EnterWindowMask | LeaveWindowMask;
	XChangeWindowAttributes(info.info.x11.display, info.info.x11.window, CWEventMask, &attributes);
	info.info.x11.unlock_func();

#endif
#endif
	//must initialize the clipboard support
#ifndef WITHOUT_SCRAP
	if (init_scrap() < 0) {
		S_LOG_FAILURE("Couldn't init clipboard: \n" << SDL_GetError());
	}
#endif // WITHOUT_SCRAP
	ConsoleBackend& console = ConsoleBackend::getSingleton();
	console.registerCommand(BINDCOMMAND, this);
	console.registerCommand(UNBINDCOMMAND, this);

}

void Input::setGeometry(int width, int height)
{
	mScreenWidth = width;
	mScreenHeight = height;
	if (mMainVideoSurface->h != height || mMainVideoSurface->w != width) {
		mMainVideoSurface = SDL_SetVideoMode(width, height, mMainVideoSurface->format->BitsPerPixel, mMainVideoSurface->flags);
		EventSizeChanged.emit(width, height);
	}
}

void Input::runCommand(const std::string &command, const std::string &args)
{
	if (command == BINDCOMMAND) {
		Tokeniser tokeniser;
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
		Tokeniser tokeniser;
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
	} else if (ToggleFullscreen && *ToggleFullscreen == command) {
		setFullscreen((mMainVideoSurface->flags & SDL_FULLSCREEN) == 0);
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
	if (mWindowProvider) {
		return mWindowProvider->isWindowVisible();
	}
	return SDL_GetAppState() & SDL_APPACTIVE;
}

bool Input::isApplicationFocused()
{
	return SDL_GetAppState() & SDL_APPMOUSEFOCUS;
}

void Input::startInteraction()
{
	mConfigListenerContainer->registerConfigListenerWithDefaults("input", "catchmouse", sigc::mem_fun(*this, &Input::Config_CatchMouse), true);
	mConfigListenerContainer->registerConfigListenerWithDefaults("input", "invertcamera", sigc::mem_fun(*this, &Input::Config_InvertCamera), true);
}

void Input::processInput()
{
	uint32_t ticks = SDL_GetTicks();
	float secondsSinceLast = (ticks - mLastTick) * 1000.0f;
	mLastTick = ticks;
	pollMouse(secondsSinceLast);
	pollEvents(secondsSinceLast);
	if (mWindowProvider) {
		mWindowProvider->processInput();
	}
	SDL_GL_SwapBuffers();
}

void Input::pollMouse(float secondsSinceLast)
{
	int mouseX, mouseY;
	mMouseState = SDL_GetMouseState(&mouseX, &mouseY);

	//has the mouse moved?
	Uint8 appState = SDL_GetAppState();
	if (appState & SDL_APPMOUSEFOCUS) {
		//Wait with grabbing the mouse until the app has input focus.
		if (mMouseGrabbingRequested && (appState & SDL_APPINPUTFOCUS)) {
			setMouseGrab(true);
		}
		if (mMousePosition.xPixelPosition != mouseX || mMousePosition.yPixelPosition != mouseY) {

			//we'll calculate the mouse movement difference and send the values to those
			//listening to the MouseMoved event
			float diffX, diffY;
			diffX = (mMousePosition.xPixelPosition - mouseX) / mScreenWidth;
			diffY = (mMousePosition.yPixelPosition - mouseY) / mScreenHeight;
			MouseMotion motion;
			motion.xPosition = mouseX;
			motion.yPosition = mouseY;
			motion.xRelativeMovement = diffX * mInvertMouse;
			motion.yRelativeMovement = diffY * mInvertMouse;
			motion.xRelativeMovementInPixels = (mMousePosition.xPixelPosition - mouseX) * mInvertMouse;
			motion.yRelativeMovementInPixels = (mMousePosition.yPixelPosition - mouseY) * mInvertMouse;
			motion.timeSinceLastMovement = secondsSinceLast;

			EventMouseMoved.emit(motion, mCurrentInputMode);

			bool freezeMouse = false;
			//if we're in gui mode, we'll just send the mouse movement on to CEGUI
			if (mCurrentInputMode == IM_GUI) {

				for (IInputAdapterStore::const_iterator I = mAdapters.begin(); I != mAdapters.end();) {
					IInputAdapter* adapter = *I;
					++I;
					if (!(adapter)->injectMouseMove(motion, freezeMouse))
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
	static SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		/* Look for a keypress */
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			keyChanged(event.key);
			break;
		case SDL_QUIT:
			if (mMainLoopController) {
				mMainLoopController->requestQuit();
			}
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
					for (IInputAdapterStore::const_iterator I = mAdapters.begin(); I != mAdapters.end();) {
						IInputAdapter* adapter = *I;
						++I;
						if (!(adapter)->injectMouseButtonUp(Input::MouseButtonLeft))
							break;
					}

					EventMouseButtonReleased.emit(MouseButtonLeft, mCurrentInputMode);

				} else if (event.button.button == SDL_BUTTON_MIDDLE) {
					//middle mouse button released
					for (IInputAdapterStore::const_iterator I = mAdapters.begin(); I != mAdapters.end();) {
						IInputAdapter* adapter = *I;
						++I;
						if (!(adapter)->injectMouseButtonUp(Input::MouseButtonMiddle))
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

					for (IInputAdapterStore::const_iterator I = mAdapters.begin(); I != mAdapters.end();) {
						IInputAdapter* adapter = *I;
						++I;
						if (!(adapter)->injectMouseButtonDown(Input::MouseButtonLeft))
							break;
					}

					EventMouseButtonPressed.emit(MouseButtonLeft, mCurrentInputMode);
				} else if (event.button.button == SDL_BUTTON_MIDDLE) {
					//middle mouse button pressed
					for (IInputAdapterStore::const_iterator I = mAdapters.begin(); I != mAdapters.end();) {
						IInputAdapter* adapter = *I;
						++I;
						if (!(adapter)->injectMouseButtonDown(Input::MouseButtonMiddle))
							break;
					}

					EventMouseButtonPressed.emit(MouseButtonMiddle, mCurrentInputMode);

				} else if (event.button.button == SDL_BUTTON_WHEELUP) {
					EventMouseButtonPressed.emit(MouseWheelUp, mCurrentInputMode);
					for (IInputAdapterStore::const_iterator I = mAdapters.begin(); I != mAdapters.end();) {
						IInputAdapter* adapter = *I;
						++I;
						if (!(adapter)->injectMouseButtonDown(Input::MouseWheelUp))
							break;
					}
				} else if (event.button.button == SDL_BUTTON_WHEELDOWN) {
					EventMouseButtonPressed.emit(MouseWheelDown, mCurrentInputMode);
					for (IInputAdapterStore::const_iterator I = mAdapters.begin(); I != mAdapters.end();) {
						IInputAdapter* adapter = *I;
						++I;
						if (!(adapter)->injectMouseButtonDown(Input::MouseWheelDown))
							break;
					}
				}
			}
			break;
		case SDL_ACTIVEEVENT:
			if (event.active.state & SDL_APPACTIVE) {
				EventWindowActive.emit(event.active.gain);
//On Windows we get a corrupted screen if we just switch to non-fullscreen here.
#ifndef _WIN32
				lostFocus();
#endif
			}
			break;

		case SDL_VIDEORESIZE:
			setGeometry(event.resize.w, event.resize.h);
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
		for (IInputAdapterStore::const_iterator I = mAdapters.begin(); I != mAdapters.end();) {
			IInputAdapter* adapter = *I;
			++I;
			if (!(adapter)->injectChar(scrap[i]))
				break;
		}
	}
#endif // WITHOUT_SCRAP
}

void Input::keyChanged(const SDL_KeyboardEvent &keyEvent)
{
	//catch paste key presses

	//check for paste actions
	if (((keyEvent.keysym.mod & KMOD_CTRL)|| (keyEvent.keysym.mod & KMOD_LCTRL) || (keyEvent.keysym.mod & KMOD_RCTRL))&& (keyEvent.keysym.sym == SDLK_v)) {
		if (keyEvent.type == SDL_KEYDOWN) {
			pasteFromClipboard();
		}
//On windows the OS will handle alt-tab independently, so we don't need to check here
#ifndef _WIN32
		} else if ((keyEvent.keysym.mod & KMOD_LALT) && (keyEvent.keysym.sym == SDLK_TAB)) {
			if (keyEvent.type == SDL_KEYDOWN) {
				lostFocus();
			}
#endif
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

bool Input::isKeyDown(const SDLKey &key) const
{
	return mKeysPressed.find(key) != mKeysPressed.end();
}

void Input::keyPressed(const SDL_KeyboardEvent &keyEvent)
{
	mSuppressForCurrentEvent = false;
	if (mCurrentInputMode == IM_GUI) {
		// do event injection
		// key down
		for (IInputAdapterStore::const_iterator I = mAdapters.begin(); I != mAdapters.end() && !mSuppressForCurrentEvent;) {
			IInputAdapter* adapter = *I;
			++I;
			if (!(adapter)->injectKeyDown(keyEvent.keysym.sym))
				break;
		}

		// now character
		if (mNonCharKeys.find(keyEvent.keysym.sym) == mNonCharKeys.end()) {
			for (IInputAdapterStore::const_iterator I = mAdapters.begin(); I != mAdapters.end() && !mSuppressForCurrentEvent;) {
				IInputAdapter* adapter = *I;
				++I;
				if (!(adapter)->injectChar(keyEvent.keysym.unicode))
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
		for (IInputAdapterStore::const_iterator I = mAdapters.begin(); I != mAdapters.end() && !mSuppressForCurrentEvent;) {
			IInputAdapter* adapter = *I;
			++I;
			if (!(adapter)->injectKeyUp(keyEvent.keysym.sym))
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
	setMouseGrab(mode == IM_MOVEMENT);
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
	mAdapters.push_front(adapter);
}

void Input::removeAdapter(IInputAdapter* adapter)
{
	mAdapters.remove(adapter);
}

const MousePosition& Input::getMousePosition() const
{
	return mMousePosition;
}

void Input::setMainLoopController(MainLoopController* mainLoopController)
{
	mMainLoopController = mainLoopController;
}

MainLoopController* Input::getMainLoopController() const
{
	return mMainLoopController;
}

void Input::Config_CatchMouse(const std::string& section, const std::string& key, varconf::Variable& variable)
{
	try {
		if (variable.is_bool()) {
			bool enabled = static_cast<bool>(variable);
			if (enabled) {
				mMouseGrabbingRequested = true;
			} else {
				setMouseGrab(false);
			}
		}
	} catch (const std::exception& ex) {
		S_LOG_FAILURE("Error when changing mouse grabbing." << ex);
	}
}

void Input::Config_InvertCamera(const std::string& section, const std::string& key, varconf::Variable& variable)
{
	if (variable.is_bool()) {
		mInvertMouse = static_cast<bool>(variable) ? -1 : 1;
	}
}

void Input::setFullscreen(bool enabled)
{
	int newFlags = mMainVideoSurface->flags;

	if (enabled) {
		newFlags |= SDL_FULLSCREEN;
	} else {
		newFlags &= ~SDL_FULLSCREEN;
	}
	mMainVideoSurface = SDL_SetVideoMode(mMainVideoSurface->w, mMainVideoSurface->h, mMainVideoSurface->format->BitsPerPixel, newFlags);

}

bool Input::hasWindow() const
{
	return mMainVideoSurface != 0;
}

void Input::lostFocus()
{
	setInputMode(Input::IM_GUI);
	setMouseGrab(false);
	setFullscreen(false);
//	EventWindowFocusChange.emit();
}

void Input::setMouseGrab(bool enabled)
{
	if (!enabled) {
		S_LOG_INFO("Releasing mouse.");
		SDL_WM_GrabInput(SDL_GRAB_OFF);
		mMouseGrabbingRequested = false;
	} else {
		Uint8 appState = SDL_GetAppState();
		if (enabled && !(appState & SDL_APPMOUSEFOCUS)) {
			mMouseGrabbingRequested = true;
		} else {
			S_LOG_VERBOSE("Setting mouse catching to " << (enabled ? "enabled": "disabled"));
			SDL_WM_GrabInput(enabled ? SDL_GRAB_ON : SDL_GRAB_OFF);
			mMouseGrabbingRequested = false;
		}
	}
}

}

