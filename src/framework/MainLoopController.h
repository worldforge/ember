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

#ifndef MAINLOOPCONTROLLER_H_
#define MAINLOOPCONTROLLER_H_

#include <sigc++/signal.h>
#include <Eris/EventService.h>
#include "Singleton.h"

namespace Ember {

class TimeFrame;

/**
 * @author Erik Ogenvik <erik@ogenvik.org>
 *
 * @brief Singleton instance representing the main loop.
 *
 * This is mainly used to request that the application quits.
 */
class MainLoopController : public Singleton<MainLoopController> {
public:

	/**
	 * @brief Specifies what kind of actions have been carried out in one frame cycle.
	 */
	enum FrameAction {
		/**
		 * Graphics was updated.
		 */
				FA_GRAPHICS = 1 << 0,

		/**
		 * Eris was polled.
		 */
				FA_ERIS = 1 << 1,

		/**
		 * Sound was updated.
		 */
				FA_SOUND = 1 << 2,

		/**
		 * Input was handled.
		 */
				FA_INPUT = 1 << 3
	};

	/**
	 * @brief Ctor.
	 * @param shouldQuit A reference to a boolean which represents whether the application should quit.
	 * @param pollEris Whether Eris should be polled each frame.
	 */
	MainLoopController(bool& shouldQuit, bool& pollEris, Eris::EventService& eventService);

	/**
	 * @brief Return true if application has received an "exit" command else false.
	 *
	 * @return true if "shouldquit" else false
	 */
	bool shouldQuit();

	/**
	 * @brief Causes the application to quit.
	 * This will instantly shut down the application, in contrast to requestQuit which will try to show a confirmation dialog to the user.
	 */
	void quit();

	/**
	 * @brief Call this to "soft quit" the app. This means that an signal will be emitted, which hopefully will be taken care of by some widget, which will show a confirmation window, asking the user if he/she wants to quit.
	 * However, if there is no widget etc. handling the request, the application will instantly quit.
	 */
	void requestQuit();

	/**
	 * @brief Sets whether eris should be polled each frame. Defaults to true.
	 * Normally Eris is polled each frame. A "poll" means that Eris is asked to send and receive any data from the server and act on it.
	 * @param doPoll True if polling should occur each frame.
	 */
	void setErisPolling(bool doPoll);

	/**
	 * @brief Gets whether eris should be polled each frame.
	 * @return True if polling occurs each frame.
	 */
	bool getErisPolling() const;

	Eris::EventService& getEventService();

	/**
	 * @brief Emitted before processing input. This event is emitted continuously.
	 * The parameter sent is the time slice since this event last was emitted.
	 */
	sigc::signal<void, float> EventBeforeInputProcessing;

	/**
	 * @brief Emitted after processing input. This event is emitted continuously.
	 * The parameter sent is the time slice since this event last was emitted.
	 */
	sigc::signal<void, float> EventAfterInputProcessing;

	/**
	 * @brief Emitted when the use wants to quit the game. Preferably the GUI should show some kind of confirmation window.
	 */
	sigc::signal<void, bool&> EventRequestQuit;

	/**
	 * @brief Emitted after one frame has been processed.
	 * The parameters sent is the time frame for this frame as well as a bitmask of what kind of actions were carried out. See FrameAction.
	 */
	sigc::signal<void, const TimeFrame&, unsigned int> EventFrameProcessed;

private:

	/**
	 * @brief Set this to true when the application should quit.
	 */
	bool& mShouldQuit;

	/**
	 * @brief Whether Eris should be polled each frame or not.
	 */
	bool& mPollEris;

	Eris::EventService& mEventService;

};

}
#endif /* MAINLOOPCONTROLLER_H_ */
