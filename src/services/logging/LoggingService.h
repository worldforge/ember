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
#include <memory>
#include <sigc++/object.h>

#ifdef WIN32
    // These are needed by mingw
    #include <ctime>
    #include <stdarg.h>
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
	private:

	/**
	* pseudo-type used for multiple overriding with the same type of the operator<<
	*/
	struct HexNumber
	{
		int myNumber;
	};

	public:
	friend class ErisLogReciever;
	friend class LoggingInstance;
	
	static const int NUMBER_BUFFER_SIZE = 24;
	static const int MESSAGE_BUFFER_SIZE = 4096;
	
	/**
	* @brief This enum contains all levels of message importance.
	* -VERBOSE messages are for maxiumum level of verboseness and are emitted frequently with details of Ember's internal state.
	* -INFO messages are intended to be read only to look for reasons of errors.
	* -WARNING messages appear whenever something could get critical in some case.
	* -CRITICAL messages should be read always and contain fatal errors.
	*/
	enum MessageImportance
	{
		VERBOSE = 0,
		INFO = 1,
		WARNING = 2,
		FAILURE = 3,
		CRITICAL = 4
	};


	/**
	* Pseudo-enum necessary to make the END_MESSAGE constant not be mixed with ints
	*/
	enum EndMessageEnum
	{
		END_MESSAGE = 0
	};


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
	return *this;
	} */

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

	/**
	* @brief Adds a message presented by various options, a format string and variable params like in printf using also the same format specifications.
	*
	* @param file The source code file the message was initiated.
	* @param line The source code line the message was initiated.
	* @param importance The level of importance (see MessageImportance enum)
	* @param message The message format string.
	*
	*/
	void log(const char *message, ...);

	void log(const char *file, const char *message, ...);

	void log(const char *file, const int line, const char *message, ...);

	void log(const MessageImportance importance, const char *message, ...);

	void log(const char *file, const MessageImportance importance, const char *message, ...);

	void log(const char *file, const int line, const MessageImportance importance, const char *message, ...);


	/**
	*  @brief Is actually used in all cases of log. (Generates the message and then uses sendMessage.)
	* @see log
	*/
	void logVarParam(const char *file, const int line, const MessageImportance importance, const char *message, va_list argptr);

	/**
	* @brief Gives the possibility to specify options when using the streaming method << for messages.
	*
	* @param file The source code file the message was initiated.
	* @param line The source code line the message was initiated.
	* @param importance The level of importance (see MessageImportance enum)
	*
	* @return This function always returns a reference to a LoggingInstance object. You
	* can thus easily apply the shifting operator to it.
	*/

	LoggingInstance slog(const std::string & file, const int line, const MessageImportance importance);

	LoggingInstance slog(const MessageImportance importance);

	LoggingInstance slog(const std::string & file, const MessageImportance importance);

	LoggingInstance slog(const std::string & file, const int line);

	LoggingInstance slog(const std::string & file);

	/**
	* @brief Adds an observer to the list.
	*
	* @param observer Pointer to an object with an Observer interface to be added to the list.
	*/
	void addObserver(Observer * observer);


	/**
	* @brief Removes an observer from the list.
	*
	* @param observer The pointer previously added the the observer list via addObserver.
	* @return 0 if the observer was found and removed, <> 0 otherwise
	*/
	int removeObserver(Observer * observer);

	/**
	* @brief Converts a normal int to a hexadecimal int that can be streamed into the LoggingService object. (use HEX_NUM macro if you want)
	*/
	static HexNumber hexNumber(const int intDecimal);

private:

	typedef std::list<Observer*> ObserverList;

	/**
	list of observers
	*/
	ObserverList myObserverList;


	std::auto_ptr<ErisLogReciever> mErisLogReciever;

	/**
	* Unifies the sending mechanism for streaming- and formatting-input
	*/
	virtual void sendMessage (const std::string & message, const std::string & file, const int line, const MessageImportance importance);

};// LoggingService


/**
@brief A logging instance.
Use the << methods to write log message to the instance.
Send LoggingService::EndMessageEnum to it to make it write its contents to the service. If nothing has been written at the time of deletion it will be written then.
*/
class LoggingInstance
{
public:
	friend class LoggingService;

	LoggingInstance(LoggingService& service, const std::string & file, const int line, const LoggingService::MessageImportance importance);

	LoggingInstance(LoggingService& service, const LoggingService::MessageImportance importance);

	LoggingInstance(LoggingService& service, const std::string & file, const LoggingService::MessageImportance importance);

	LoggingInstance(LoggingService& service, const std::string & file, const int line);

	LoggingInstance(LoggingService& service, const std::string & file);
	
	
	LoggingInstance& operator<< (const std::string & stringToAdd);

	LoggingInstance& operator<< (const int intToAdd);

	LoggingInstance& operator<< (const unsigned int uintToAdd);

	LoggingInstance& operator<< (const long longToAdd);

	LoggingInstance& operator<< (const unsigned long ulongToAdd);

	LoggingInstance& operator<< (const LoggingService::HexNumber & intHexToAdd);

	LoggingInstance& operator<< (const double doubleToAdd);
	
	/**
	 * @brief At destruction the message will be written, if not already done.
	 */
	~LoggingInstance();

	/**
	* By streaming in END_MESSAGE (equally to ENDM-macro) you finish the message and start
	* sending it.
	*/
	void operator<< (const LoggingService::EndMessageEnum endMessage);

private:

	/**
	 * @brief This is private to prevent copying.
	 * @param l 
	 */
	LoggingInstance(const LoggingInstance& l);

	/**
	 * @brief The service to which this instance is bound.
	 */
	LoggingService& mService;

	/**
	* currently set source file (option; used by << streaming only)
	* An empty string indicates that no file option was set.
	*/
	std::string mFile;

	/**
	* currently set source code line (option; used by << streaming only)
	* -1 indicates that no line option was set.
	*/
	int mLine;
	

	/**
	* currently set importance (option; used by << streaming only)
	* The default value is INFO.
	*/
	LoggingService::MessageImportance mImportance;
	
	/**
	currently given part of the message string (used by << streaming only)
	*/
	std::string mMessage;
	
};

}// namespace Ember

#endif
