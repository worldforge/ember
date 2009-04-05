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
#include "framework/Singleton.h"

// Include library headers here
#include <cstdarg>
#include <memory>
#include <sigc++/object.h>

#ifdef WIN32
    // These are needed by mingw
    #include <ctime>
#endif

namespace Ember
{

class ErisLogReciever;
class Observer;

//======================================================================
// Short type macros
//======================================================================

#define ENDM (Ember::LoggingService::END_MESSAGE)
#define HEX_NUM(number) (Ember::LoggingService::hexNumber(number))
	
//Added by nikal 2002/10/22 For convenience. 
//%TASK nikal,1: Perhaps a script to switch the macros to the actual function call would make code more readable?
#define S_LOG_VERBOSE(message) (Ember::LoggingService::getSingleton().slog(__FILE__, __LINE__, Ember::LoggingService::VERBOSE) << message << ENDM)
#define S_LOG_INFO(message) (Ember::LoggingService::getSingleton().slog(__FILE__, __LINE__, Ember::LoggingService::INFO) << message << ENDM)
#define S_LOG_WARNING(message) (Ember::LoggingService::getSingleton().slog(__FILE__, __LINE__, Ember::LoggingService::WARNING) << message << ENDM)
#define S_LOG_FAILURE(message) (Ember::LoggingService::getSingleton().slog(__FILE__, __LINE__, Ember::LoggingService::FAILURE) << message << ENDM)
#define S_LOG_CRITICAL(message) (Ember::LoggingService::getSingleton().slog(__FILE__, __LINE__, Ember::LoggingService::CRITICAL) << message << ENDM)


class LoggingInstance;

/**
* @brief Easy-to-deal-with logging class.
*
* This service class should make adding and observing logging messages more easy. This
* can be done using printf-like log method or just streaming operator << in
* cout-like way.
*
* The cout-like way always needs a LoggingService::END_MESSAGE to be streamed-in
* last (use ENDM* macro). To specify formats like hexadecimal printing static functions
* are available for conversion (use the HEX_NUM* macro).
*
*
*
* There are some log variants for which you can specify some options:
* - source file (use __FILE__)
* - source line (use __LINE__)
* - level of importance (see enum called MessageImportance), always INFO if not specified
*
* As a special feature you can use a function called slog* (abbr. for stream log) that can
* be used for setting the options before using streaming. (See example.)
*
* Observers of logging process can easily be managed
* using addObserver and removeObserver. An observer class handling FILE * is predefined already.
*
* To less the amount of messages passed through to the observers, you can specify a filter by
* levels of importance. Thus all messages above or equal a filter level of importance are
* written/passed by the callback to an observer.
*
*
* HINT: Names marked with * were chosen this short, because they are intentended to be used very
* frequently.
*
* SAMPLE:
* using namespace Ember::services;
* LoggingService * logger;
* //service is assumed to be started; observers are added
*
* //do you prefer this way?
* logger->log(__FILE__, __LINE__, LoggingService::WARNING,
*      "Player %s (ID: %x) is already dead (but used in %d new messages).",
*		player->getName(), player->getID(), player->getMessages()->getCount());
*
* //or this?
* logger->slog(__FILE__, __LINE__, LoggingService::WARNING) << "Player: " << player->getName()
*		<<"(ID: " << HEX_NUM(player->getID()) << "is already dead (but used in " <<
*      player->getMessages()->getCount() << " new messages)." << ENDM;
*
*
* @author Tim Enderling
*/
class LoggingService : public Service, public Ember::Singleton<LoggingService>
{
	public:
	friend class ErisLogReciever;

	/**
	* Creates a new LoggingService using default values.
	*/
	LoggingService ();

	/**
	* Deletes a LoggingService instance.
	*/
	virtual ~LoggingService ();

	virtual Service::Status start ();

    virtual void stop(int code);



private:

	std::auto_ptr<ErisLogReciever> mErisLogReciever;


};// LoggingService


}// namespace Ember

#endif
