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
#include "IWindowProvider.h"
#include "InputCommandMapper.h"
#include "EmberIcon.h"

#include "IInputAdapter.h"

#include "services/config/ConfigListenerContainer.h"

#include "framework/Tokeniser.h"
#include "framework/ConsoleBackend.h"
#include "framework/LoggingInstance.h"
#include "framework/MainLoopController.h"
#include "framework/utf8.h"

#ifdef _WIN32
#include "platform/platform_windows.h"
#endif

#include <SDL.h>
#include <SDL_syswm.h>
#include <SDL_keyboard.h>
#include <SDL_video.h>

#ifndef __APPLE__
#ifndef _WIN32
#include <X11/Xlib.h>
#endif
#endif

#include <sstream>
#include <algorithm>

template<> Ember::Input* Ember::Singleton<Ember::Input>::ms_Singleton = 0;

namespace Ember
{

const std::string Input::BINDCOMMAND("bind");
const std::string Input::UNBINDCOMMAND("unbind");

Input::Input() :
		ToggleFullscreen(0), mCurrentInputMode(IM_GUI), mMouseState(0), mTimeSinceLastRightMouseClick(0), mSuppressForCurrentEvent(false), mMovementModeEnabled(false), mConfigListenerContainer(new ConfigListenerContainer()), mMouseGrabbingRequested(false), mMouseGrab(false), mMainLoopController(0), mWindowProvider(nullptr), mScreenWidth(0), mScreenHeight(0), mMainVideoSurface(0), mInvertMouse(1), mHandleOpenGL(false), mMainWindowId(0)
{
	mMousePosition.xPixelPosition = 0;
	mMousePosition.yPixelPosition = 0;
	mMousePosition.xRelativePosition = 0.0f;
	mMousePosition.yRelativePosition = 0.0f;

	//If building WebEmber on Linux we shouldn't initialize SDL here, as that's done separately in another thread.
#if !defined(BUILD_WEBEMBER) || defined(_WIN32) || defined(__APPLE__)
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE);
#endif
	mLastTick = std::chrono::system_clock::now();

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
	int flags = SDL_WINDOW_SHOWN;

	if (resizable) {
		flags |= SDL_WINDOW_RESIZABLE;
	}

	if (mHandleOpenGL) {
		flags |= SDL_WINDOW_OPENGL;
	}

	if (fullscreen) {
		flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
	}

	if (mMainVideoSurface) {
		SDL_DestroyWindow(mMainVideoSurface);
		mMainVideoSurface = nullptr;
		mMainWindowId = 0;
	}
	mMainVideoSurface = SDL_CreateWindow("Ember", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags); // create an SDL window
	createIcon();

	mMainWindowId = SDL_GetWindowID(mMainVideoSurface);
	SDL_GetWindowSize(mMainVideoSurface, &mScreenWidth, &mScreenHeight);
	SDL_SysWMinfo info;
	SDL_VERSION(&info.version);

	SDL_GetWindowWMInfo(mMainVideoSurface, &info);

#ifdef _WIN32
	if (centered) {
		// When SDL is centering the window, it doesn't take into account the tray bar.
		// This approach will center the window to the work area.
		HWND hwnd = info.info.win.window;
		HMONITOR hMonitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
		if (hMonitor) {
			MONITORINFO mon;
			memset(&mon, 0, sizeof(MONITORINFO));
			mon.cbSize = sizeof(MONITORINFO);
			GetMonitorInfo(hMonitor, &mon);

			RECT r;
			GetWindowRect(hwnd, &r);

			int winWidth = r.right - r.left;
			int winLeft = mon.rcWork.left + (mon.rcWork.right - mon.rcWork.left) / 2 - winWidth / 2;

			int winHeight = r.bottom - r.top;
			int winTop = mon.rcWork.top + (mon.rcWork.bottom - mon.rcWork.top) / 2 - winHeight / 2;
			winTop = std::max(winTop, 0);

			SetWindowPos(hwnd, 0, winLeft, winTop, 0, 0, SWP_NOSIZE);
		}
	}
#endif


	SDL_ShowCursor(0);
//	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

	std::stringstream ss;
#ifdef __APPLE__
	//On OSX we'll tell Ogre to use the current OpenGL context; thus we don't need to return the window id
#elif _WIN32
	ss << (size_t)info.info.win.window;
#else
	ss << info.info.x11.window;
#endif

	ConsoleBackend& console = ConsoleBackend::getSingleton();
	console.registerCommand(BINDCOMMAND, this);
	console.registerCommand(UNBINDCOMMAND, this);

	setGeometry(width, height);

	if (!ToggleFullscreen) {
		ToggleFullscreen = new ConsoleCommandWrapper("toggle_fullscreen", this, "Switch between windowed and full screen mode.");
	}

	return ss.str();
}

