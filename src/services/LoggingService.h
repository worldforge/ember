/*
    Copyright (C) 2002  Tim Enderling

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
#include "Service.h"

// Include library headers here

#include <string>
#include <list>
#include <stdio.h> 
#include <stdarg.h>
#include <ctime>
//#include <varargs.h> //Needed by unix?

using namespace std;

// Include system headers here

namespace dime {
namespace services {

//General TODOs:
// - resolve minor TODOs
// - test if it compiles under Unix especially
// - test if it works
// - make LoggingService threadsafe!
// - Is it necessary to support UNICODE/wide characters?
// - Add debug assertions. (How are the platform independent macros called?)


/**
 * Easy-to-deal-with logging class.
 *
 * This service class should make adding and observing logging messages more easy. This
 * can be done using printf-like log method or just streaming operator << in
 * cout-like way. 
 *
 * The cout-like way always needs a LoggingService::END_MESSAGE to be streamed-in 
 * last (use ENDM* macro). To specify formats like hexadecimal printing static functions
 * are available for conversion (use the HEX_NUM* macro).
 * 
 * NOTE: Since streaming actually constist of many operations that can be interrupted by
 * switching the active thread, streaming should only be used in thread safe areas. [TODO: How
 * to fix this unwanted behaviour?]
 *
 *
 * There are some log variants for which you can specify some options:
 * - source file (use __FILE__) 
 * - source line (use __LINE__)
 * - level of importance (see enum called MessageImportance), always INFO if not specified
 *
 * As a special feature you can use a function called slog * (stands for stream log) that can
 * be used for setting the options before using streaming. (See example.)  
 *
 * Callback observers of logging process can easily be managed 
 * using addObserver and removeObserver. FILE * can be used as a special kind
 * of observer where all messages are written in including optional fields separated by "\t".
 * To less the amount of messages passed through to the observers, you can specify a filter by
 * levels of importance. Thus all messages above or equal a filter level of importance are
 * written/passed by the callback to an observer.
 * 
 * 
 * HINT: Names marked with * were chosen this short, because they are intentended to be used very 
 * frequently.
 *
 * SAMPLE:
 * using namespace dime::services;
 * LoggingService * logger;
 * //service is assumed to be started
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

//======================================================================
// Short type macros
//======================================================================

#define ENDM LoggingService::END_MESSAGE;
#define HEX_NUM(number) LoggingService::hexNumber(number)
	

class LoggingService: public Service
{
    //======================================================================
    // Public Constants and enums
    //======================================================================
    public:	

	/**
	 * This enum contains all levels of message importance.
	 * -INFO messages are intended to be read only to look for reasons of errors
	 * -WARNING messages appear whenever something could get critical in some case
	 * -CRITICAL messages should be read always and contain fatal errors
	 */
	enum MessageImportance
	{
		INFO		= 0,
		WARNING		= 1,
		ERROR		= 2,
		CRITICAL	= 3
	};


	/**
	 * Pseudo-enum necessary to make the END_MESSAGE constant not be mixed with ints
	 */
	enum EndMessageEnum
	{
		END_MESSAGE = 0
	};

	//======================================================================
    // Inner Classes and typedefs
    //======================================================================
	 
	public:
	/**
	 * Defines the look of a callback function used by an observer. 
	 *
	 * @param cockie The cockie given to addObserver (e.g. a this pointer)
	 * @param message The message string to send.
     * @param file The source code file the message was initiated or empty if not specified.
	 * @param line The source code line the message was initiated or -1 if not specified.
	 * @param importance The level of importance (see MessageImportance enum)
	 * @param time_t The time the message was initiated.
	 */

	typedef void (* OBSERVER_CALLBACK) ( void * cockie, const string & message, const string & file,
			const int & line, const MessageImportance & importance, const time_t & timeStamp);
	
	private:
	
	struct FileObserver
	{
		FILE *			  myFile;
		MessageImportance myFilter;
	};

	struct CallbackObserver
	{
		OBSERVER_CALLBACK myCallbackFunction;
		void *			  myCockie;
		MessageImportance myFilter;
	};

	typedef std::list<CallbackObserver>  CallbackObserverList;
	typedef std::list<FileObserver>		 FileObserverList;
	
	/**
	* pseudo-type used for multiple overriding with the same type of the operator<<
	*/

	struct HexNumber
	{
		int myNumber;
	};

    //======================================================================
    // Private Constants
    //======================================================================
    private:
	static const int NUMBER_BUFFER_SIZE = 20;
	static const int MESSAGE_BUFFER_SIZE = 1024;

    //======================================================================
    // Private Variables
    //======================================================================
    private:

    /**
	 list of Callback observers (via OBSERVER_CALLBACK)
     */
    CallbackObserverList myCallbackObserverList;

	/**
	 list of FILE * observers
     */
	FileObserverList myFileObserverList;

	/**
	 currently given part of the message string (used by << streaming only)
	 */
	string myMessage;
	
	/**
	 * currently set source file (option; used by << streaming only)
	 * An empty string indicates that no file option was set.
	 */
	string myFile;

	/**
	 * currently set source code line (option; used by << streaming only)
	 * -1 indicates that no line option was set.
	 */
	int	myLine;

	/**
	 * currently set importance (option; used by << streaming only)
	 * The default value is INFO.
	 */
	MessageImportance myImportance;


    //======================================================================
    // Public Methods
    //======================================================================
    public:

    //----------------------------------------------------------------------
    // Constructors

    /**
     * Creates a new LoggingService using default values.
     */
    LoggingService()
    {
		//set service properties
		setName("Logging");
		setDescription("Helps to ease message writing and distribution.");
		

		//set all option values to not specified
		myMessage			= "";
		myFile				= "";
		myLine				= -1;
		myImportance		= INFO;
    }
   
	/* NOTE: No copy constructor available (needed anywhere?)
	LoggingService( const LoggingService &source )
    {
        // Use assignment operator to do the copy
        // NOTE: If you need to do custom initialization in the constructor this may not be enough.
        *this = source;
    }

	LoggingService &operator= ( const LoggingService &source )
    {
        // Copy fields from source class to this class here.

        // Return this object with new value
        return &this;
    }*/


    //----------------------------------------------------------------------
    // Destructor

    /**
     * Deletes a LoggingService instance.
     */
    virtual ~LoggingService()
    {
        //all freeing of memory is done in subsequent member destructors
    }


    //----------------------------------------------------------------------
    // Getters

    //----------------------------------------------------------------------
    // Setters

    //----------------------------------------------------------------------
    // Other public methods

	 virtual int start()
	 {
		 setRunning(true);
		 return 0;
	 }

    /**
     * [General documenting TODO: How does doxygen handle function overriding?]
	 *
	 * Adds a message presented by various options, a format string and variable params like
	 * in printf using also the same format specifications.
     *
     * @param file The source code file the message was initiated.
	 * @param line The source code line the message was initiated.
	 * @param importance The level of importance (see MessageImportance enum)
	 * @param message The message format string.
     *
	 */

	void log(const string & message, ...) 
	{
	    va_list vl;
		va_start(vl, message);
		logVarParam("", -1, INFO, message, vl);
		va_end(vl);
	}

	void log(const string & file, const string & message, ...)
	{
		va_list vl;
		va_start(vl, message);
		logVarParam(file, -1, INFO, message, vl);
		va_end(vl);
	}

	void log(const string & file, const int line, const string & message, ...)
	{
		va_list vl;
		va_start(vl, message);
		logVarParam(file, line, INFO, message, vl);
		va_end(vl);
	}

	void log(const MessageImportance importance, const string & message, ...)
	{
		va_list vl;
		va_start(vl, message);
		logVarParam("", -1, importance, message, vl);
		va_end(vl);
	}

	void log(const string & file, const MessageImportance importance, 
					 const string & message, ...)
	{
		va_list vl;
		va_start(vl, message);
		logVarParam(file, -1, importance, message, vl);
		va_end(vl);
	}

	void log(const string & file, const int line, const MessageImportance importance, 
					const string & message, ...)
	{
		va_list vl;
		va_start(vl, message);
		logVarParam(file, line, importance, message, vl);
		va_end(vl);
	}
	
	/**
	 * Is actually used in all cases of log. (Generates the message and then uses sendMessage.)
	 * @see log
	 */

	void logVarParam(const string & file, const int line, const MessageImportance importance, 
								const string & message, va_list argptr)
	{
		char Buffer[MESSAGE_BUFFER_SIZE];
		vsprintf((char*)Buffer, message.data(), argptr);
		
		sendMessage(string((char*)Buffer), file, line, importance);
	}

	/**
	 * Gives the possibility to specify options when using the streaming method << for messages.
	 * 
     * @param file The source code file the message was initiated.
	 * @param line The source code line the message was initiated.
	 * @param importance The level of importance (see MessageImportance enum)
	 *
	 * @return This function always returns a reference to the LoggingService object. You
	 * can thus easily apply the shifting operator to it.
	 */

	LoggingService & slog(const string & file, const int line, const MessageImportance importance)
	{
		myFile = file;
		myLine = line;
		myImportance = importance;
		return *this;
	}
	
	LoggingService & slog(const MessageImportance importance)
	{
		myImportance = importance;
		return *this;
	}

	LoggingService & slog(const string & file, const MessageImportance importance)
	{
		myFile = file;
		myImportance = importance;
		return *this;
	}

	LoggingService & slog(const string & file, const int line)
	{
		myFile = file;
		myLine = line;
		return *this;
	}

	LoggingService & slog(const string & file)
	{
		myFile = file;		
		return *this;
	}

	/**
	 * Adds an observer to the list. This can be a Callback observer (in case an OBSERVER_CALLBACK
	 * parameter is given) or a FILE * callback (in case a FILE * parameter is given).
	 *
	 * @param callback The function to be called back when a new message arrived.
	 * @param file The file to write in when a new message arrived.
	 * @param cockie A pointer to anything useful for the callback function.
	 * @param filter The minimum level of importance needed for messages that should be passed through
	 *               to the observer.
	 *
	 */

	void addObserver(FILE * file, MessageImportance filter)
	{
		FileObserver fileObserver;
		
		fileObserver.myFile		= file;
		fileObserver.myFilter	= filter;
		
		//TODO: Should we test on already existing observers?

        //NOTE(zzorn, 2002-01-21): Perhaps we could do that, and just change the message 
        //                         importance filter setting if the listener already exists?

		myFileObserverList.push_back(fileObserver);
	}

	void addObserver(OBSERVER_CALLBACK callback, void * cockie,
                      MessageImportance filter)
	{
		CallbackObserver callbackObserver;
		
		callbackObserver.myCallbackFunction = callback;
		callbackObserver.myCockie			= cockie;
		callbackObserver.myFilter			= filter;

		//TODO: Should we test on already existing observers?

        //NOTE(zzorn, 2002-01-21): Perhaps we could do that, and just change the message 
        //                         importance filter setting if the listener already exists?

		myCallbackObserverList.push_back(callbackObserver);
	}
	
	/**
	 * Removes an observer to the list. This can be a Callback observer (in case an OBSERVER_CALLBACK
	 * parameter is given) or a FILE * callback (in case a FILE * parameter is given).
	 *
	 * @param callback The function previously registered to be called back when a new message arrived.
	 * @param file The file previously registered to write in when a new message arrived.
     *
	 * @return 0 if the observer was found and removed, <> 0 otherwise
	 */
	
	int removeObserver(FILE * file)
	{
		for (FileObserverList::iterator i = myFileObserverList.begin(); 
			i != myFileObserverList.end(); i++)
		{
			if (i->myFile == file)
			{
				myFileObserverList.erase(i);
				return 0;
			}
		}

		return -1;
	}

	int removeObserver(OBSERVER_CALLBACK callback)
	{
		for (CallbackObserverList::iterator i = myCallbackObserverList.begin(); 
			i != myCallbackObserverList.end(); i++)
		{
			if (i->myCallbackFunction == callback)
			{
				myCallbackObserverList.erase(i);
				return 0;
			}
		}

		return -1;
	}
	
	//----------------------------------------------------------------------
	// shifting operator and helper functions

	/**
	 * Converts a normal int to a hexadecimal int that can be streamed into the
	 * LoggingService object. (use HEX_NUM macro if you want)
	 */
	
	static HexNumber hexNumber(const int intDecimal)
	{
		HexNumber intHex;
		intHex.myNumber = intDecimal;
		return intHex;
	}

	LoggingService & operator<< (const string & stringToAdd)
	{
		myMessage += stringToAdd;
		return *this;
	}

	LoggingService & operator<< (const int intToAdd)
	{
		char buffer[NUMBER_BUFFER_SIZE];
		myMessage += itoa(intToAdd, (char*)buffer, 10);
		return *this;
	}

	LoggingService & operator<< (const HexNumber & intHexToAdd)
	{
		char buffer[NUMBER_BUFFER_SIZE];
		myMessage += itoa(intHexToAdd.myNumber, (char*)buffer, 16);
		return *this;
	}

	/**
	 * By streaming in END_MESSAGE (equally to ENDM-macro) you finish the message and start
	 * sending it.
	 */

	void operator<< (const EndMessageEnum endMessage)
	{
		sendMessage(myMessage, myFile, myLine, myImportance);
		
		myMessage	 = "";
		myFile		 = "";
		myLine		 = -1;
		myImportance = INFO;
	}

    //======================================================================
    // Protected Methods
    //======================================================================
    protected:


    //======================================================================
    // Private Methods
    //======================================================================
    private:

	/**
	 * Unifies the sending mechanism for streaming- and formatting-input
	 */

	virtual void sendMessage(const string & message, 
		const string & file, const int line, const MessageImportance importance)
	{
		time_t currentTime;
		time(&currentTime);

		tm * ctm = localtime(&currentTime); //currentLocalTime was too long, sorry

		//Separate them by "\t"

		//Order: currentTime importance file line message

		char buffer[MESSAGE_BUFFER_SIZE];

        // NOTE(zzorn, 2002-01-21): Changed to snprintf to avoid buffer overruns.
        //                          The additional - 1 in the buffer size is just some paranoia of mine.
		int bufferLength = snprintf((char*)buffer, MESSAGE_BUFFER_SIZE - 1,
			"[%04d-%02d-%02d %02d:%02d:%02d]\t%s\t%s\t%d\t%s\n",
			ctm->tm_year, ctm->tm_mon, ctm->tm_mday, ctm->tm_hour, ctm->tm_min, ctm->tm_sec,
			(importance == CRITICAL) ?  "CRITICAL" : 
				((importance == ERROR) ?  "ERROR" : 
					((importance == WARNING) ? "WARNING" : "INFO")),
			file, line, message);


		for (CallbackObserverList::iterator i = myCallbackObserverList.begin(); 
			i != myCallbackObserverList.end(); i++)
		{
			if ((int)importance >= (int)i->myFilter)
			{
				i->myCallbackFunction(i->myCockie, message, file, line, importance, currentTime);
			}
		}

		for (FileObserverList::iterator j = myFileObserverList.begin();
			j != myFileObserverList.end(); j++)
		{
			
			if ((int)importance >= (int)j->myFilter)
			{
				fwrite((char*)buffer, sizeof(char), bufferLength, j->myFile);
			}
		}
	}
							

}; // End of class

} // End of subsystem namespace
} // End of application namespace

#endif
