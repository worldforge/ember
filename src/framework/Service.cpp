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

#include "Service.h"
#include "LoggingInstance.h"

namespace Ember {

Service::Service(std::string name) :
		mName(std::move(name)),
		mRunning(false) {
}

Service::~Service() {
	assert(!mRunning); //Make sure that stop() is called before the destruction of the service.
}

bool Service::isRunning() const {
	return mRunning;
}

void Service::stop() {
	EventStopping.emit();
	setRunning(false);
}

void Service::setRunning(bool running) {
	if (mRunning && !running) {
		S_LOG_INFO("Service '" << mName << "' stopped.");
	}
	mRunning = running;
}

}
