/*
	Copyright (C) 2002  Tim Enderling
    Copyright (C) 2005	Erik Hjortsberg

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

#include "LoggingService.h"
#include "ErisLogReciever.h"
#include "Observer.h"
#include "FileObserver.h"

// Include system headers here
#include <string>
#include <list>
#include <cstdio>
#include <sstream>

#include <sigc++/object_slot.h>

namespace Ember {

template<> Ember::LoggingService* Ember::Singleton<Ember::LoggingService>::ms_Singleton = 0;


Service::Status LoggingService::start ()
{
	setRunning(true);
	setStatus(Service::OK);
	return Service::OK;
}

void LoggingService::stop(int code)
{
	Service::stop(code);
}




LoggingService::LoggingService () : Service()
{
	//set service properties
	
	setName("Logging");
	setDescription("Eases message writing and distribution.");
	
	mErisLogReciever = std::auto_ptr<ErisLogReciever>(new ErisLogReciever(*this));
}


LoggingService::~LoggingService ()
{

}


}
