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
#include "framework/Service.h"

// Include library headers here

#include <string>
#include <list>
#include <stdio.h> 
#include <stdarg.h>
#include <ctime>
//#include <varargs.h> //TODO: Needed by unix?

using namespace std;

// Include system headers here

namespace dime {

//General TODOs:
// - make LoggingService threadsafe!
// - Is it necessary to support UNICODE/wide characters?
// - Add debug assertions. (How are the platform independent macros called?)




//======================================================================
// Short type macros
//======================================================================

#define ENDM LoggingService::END_MESSAGE;
#define HEX_NUM(number) LoggingService::hexNumber(number)


//TODO: Sorry, but innerclass didn't work properly
const int NUMBER_BUFFER_SIZE  = 24;
const int MESSAGE_BUFFER_SIZE = 1024;

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
 * using namespace dime::services;
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
	 * Abstract base class (=interface) for all observers
	 */
	class Observer 
	{
		public:	
		
		Observer::Observer()
		{
			myFilter = INFO; //No filtering assumed
		}

		/**
		 * Called every time a new message arrived at the LoggingService
		 *
		 * @param message The message string to send.
		 * @param file The source code file the message was initiated or empty if not specified.
		 * @param line The source code line the message was initiated or -1 if not specified.
		 * @param importance The level of importance (see MessageImportance enum)
		 * @param time_t The time the message was initiated.
		 */

		virtual void onNewMessage(const string & message, const string & file, const int & line,
			const MessageImportance & importance, const time_t & timeStamp) = 0;

		MessageImportance getFilter() 
		{
			return myFilter;
		}
	
		void setFilter(MessageImportance filter)
		{
			myFilter = filter;
		}

		private:

		/**
		 * A filter used by the LoggingService to determine wether the message should be send
		 * to onNewMessage. This happens only if the message is at least as important as
		 * the filter value.
		 */

		MessageImportance myFilter;
	};

	/**
	 * Predefined implementation of Observer-class handling FILE * 
	 *
	 * The format of messages written into a FILE * is the following:
	 * 
	 * timeStamp "\t" importance "\t" file "\t" line "\t" message
	 */

	class FileObserver: public Observer
	{
		public:
		FileObserver(FILE * file, MessageImportance filter)
		{
			setFilter(filter);
			myFile = file;
		}

		virtual void onNewMessage(const string & message, const string & file, const int & line, 
			const MessageImportance & importance, const time_t & timeStamp)
		{
			tm * ctm = localtime(&timeStamp); //currentLocalTime was too long, sorry

			fprintf(myFile, "[%04d-%02d-%02d %02d:%02d:%02d]\t%s\t%s\t%d\t%s\n",
				ctm->tm_year, ctm->tm_mon, ctm->tm_mday, ctm->tm_hour, ctm->tm_min, ctm->tm_sec,
				(importance == CRITICAL) ?  "CRITICAL" : 
					((importance == ERROR) ?  "ERROR" : 
						((importance == WARNING) ? "WARNING" : "INFO")),
				file.c_str(), line, message.c_str());
		}

		FILE * getFile()
		{
			return myFile;
		}

		private: 
		FILE * myFile;
	};

	private:

	typedef std::list<Observer *> ObserverList;
	
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

    //======================================================================
    // Private Variables
    //======================================================================
    private:

    /**
	 list of observers
     */
    ObserverList myObserverList;

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
    // Singleton

    /**
     * Returns the LoggingService instance.
     */
    static LoggingService *getInstance()
    {
		static LoggingService singleinstance;
		return &singleinstance;
    }

    //----------------------------------------------------------------------
    // Constructors
   
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
     * Adds a message presented by various options, a format string and variable params like
	 * in printf using also the same format specifications.
     *
     * @param file The source code file the message was initiated.
	 * @param line The source code line the message was initiated.
	 * @param importance The level of importance (see MessageImportance enum)
	 * @param message The message format string.
     *
	 */

	 //ATTENTION (by tim): I've changed this back to my initial version, because the presented
	 // ones didn't compile using MSVC. Is there any problem with this on 

	void log(const string message, ...) 
	{
	    va_list vl;
		va_start(vl, message);
		logVarParam("", -1, INFO, message, vl);
		va_end(vl);
	}

	void log(const string & file, const string message, ...)
	{
		va_list vl;
		va_start(vl, message);
		logVarParam(file, -1, INFO, message, vl);
		va_end(vl);
	}

	void log(const string & file, const int line, const string  message, ...)
	{
		va_list vl;
		va_start(vl, message);
		logVarParam(file, line, INFO, message, vl);
		va_end(vl);
	}

	void log(const MessageImportance importance, const string message, ...)
	{
		va_list vl;
		va_start(vl, message);
		logVarParam("", -1, importance, message, vl);
		va_end(vl);
	}

	void log(const string & file, const MessageImportance importance, 
					 const string message, ...)
	{
		va_list vl;
		va_start(vl, message);
		logVarParam(file, -1, importance, message, vl);
		va_end(vl);
	}

	void log(const string & file, const int line, const MessageImportance importance, 
					const string message, ...)
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
		vsprintf((char*)Buffer, message.c_str(), argptr);
		
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
	 * Adds an observer to the list. 
	 *
	 * @param observer Pointer to an object with an Observer interface to be added to the list.
	 */

	void addObserver(Observer * observer)
	{
		
		//test on already existing observer

		for (ObserverList::iterator i = myObserverList.begin(); 
			i != myObserverList.end(); i++)
		{
			if (*i == observer)
			{
				return;
			}
		}
		
		//no existing observer, add a new

		myObserverList.push_back(observer);
	}
	
	/**
	 * Removes an observer from the list. 
	 *
	 * @param observer The pointer previously added the the observer list via addObserver.
     *
	 * @return 0 if the observer was found and removed, <> 0 otherwise
	 */
	
	int removeObserver(Observer * observer)
	{
		for (ObserverList::iterator i = myObserverList.begin(); 
			i != myObserverList.end(); i++)
		{
			if (*i == observer)
			{
				myObserverList.erase(i);
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
		sprintf(buffer, "%d", intToAdd);
		myMessage += buffer;
		return *this;
	}

	LoggingService & operator<< (const HexNumber & intHexToAdd)
	{
		char buffer[NUMBER_BUFFER_SIZE];
		sprintf(buffer, "%x", intHexToAdd);
		myMessage += buffer;
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

	//----------------------------------------------------------------------
    // Constructors
	
	/**
     * Creates a new LoggingService using default values.
	 * Private so LoggingService can only be initialized through getInstance.
     */
    LoggingService()
    {

		//set service properties
		setName("Logging");
		setDescription("Eases message writing and distribution.");
		

		//set all option values to not specified
		myMessage			= "";
		myFile				= "";
		myLine				= -1;
		myImportance		= INFO;
    }
	
	//----------------------------------------------------------------------
    // Other private methods

	/**
	 * Unifies the sending mechanism for streaming- and formatting-input
	 */

	virtual void sendMessage(const string & message, 
		const string & file, const int line, const MessageImportance importance)
	{
		time_t currentTime;
		time(&currentTime);

		for (ObserverList::iterator i = myObserverList.begin(); 
			i != myObserverList.end(); i++)
		{
			if ((int)importance >= (int)(*i)->getFilter())
			{
				(*i)->onNewMessage(message, file, line, importance, currentTime);
			}
		}
	}
							

}; // LoggingService

} // namespace dime

#endif
