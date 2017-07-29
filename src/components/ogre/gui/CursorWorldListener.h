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
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef CURSORWORLDLISTENER_H_
#define CURSORWORLDLISTENER_H_

#include "components/ogre/IWorldPickListener.h"
#include "services/input/Input.h"
#include "services/config/ConfigListenerContainer.h"

#include <CEGUI/Event.h>
#include <sigc++/trackable.h>
#include <vector>

namespace CEGUI
{
class Window;
template <typename> class Vector2;
typedef class Vector2<float> Vector2f;

}

namespace Ember
{
class MainLoopController;
class ConfigListenerContainer;
class TimeFrame;

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
 * @brief Handles clicks on the world.
 *
 * Whenever the user clicks on the world (in contrast to interacting with the UI) this should be handled by this class.
 * The actual handling of mouse press events will be handled by the MainCamera instance, which will delegate to IWorldPickListener instances.
 * The behaviour is as follows:
 * First a "press" event is emitted.
 * If the user releases the mouse button within a threshold set by mClickThresholdMilliseconds it will be considered a "click" event, and one such will be emitted.
 * If not, and the user keeps the mouse button pressed, it is considered a "pressed" event, to be emitted.
 *
 * In addition, if the mouse cursors linger too long, a "hover" event will be emitted.
 *
 */
class CursorWorldListener: public virtual sigc::trackable
{
public:

	/**
	 * @brief Ctor.
	 * @param mainLoopController The main loop controller.
	 * @param mainWindow The main window which covers the whole world. When this is clicked events will be emitted.
	 * @param mainCamera The main camera, through which all picking events will be emitted.
	 */
	CursorWorldListener(MainLoopController& mainLoopController, CEGUI::Window& mainWindow, Camera::MainCamera& mainCamera);

	/**
	 * @brief Dtor.
	 */
	virtual ~CursorWorldListener();

protected:

	/**
	 * @brief Stores CEGUI::Event::Connection instances.
	 */
	typedef std::vector<CEGUI::Event::Connection> ConnectionStore;

	/**
	 * @brief The main window which covers the whole world. When this is clicked events will be emitted.
	 */
	CEGUI::Window& mMainWindow;

	/**
	 * @brief The main camera, through which all picking events will be emitted.
	 */
	Camera::MainCamera& mMainCamera;

	/**
	 * @brief If a "hover" event already has been sent this will be true.
	 *
	 * This is set to false whenever the mouse moves.
	 */
	bool mHoverEventSent;

	/**
	 * @brief The time stamp of when the cursor started lingering, i.e. not moving.
	 *
	 * This is used to determine when the cursor has lingered long enough for a "hover" event to be sent.
	 */
	long long mCursorLingerStart;

	/**
	 * @brief Keeps track of the connection for mouse movements.
	 *
	 * This should only be valid when the mouse is contained within the main window.
	 */
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
	TimeFrame* mMousePressedTimeFrame;

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

	/**
	 * @brief Sends a hover event.
	 *
	 * Call this when the mouse has lingered long enough.
	 */
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
	void sendWorldClick(MousePickType pickType, const CEGUI::Vector2f& pixelPosition);

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
