/*
 Copyright (C) 2011 Erik Hjortsberg

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

#include "Service.h"
#include "LoggingInstance.h"

namespace Ember
{

Service::Service() :
	myRunning(false), myStatus(OK)
{
}

Service::~Service()
{
	if (myRunning) {
		stop(0);
	}
}

const std::string& Service::getName() const
{
	return myName;
}

const std::string& Service::getDescription() const
{
	return myDescription;
}

Service::Status Service::getStatus() const
{
	return myStatus;
}

bool Service::isRunning() const
{
	return myRunning;
}

const std::string& Service::getStatusText() const
{
	return myStatusText;
}

void Service::stop(int code)
{
	EventStopping.emit();
	setRunning(false);
}

void Service::setName(const std::string& name)
{
	myName = name;
}

void Service::setDescription(const std::string& description)
{
	myDescription = description;
}

void Service::setStatus(Service::Status status)
{
	myStatus = status;
}

void Service::setRunning(bool running)
{
	if (myRunning && !running) {
		S_LOG_INFO("Service '"<< myName <<"' stopped.");
	}
	myRunning = running;
}

void Service::setStatusText(const std::string& statusText)
{
	myStatusText = statusText;
}

}
