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

#ifndef LOGGINGSERVICE_H
#define LOGGINGSERVICE_H

// Include other headers of the current program here
#include "framework/Service.h"

// Include library headers here
#include <memory>



namespace Ember
{

class ErisLogReciever;

class LoggingService : public Service
{
	public:

	/**
	* Creates a new LoggingService using default values.
	*/
	LoggingService();

	/**
	* Deletes a LoggingService instance.
	*/
	virtual ~LoggingService();

	virtual Service::Status start();

    virtual void stop(int code);



private:

	std::auto_ptr<ErisLogReciever> mErisLogReciever;


};// LoggingService


}// namespace Ember

#endif
