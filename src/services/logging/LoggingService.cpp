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
#include <stdio.h>
#include <sstream>
//#include <varargs.h> //TODO: Needed by unix?

#include <sigc++/object_slot.h>

namespace Ember {

template<> Ember::LoggingService* Ember::Singleton<Ember::LoggingService>::ms_Singleton = 0;


Service::Status LoggingService::start ()
{
    setRunning (true);
    setStatus (Service::OK);
    return Service::OK;
}

void LoggingService::stop(int code)
{
	slog(__FILE__, __LINE__, Ember::LoggingService::INFO) << "Shutting down logging service." << END_MESSAGE;
	Service::stop(code);
}



void LoggingService::log (const char *message, ...)
{
    va_list vl;
    va_start (vl, message);
    logVarParam ("", -1, INFO, message, vl);
    va_end (vl);
}


void LoggingService::log (const char *file, const char *message, ...)
{
    va_list vl;
    va_start (vl, message);
    logVarParam (file, -1, INFO, message, vl);
    va_end (vl);
}



void LoggingService::log (const char *file, const int line,
                                const char *message, ...)
{
    va_list vl;
    va_start (vl, message);
    logVarParam (file, line, INFO, message, vl);
    va_end (vl);
}


void LoggingService::log (const MessageImportance importance,
                                const char *message, ...)
{
    va_list vl;
    va_start (vl, message);
    logVarParam ("", -1, importance, message, vl);
    va_end (vl);
}

void LoggingService::log (const char *file,
                                const MessageImportance importance,
                                const char *message, ...)
{
    va_list vl;
    va_start (vl, message);
    logVarParam (file, -1, importance, message, vl);
    va_end (vl);
}

void LoggingService::log (const char *file, const int line,
                                const MessageImportance importance,
                                const char *message, ...)
{
    va_list vl;
    va_start (vl, message);
    logVarParam (file, line, importance, message, vl);
    va_end (vl);
}

void LoggingService::logVarParam (const char *file, const int line,
                                        const MessageImportance importance,
                                        const char *message, va_list argptr)
{
    char Buffer[MESSAGE_BUFFER_SIZE];
    vsprintf ((char *) Buffer, message, argptr);
    sendMessage (std::string ((char *) Buffer), file, line, importance);
}


LoggingService & LoggingService::slog (const std::string & file,
                                                   const int line,
                                                   const MessageImportance
                                                   importance)
{
    myFile = file;
    myLine = line;
    myImportance = importance;
    return *this;
}

LoggingService & LoggingService::
slog (const MessageImportance importance)
{
    myImportance = importance;
    return *this;
}

LoggingService & LoggingService::slog (const std::string & file,
                                                   const MessageImportance
                                                   importance)
{
    myFile = file;
    myImportance = importance;
    return *this;
}

LoggingService & LoggingService::slog (const std::string & file,
                                                   const int line)
{
    myFile = file;
    myLine = line;
    return *this;
}

LoggingService & LoggingService::slog (const std::string & file)
{
    myFile = file;
    return *this;
}




void LoggingService::addObserver (Observer * observer)
{
    //test on already existing observer
    for (ObserverList::iterator i = myObserverList.begin ();
         i != myObserverList.end (); i++)
    {
        if (*i == observer)
        {
            return;
        }
    }

    //no existing observer, add a new                    
    myObserverList.push_back (observer);
}

int LoggingService::removeObserver (Observer * observer)
{
    for (ObserverList::iterator i = myObserverList.begin ();
         i != myObserverList.end (); i++)
    {
        if (*i == observer)
        {
            myObserverList.erase (i);
            return 0;
        }
    }

    return -1;
}

LoggingService::HexNumber LoggingService::
hexNumber (const int intDecimal)
{
    HexNumber intHex;
    intHex.myNumber = intDecimal;
    return intHex;
}

LoggingService & LoggingService::operator<< (const std::string & stringToAdd)
{
    myMessage += stringToAdd;
    return *this;
}

LoggingService & LoggingService::operator<< (const unsigned int uintToAdd)
{
	std::stringstream ss;
	ss << uintToAdd;
    myMessage += ss.str();
    
    return *this;
}

LoggingService & LoggingService::operator<< (const int intToAdd)
{
	std::stringstream ss;
	ss << intToAdd;
    myMessage += ss.str();
    
    return *this;
}

LoggingService & LoggingService::
operator<< (const HexNumber & intHexToAdd)
{
    char buffer[NUMBER_BUFFER_SIZE];
    sprintf (buffer, "%x", intHexToAdd.myNumber);
    myMessage += buffer;
    return *this;
}

LoggingService & LoggingService::
operator<< (const double doubleToAdd)
{
	std::stringstream ss;
	ss << doubleToAdd;
    myMessage += ss.str();
    return *this;
}

LoggingService & LoggingService::operator<< (const long longToAdd)
{
	std::stringstream ss;
	ss << longToAdd;
    myMessage += ss.str();
    
    return *this;
}

LoggingService & LoggingService::
operator<< (const unsigned long ulongToAdd)
{
	std::stringstream ss;
	ss << ulongToAdd;
    myMessage += ss.str();
    return *this;
}


void LoggingService::operator<< (const EndMessageEnum endMessage)
{
    sendMessage (myMessage, myFile, myLine, myImportance);

    myMessage = "";
    myFile = "";
    myLine = -1;
    myImportance = INFO;
}

LoggingService::LoggingService () : Service()
{
    //set service properties

    setName ("Logging");
    setDescription ("Eases message writing and distribution.");


    //set all option values to not specified
    myMessage = "";
    myFile = "";
    myLine = -1;
    myImportance = INFO;

	mErisLogReciever = std::auto_ptr<ErisLogReciever>(new ErisLogReciever(*this));
}


LoggingService::~LoggingService ()
{

}


void LoggingService::sendMessage (const std::string & message,
                                        const std::string & file,
                                        const int line,
                                        const MessageImportance importance)
{
    time_t currentTime;
    time (&currentTime);

    for (ObserverList::iterator i = myObserverList.begin ();
         i != myObserverList.end (); i++)
    {
        if ((int) importance >= (int) (*i)->getFilter ())
        {
            (*i)->onNewMessage (message, file, line, importance, currentTime);
        }
    }
}

}
