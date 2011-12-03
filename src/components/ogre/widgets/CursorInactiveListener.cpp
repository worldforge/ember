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

#include "CursorInactiveListener.h"

#include "framework/Time.h"
#include "framework/LoggingInstance.h"
#include "framework/MainLoopController.h"
#include "components/ogre/MousePicker.h"
#include <CEGUIWindow.h>
#include <CEGUIWindowManager.h>

namespace Ember
{
namespace OgreView
{
namespace Gui
{

CursorInactiveListener::CursorInactiveListener(MainLoopController& mainLoopController, CEGUI::Window& mainWindow, MousePicker& mousePicker) :
		mMainWindow(mainWindow), mMousePicker(mousePicker), mHoverEventSent(false), mCursorLingerStart(0), mClickThresholdMilliseconds(100), mMousePressedStart(0)
{
	mainLoopController.EventAfterInputProcessing.connect(sigc::mem_fun(*this, &CursorInactiveListener::afterEventProcessing));
	Ember::Input::getSingleton().EventMouseButtonReleased.connect(sigc::mem_fun(*this, &CursorInactiveListener::input_MouseButtonReleased));

	mMainWindow.subscribeEvent(CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber(&CursorInactiveListener::windowMouseEnters, this));
	mMainWindow.subscribeEvent(CEGUI::Window::EventMouseLeaves, CEGUI::Event::Subscriber(&CursorInactiveListener::windowMouseLeaves, this));

	mMainWindow.subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&CursorInactiveListener::windowMouseButtonDown, this));
	mMainWindow.subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&CursorInactiveListener::windowMouseButtonUp, this));
	mMainWindow.subscribeEvent(CEGUI::Window::EventMouseDoubleClick, CEGUI::Event::Subscriber(&CursorInactiveListener::windowMouseDoubleClick, this));

}

CursorInactiveListener::~CursorInactiveListener()
{
}

void CursorInactiveListener::afterEventProcessing(float timeslice)
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
				sendWorldClick(MPT_PRESS, CEGUI::MouseCursor::getSingleton().getPosition());
			}
		}
	}
}

bool CursorInactiveListener::windowMouseEnters(const CEGUI::EventArgs& args)
{
	S_LOG_VERBOSE("Mouse enters.");
	mMouseMovesConnection = mMainWindow.subscribeEvent(CEGUI::Window::EventMouseMove, CEGUI::Event::Subscriber(&CursorInactiveListener::windowMouseMoves, this));
	mHoverEventSent = false;
	return true;
}

bool CursorInactiveListener::windowMouseLeaves(const CEGUI::EventArgs& args)
{
	S_LOG_VERBOSE("Mouse leaves.");
	mMouseMovesConnection->disconnect();
	mHoverEventSent = true;
	return true;
}

bool CursorInactiveListener::windowMouseMoves(const CEGUI::EventArgs& args)
{
	mCursorLingerStart = 0;
	mHoverEventSent = false;
	return true;
}

void CursorInactiveListener::sendHoverEvent()
{
	const CEGUI::Vector2& pixelPosition = CEGUI::MouseCursor::getSingleton().getPosition();
	const CEGUI::Point& position = CEGUI::MouseCursor::getSingleton().getDisplayIndependantPosition();
	MousePickerArgs pickerArgs;
	pickerArgs.windowX = pixelPosition.d_x;
	pickerArgs.windowY = pixelPosition.d_y;
	pickerArgs.pickType = MPT_HOVER;
	mMousePicker.doMousePicking(position.d_x, position.d_y, pickerArgs);

	mHoverEventSent = true;
}

void CursorInactiveListener::input_MouseButtonReleased(Input::MouseButton button, Input::InputMode inputMode)
{
	mMousePressedStart = 0;
}

void CursorInactiveListener::sendWorldClick(MousePickType pickType, const CEGUI::Vector2& pixelPosition)
{
	S_LOG_VERBOSE("Main sheet is capturing input");
	CEGUI::Window* aWindow = CEGUI::Window::getCaptureWindow();
	if (aWindow) {
		aWindow->releaseInput();
		aWindow->deactivate();
	}

	const CEGUI::Point& position = CEGUI::MouseCursor::getSingleton().getDisplayIndependantPosition();
	MousePickerArgs pickerArgs;
	pickerArgs.windowX = pixelPosition.d_x;
	pickerArgs.windowY = pixelPosition.d_y;
	pickerArgs.pickType = pickType;
	mMousePicker.doMousePicking(position.d_x, position.d_y, pickerArgs);

}

bool CursorInactiveListener::windowMouseButtonDown(const CEGUI::EventArgs& args)
{
	if (isInGUIMode()) {
		mMousePressedStart = Time::currentTimeMillis();
	}

	return true;
}

bool CursorInactiveListener::windowMouseButtonUp(const CEGUI::EventArgs& args)
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

bool CursorInactiveListener::windowMouseDoubleClick(const CEGUI::EventArgs& args)
{
	const CEGUI::MouseEventArgs& mouseArgs = static_cast<const CEGUI::MouseEventArgs&>(args);
	sendWorldClick(MPT_DOUBLECLICK, mouseArgs.position);

	return true;
}

const bool CursorInactiveListener::isInGUIMode() const
{
	return Input::getSingleton().getInputMode() == Input::IM_GUI;
}
}
}
}
