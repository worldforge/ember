/*
 Copyright (C) 2011 Erik Ogenvik

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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "CursorWorldListener.h"

#include "framework/Time.h"
#include "framework/LoggingInstance.h"
#include "framework/MainLoopController.h"
#include "components/ogre/camera/MainCamera.h"
#include <CEGUIWindow.h>
#include <CEGUIWindowManager.h>

namespace Ember
{
namespace OgreView
{
namespace Gui
{

CursorWorldListener::CursorWorldListener(MainLoopController& mainLoopController, CEGUI::Window& mainWindow, Camera::MainCamera& mainCamera) :
		mMainWindow(mainWindow), mMainCamera(mainCamera), mHoverEventSent(false), mCursorLingerStart(0), mClickThresholdMilliseconds(200), mMousePressedStart(0), mConfigListenerContainer(new ConfigListenerContainer())
{
	mainLoopController.EventBeforeInputProcessing.connect(sigc::mem_fun(*this, &CursorWorldListener::afterEventProcessing));
	Ember::Input::getSingleton().EventMouseButtonReleased.connect(sigc::mem_fun(*this, &CursorWorldListener::input_MouseButtonReleased));

	mConnections.push_back(mMainWindow.subscribeEvent(CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber(&CursorWorldListener::windowMouseEnters, this)));
	mConnections.push_back(mMainWindow.subscribeEvent(CEGUI::Window::EventMouseLeaves, CEGUI::Event::Subscriber(&CursorWorldListener::windowMouseLeaves, this)));

	mConnections.push_back(mMainWindow.subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&CursorWorldListener::windowMouseButtonDown, this)));
	mConnections.push_back(mMainWindow.subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&CursorWorldListener::windowMouseButtonUp, this)));
	mConnections.push_back(mMainWindow.subscribeEvent(CEGUI::Window::EventMouseDoubleClick, CEGUI::Event::Subscriber(&CursorWorldListener::windowMouseDoubleClick, this)));

	mConfigListenerContainer->registerConfigListenerWithDefaults("input", "clickthreshold", sigc::mem_fun(*this, &CursorWorldListener::Config_ClickThreshold), 200);

}

CursorWorldListener::~CursorWorldListener()
{
	delete mConfigListenerContainer;
	for (ConnectionStore::iterator I = mConnections.begin(); I != mConnections.end(); ++I) {
		(*I)->disconnect();
	}
	if (mMouseMovesConnection.isValid()) {
		mMouseMovesConnection->disconnect();
	}
}

void CursorWorldListener::afterEventProcessing(float timeslice)
{
	if (!mHoverEventSent) {
		mCursorLingerStart += timeslice * 1000;

		if (mCursorLingerStart > 500) {
			sendHoverEvent();
		}
	}

	if (isInGUIMode()) {
		if (mMousePressedStart != 0) {
			if ((Time::currentTimeMillis() - mMousePressedStart) > mClickThresholdMilliseconds) {
				mMousePressedStart = 0;
				sendWorldClick(MPT_PRESSED, CEGUI::MouseCursor::getSingleton().getPosition());
			}
		}
	}
}

bool CursorWorldListener::windowMouseEnters(const CEGUI::EventArgs& args)
{
	mMouseMovesConnection = mMainWindow.subscribeEvent(CEGUI::Window::EventMouseMove, CEGUI::Event::Subscriber(&CursorWorldListener::windowMouseMoves, this));
	mHoverEventSent = false;
	return true;
}

bool CursorWorldListener::windowMouseLeaves(const CEGUI::EventArgs& args)
{
	if (mMouseMovesConnection.isValid()) {
		mMouseMovesConnection->disconnect();
		mMouseMovesConnection = CEGUI::Event::Connection();
	}
	mHoverEventSent = true;
	return true;
}

bool CursorWorldListener::windowMouseMoves(const CEGUI::EventArgs& args)
{
	mCursorLingerStart = 0;
	mHoverEventSent = false;
	return true;
}

void CursorWorldListener::sendHoverEvent()
{
	const CEGUI::Vector2& pixelPosition = CEGUI::MouseCursor::getSingleton().getPosition();
	sendWorldClick(MPT_HOVER, pixelPosition);
	mHoverEventSent = true;
}

void CursorWorldListener::input_MouseButtonReleased(Input::MouseButton button, Input::InputMode inputMode)
{
	mMousePressedStart = 0;
}

void CursorWorldListener::sendWorldClick(MousePickType pickType, const CEGUI::Vector2& pixelPosition)
{

	const CEGUI::Point& position = CEGUI::MouseCursor::getSingleton().getDisplayIndependantPosition();
	MousePickerArgs pickerArgs;
	pickerArgs.windowX = pixelPosition.d_x;
	pickerArgs.windowY = pixelPosition.d_y;
	pickerArgs.pickType = pickType;
	mMainCamera.pickInWorld(position.d_x, position.d_y, pickerArgs);

}

bool CursorWorldListener::windowMouseButtonDown(const CEGUI::EventArgs& args)
{
	if (isInGUIMode()) {
		S_LOG_VERBOSE("Main sheet is capturing input");
		CEGUI::Window* aWindow = CEGUI::Window::getCaptureWindow();
		if (aWindow) {
			aWindow->releaseInput();
			aWindow->deactivate();
		}

		mMousePressedStart = Time::currentTimeMillis();
		sendWorldClick(MPT_PRESS, CEGUI::MouseCursor::getSingleton().getPosition());
	}

	return true;
}

bool CursorWorldListener::windowMouseButtonUp(const CEGUI::EventArgs& args)
{
	if (isInGUIMode()) {
		if (mMousePressedStart != 0) {
			if ((Time::currentTimeMillis() - mMousePressedStart) < mClickThresholdMilliseconds) {
				mMousePressedStart = 0;
				sendWorldClick(MPT_CLICK, static_cast<const CEGUI::MouseEventArgs&>(args).position);
			}
		}
	}
	return true;
}

bool CursorWorldListener::windowMouseDoubleClick(const CEGUI::EventArgs& args)
{
	const CEGUI::MouseEventArgs& mouseArgs = static_cast<const CEGUI::MouseEventArgs&>(args);
	sendWorldClick(MPT_DOUBLECLICK, mouseArgs.position);

	return true;
}

const bool CursorWorldListener::isInGUIMode() const
{
	return Input::getSingleton().getInputMode() == Input::IM_GUI;
}

void CursorWorldListener::Config_ClickThreshold(const std::string& section, const std::string& key, varconf::Variable& variable)
{
	if (variable.is_int()) {
		mClickThresholdMilliseconds = static_cast<int>(variable);
	}
}

}
}
}
