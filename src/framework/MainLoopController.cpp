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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <Eris/Session.h>
#include "MainLoopController.h"

namespace Ember {

MainLoopController::MainLoopController(bool& shouldQuit, bool& pollEris, Eris::Session& session) :
		mShouldQuit(shouldQuit), mPollEris(pollEris), mSession(session) {
}

bool MainLoopController::shouldQuit() const {
	return mShouldQuit;
}

void MainLoopController::requestQuit() {
	bool handled = false;
	EventRequestQuit.emit(handled);
	//check it was handled (for example if the gui wants to show a confirmation window)
	if (!handled) {
		//it's not handled, quit now
		quit();
	}

}

void MainLoopController::quit() {
	mShouldQuit = true;
}

void MainLoopController::setErisPolling(bool doPoll) {
	mPollEris = doPoll;
}

bool MainLoopController::getErisPolling() const {
	return mPollEris;
}

Eris::EventService& MainLoopController::getEventService() {
	return mSession.getEventService();
}

boost::asio::io_service& MainLoopController::getIoService() {
	return mSession.getIoService();
}


}
