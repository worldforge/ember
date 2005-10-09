#include "LoggingService.h"

// Include system headers here
#include <string>
#include <list>
#include <stdio.h>
//#include <varargs.h> //TODO: Needed by unix?

#include <sigc++/object_slot.h>



Ember::LoggingService* Ember::LoggingService::theInstance = NULL;

Ember::Service::Status Ember::LoggingService::start ()
{
    setRunning (true);
    setStatus (Service::OK);
    return Service::OK;
}


void Ember::LoggingService::log (const char *message, ...)
{
    va_list vl;
    va_start (vl, message);
    logVarParam ("", -1, INFO, message, vl);
    va_end (vl);
}


void Ember::LoggingService::log (const char *file, const char *message, ...)
{
    va_list vl;
    va_start (vl, message);
    logVarParam (file, -1, INFO, message, vl);
    va_end (vl);
}



void Ember::LoggingService::log (const char *file, const int line,
                                const char *message, ...)
{
    va_list vl;
    va_start (vl, message);
    logVarParam (file, line, INFO, message, vl);
    va_end (vl);
}


void Ember::LoggingService::log (const MessageImportance importance,
                                const char *message, ...)
{
    va_list vl;
    va_start (vl, message);
    logVarParam ("", -1, importance, message, vl);
    va_end (vl);
}

void Ember::LoggingService::log (const char *file,
                                const MessageImportance importance,
                                const char *message, ...)
{
    va_list vl;
    va_start (vl, message);
    logVarParam (file, -1, importance, message, vl);
    va_end (vl);
}

void Ember::LoggingService::log (const char *file, const int line,
                                const MessageImportance importance,
                                const char *message, ...)
{
    va_list vl;
    va_start (vl, message);
    logVarParam (file, line, importance, message, vl);
    va_end (vl);
}

void Ember::LoggingService::logVarParam (const char *file, const int line,
                                        const MessageImportance importance,
                                        const char *message, va_list argptr)
{
    char Buffer[MESSAGE_BUFFER_SIZE];
    vsprintf ((char *) Buffer, message, argptr);
    sendMessage (std::string ((char *) Buffer), file, line, importance);
}


Ember::LoggingService & Ember::LoggingService::slog (const std::string & file,
                                                   const int line,
                                                   const MessageImportance
                                                   importance)
{
    myFile = file;
    myLine = line;
    myImportance = importance;
    return *this;
}

Ember::LoggingService & Ember::LoggingService::
slog (const MessageImportance importance)
{
    myImportance = importance;
    return *this;
}

Ember::LoggingService & Ember::LoggingService::slog (const std::string & file,
                                                   const MessageImportance
                                                   importance)
{
    myFile = file;
    myImportance = importance;
    return *this;
}

Ember::LoggingService & Ember::LoggingService::slog (const std::string & file,
                                                   const int line)
{
    myFile = file;
    myLine = line;
    return *this;
}

Ember::LoggingService & Ember::LoggingService::slog (const std::string & file)
{
    myFile = file;
    return *this;
}




void Ember::LoggingService::addObserver (Observer * observer)
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

int Ember::LoggingService::removeObserver (Observer * observer)
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

Ember::LoggingService::HexNumber Ember::LoggingService::
hexNumber (const int intDecimal)
{
    HexNumber intHex;
    intHex.myNumber = intDecimal;
    return intHex;
}

Ember::LoggingService & Ember::LoggingService::operator<< (const std::
                                                         string & stringToAdd)
{
    myMessage += stringToAdd;
    return *this;
}

Ember::LoggingService & Ember::LoggingService::operator<< (const int intToAdd)
{
    char buffer[NUMBER_BUFFER_SIZE];
    sprintf (buffer, "%d", intToAdd);
    myMessage += buffer;
    return *this;
}

Ember::LoggingService & Ember::LoggingService::
operator<< (const HexNumber & intHexToAdd)
{
    char buffer[NUMBER_BUFFER_SIZE];
    sprintf (buffer, "%x", intHexToAdd.myNumber);
    myMessage += buffer;
    return *this;
}

// Ember::LoggingService & Ember::LoggingService::
// operator<< (const double doubleToAdd)
// {
//     char buffer[NUMBER_BUFFER_SIZE];
//     sprintf (buffer, "%e", doubleToAdd);
//     myMessage += buffer;
//     return *this;
// }


void Ember::LoggingService::operator<< (const EndMessageEnum endMessage)
{
    sendMessage (myMessage, myFile, myLine, myImportance);

    myMessage = "";
    myFile = "";
    myLine = -1;
    myImportance = INFO;
}

Ember::LoggingService::LoggingService () : Service()
{
    //set service properties

    setName ("Logging");
    setDescription ("Eases message writing and distribution.");


    //set all option values to not specified
    myMessage = "";
    myFile = "";
    myLine = -1;
    myImportance = INFO;

    //Hook up to Eris's logging Service
    Eris::Logged.
        connect (SigC::slot (*this, &LoggingService::erisLogReceiver));
}

void Ember::LoggingService::erisLogReceiver (Eris::LogLevel level,
                                            const std::string & msg)
{
    MessageImportance importance;


    // Translate Eris importance's to ours

    switch (level)
    {
    case Eris::LOG_ERROR:
        importance = CRITICAL;
        break;
    case Eris::LOG_WARNING:
        importance = WARNING;
        break;
    case Eris::LOG_NOTICE:
      importance=INFO;
      break;
    case Eris::LOG_VERBOSE:
    default:
        importance = VERBOSE;
    }

    sendMessage (msg, "ERIS", 0, importance);
}


void Ember::LoggingService::sendMessage (const std::string & message,
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