void Input::shutdownInteraction()
{
	mWindowProvider = 0;

	if (mMainVideoSurface) {
		SDL_DestroyWindow(mMainVideoSurface);
		mMainVideoSurface = 0;
	}

	//Release the mouse for safety's sake.
	SDL_SetWindowGrab(mMainVideoSurface, SDL_FALSE);
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
	SDL_Surface* iconSurface = SDL_CreateRGBSurfaceFrom(emberIcon.pixel_data, 64, 64, 24, 64 * 3, rmask, gmask, bmask, 0);
	if (iconSurface) {
		SDL_SetWindowIcon(mMainVideoSurface, iconSurface);
		//The icon surface isn't needed anymore
		SDL_FreeSurface(iconSurface);
	}

#endif // !BUILD_WEBEMBER
}

void Input::attach(IWindowProvider* windowProvider)
{

	//TODO: this code probably doesn't work with SDL2.
	//This should be redesigned to use the new features found in SDL2 which allows SDL to attach to an already created window

	//The windowProvider should not be nullptr.
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

#ifdef _WIN32

	static SDL_SysWMinfo pInfo;
	SDL_VERSION(&pInfo.version);
	SDL_GetWindowWMInfo(mMainVideoSurface, &pInfo);

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
	GetWindowRect(pInfo.info.win.window, &r);
	SetWindowPos(pInfo.info.win.window, 0, r.left + 1, r.top, 0, 0, SWP_NOSIZE);
	SetWindowPos(pInfo.info.win.window, 0, r.left, r.top, 0, 0, SWP_NOSIZE);

	RAWINPUTDEVICE Rid;
	/* we're telling the window, we want it to report raw input events from mice (needed for SDL-1.3) */
	Rid.usUsagePage = 0x01;
	Rid.usUsage = 0x02;
	Rid.dwFlags = RIDEV_INPUTSINK;
	Rid.hwndTarget = (HWND)pInfo.info.win.window;
	RegisterRawInputDevices(&Rid, 1, sizeof(Rid));
#endif

	ConsoleBackend& console = ConsoleBackend::getSingleton();
	console.registerCommand(BINDCOMMAND, this);
	console.registerCommand(UNBINDCOMMAND, this);

}

void Input::setGeometry(int width, int height)
{
	if (mScreenHeight != height || mScreenWidth != width) {
		mScreenWidth = width;
		mScreenHeight = height;
		SDL_SetWindowSize(mMainVideoSurface, width, height);
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
		setFullscreen((SDL_GetWindowFlags(mMainVideoSurface) & SDL_WINDOW_FULLSCREEN) == 0);
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

void Input::writeToClipboard(char* text, size_t length)
{
	SDL_SetClipboardText(text);
}

void Input::pasteFromClipboard(char*& text, size_t& length)
{
	char* sdl_text = SDL_GetClipboardText();

	length = strlen(sdl_text);

	text = (char*)malloc(length);
	strcpy(text, sdl_text);

	SDL_free(sdl_text);
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
	return SDL_GetWindowFlags(mMainVideoSurface) & SDL_WINDOW_SHOWN;
}

bool Input::isApplicationFocused()
{
	return SDL_GetWindowFlags(mMainVideoSurface) & SDL_WINDOW_MOUSE_FOCUS;
}

void Input::startInteraction()
{
	mConfigListenerContainer->registerConfigListenerWithDefaults("input", "catchmouse", sigc::mem_fun(*this, &Input::Config_CatchMouse), true);
	mConfigListenerContainer->registerConfigListenerWithDefaults("input", "invertcamera", sigc::mem_fun(*this, &Input::Config_InvertCamera), true);
}

void Input::processInput()
{

	auto newTick = std::chrono::system_clock::now();
	auto microsecondsSinceLast = std::chrono::duration_cast<std::chrono::microseconds>(newTick - mLastTick).count();

	float secondsSinceLast = microsecondsSinceLast * 1000000.0f;
	mLastTick = newTick;
	pollMouse(secondsSinceLast);
	pollEvents(secondsSinceLast);
	if (mWindowProvider) {
		mWindowProvider->processInput();
	}
}

void Input::pollMouse(float secondsSinceLast)
{
	SDL_PumpEvents(); // Loop through all pending system events to get the latest mouse position.
	int mouseX, mouseY, mouseRelativeX, mouseRelativeY;
	mMouseState = SDL_GetMouseState(&mouseX, &mouseY);
	if (mCurrentInputMode == IM_GUI) {
		mouseRelativeX = (mMousePosition.xPixelPosition - mouseX);
		mouseRelativeY = (mMousePosition.yPixelPosition - mouseY);
	} else {
		SDL_GetRelativeMouseState(&mouseRelativeX, &mouseRelativeY);
		//Due to legacy code we need to flip the sign on the relative mouse movement when using the SDL2 method.
		mouseRelativeX = -mouseRelativeX;
		mouseRelativeY = -mouseRelativeY;
	}

	//has the mouse moved?
	auto appState = SDL_GetWindowFlags(mMainVideoSurface);
	if (appState & SDL_WINDOW_MOUSE_FOCUS) {
		//Wait with grabbing the mouse until the app has input focus.
		if (mMouseGrabbingRequested && (appState & SDL_WINDOW_INPUT_FOCUS)) {
			setMouseGrab(true);
		}
		if (mouseRelativeX != 0 || mouseRelativeY != 0) {

			//we'll calculate the mouse movement difference and send the values to those
			//listening to the MouseMoved event
			float diffX, diffY;
			diffX = mouseRelativeX / (float)mScreenWidth;
			diffY = mouseRelativeY / (float)mScreenHeight;
			MouseMotion motion;
			motion.xPosition = mouseX;
			motion.yPosition = mouseY;
			motion.xRelativeMovement = diffX * mInvertMouse;
			motion.yRelativeMovement = diffY * mInvertMouse;
			motion.xRelativeMovementInPixels = mouseRelativeX * mInvertMouse;
			motion.yRelativeMovementInPixels = mouseRelativeY * mInvertMouse;
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
				SDL_WarpMouseInWindow(mMainVideoSurface, mMousePosition.xPixelPosition, mMousePosition.yPixelPosition);
			} else {
				mMousePosition.xPixelPosition = mouseX;
				mMousePosition.yPixelPosition = mouseY;
				mMousePosition.xRelativePosition = mouseX / (float)mScreenWidth;
				mMousePosition.yRelativePosition = mouseY / (float)mScreenHeight;
			}

		}
	}

}

void Input::pollEvents(float secondsSinceLast)
{
	mTimeSinceLastRightMouseClick += secondsSinceLast;
	static SDL_Event event;
	while (SDL_PollEvent(&event)) {
		EventSDLEventReceived(event);
		switch (event.type) {
		/* Look for a keypress */
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			keyChanged(event.key);
			break;
		case SDL_TEXTINPUT:
			textInput(event.text);
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

				}
			}
			break;
		case SDL_MOUSEWHEEL:
			if (event.wheel.y > 0) {
				EventMouseButtonPressed.emit(MouseWheelUp, mCurrentInputMode);
				for (IInputAdapterStore::const_iterator I = mAdapters.begin(); I != mAdapters.end();) {
					IInputAdapter* adapter = *I;
					++I;
					if (!(adapter)->injectMouseButtonDown(Input::MouseWheelUp))
						break;
				}
			} else if (event.wheel.y < 0) {
				EventMouseButtonPressed.emit(MouseWheelDown, mCurrentInputMode);
				for (IInputAdapterStore::const_iterator I = mAdapters.begin(); I != mAdapters.end();) {
					IInputAdapter* adapter = *I;
					++I;
					if (!(adapter)->injectMouseButtonDown(Input::MouseWheelDown))
						break;
				}
			}
			break;
		case SDL_WINDOWEVENT:
			if (event.window.windowID == mMainWindowId) {
				if (event.window.event == SDL_WINDOWEVENT_SHOWN) {
					EventWindowActive.emit(true);
				} else if (event.window.event == SDL_WINDOWEVENT_HIDDEN) {
					EventWindowActive.emit(false);
					//On Windows we get a corrupted screen if we just switch to non-fullscreen here.
#ifndef _WIN32
					lostFocus();
#endif
				} else if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
					setGeometry(event.window.data1, event.window.data2);
				}
			}
			break;
		}
	}
}

