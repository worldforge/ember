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

#ifndef CURSORWORLDLISTENER_H_
#define CURSORWORLDLISTENER_H_

#include "components/ogre/IWorldPickListener.h"
#include "services/input/Input.h"
#include "services/config/ConfigListenerContainer.h"

#include <CEGUIEvent.h>
#include <sigc++/trackable.h>
#include <vector>

namespace CEGUI
{
class Window;
}

namespace Ember
{
class MainLoopController;
class ConfigListenerContainer;

namespace OgreView
{
namespace Camera
{
class MainCamera;
}
namespace Gui
{

/**
 * @author Erik Ogenvik <erik@ogenvik.org>
 *
 *
 */
class CursorWorldListener: public virtual sigc::trackable
{
public:
	CursorWorldListener(MainLoopController& mainLoopController, CEGUI::Window& mainWindow, Camera::MainCamera& mainCamera);
	virtual ~CursorWorldListener();

protected:

	/**
	 * @brief Stores CEGUI::Event::Connection instances.
	 */
	typedef std::vector<CEGUI::Event::Connection> ConnectionStore;

	CEGUI::Window& mMainWindow;
	Camera::MainCamera& mMainCamera;

	bool mHoverEventSent;

	long long mCursorLingerStart;

	CEGUI::Event::Connection mMouseMovesConnection;

	/**
	 * @brief The threshold, in milliseconds, for when releasing a pressed mouse button is considered a "click".
	 *
	 * If the mouse button is pressed longer than this, it's considered as a "press" event.
	 */
	int mClickThresholdMilliseconds;

	/**
	 * @brief Keeps track of when any mouse button press event started.
	 *
	 * This is used to determine whether a mouse "press" or "click" event should be emitted.
	 */
	long long mMousePressedStart;

	/**
	 * @brief Listens for config changes.
	 */
	ConfigListenerContainer* mConfigListenerContainer;

	/**
	 * @brief CEGUI event connections. These will be disconnected when this instance is deleted.
	 */
	ConnectionStore mConnections;

	void afterEventProcessing(float timeslice);

	bool windowMouseEnters(const CEGUI::EventArgs& args);
	bool windowMouseLeaves(const CEGUI::EventArgs& args);
	bool windowMouseMoves(const CEGUI::EventArgs& args);
	bool windowMouseButtonDown(const CEGUI::EventArgs& args);
	bool windowMouseButtonUp(const CEGUI::EventArgs& args);
	bool windowMouseDoubleClick(const CEGUI::EventArgs& args);

	void sendHoverEvent();

	/**
	 * @brief Listens to the mouse button being released.
	 * @param button
	 * @param inputMode
	 */
	void input_MouseButtonReleased(Input::MouseButton button, Input::InputMode inputMode);

	/**
	 * @brief Sends a world click event to any listeners.
	 *
	 * This is called when the user either has clicked in the world, or pressed and held the mouse button.
	 * @param pickType The kind of picking type.
	 * @param pixelPosition The position, in pixels, of the mouse cursor.
	 */
	void sendWorldClick(MousePickType pickType, const CEGUI::Vector2& pixelPosition);

	/**
	 * Checks if the GUI is in "click" mode.
	 *
	 * @return True if the GUI is in "click" mode.
	 */
	const bool isInGUIMode() const;

	/**
	 * @brief Sets the "click" time.
	 * @param section
	 * @param key
	 * @param variable
	 */
	void Config_ClickThreshold(const std::string& section, const std::string& key, varconf::Variable& variable);

};

}
}
}
#endif /* CURSORINACTIVELISTENER_H_ */