void Input::textInput(const SDL_TextInputEvent &textEvent)
{
	if (mCurrentInputMode == IM_GUI) {

		const char* text = textEvent.text;
		size_t len = 0;
		for (; len < 4; len++) {
			if (text[len] == 0) {
				break;
			}
		}

		std::vector<int> utf32result;
		utf8::utf8to32(text, text + len, std::back_inserter(utf32result));
		if (!utf32result.empty()) {
			int character = utf32result.front();
			for (IInputAdapterStore::const_iterator I = mAdapters.begin(); I != mAdapters.end() && !mSuppressForCurrentEvent;) {
				IInputAdapter* adapter = *I;
				++I;
				if (!(adapter)->injectChar(character)) {
					break;
				}
			}
		}
	}
}

void Input::keyChanged(const SDL_KeyboardEvent &keyEvent)
{
//On windows the OS will handle alt-tab independently, so we don't need to check here
#ifndef _WIN32
	if ((keyEvent.keysym.mod & KMOD_LALT) && (keyEvent.keysym.sym == SDLK_TAB)) {
		if (keyEvent.type == SDL_KEYDOWN) {
			lostFocus();
		}
#else
		if (false) {
#endif
	} else {
		if (keyEvent.type == SDL_KEYDOWN) {
			mKeysPressed.insert(keyEvent.keysym.scancode);
			keyPressed(keyEvent);
		} else {
			mKeysPressed.erase(keyEvent.keysym.scancode);
			keyReleased(keyEvent);
		}
	}

}

bool Input::isKeyDown(const SDL_Scancode &key) const
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
			if (!(adapter)->injectKeyDown(keyEvent.keysym.scancode))
				break;
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
			if (!(adapter)->injectKeyUp(keyEvent.keysym.scancode))
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
	SDL_SetWindowFullscreen(mMainVideoSurface, enabled ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
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
}

void Input::setMouseGrab(bool enabled)
{
	SDL_SetRelativeMouseMode(enabled ? SDL_TRUE : SDL_FALSE);
	//We must reset the relative mouse state reporting.
	SDL_GetRelativeMouseState(0, 0);
}

}

