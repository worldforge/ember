/*
 Copyright (C) 2002  Miguel Guzman Miranda [Aglanor]

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

#ifndef SERVICE_H
#define SERVICE_H

// Include other headers of the current program here

// Include library headers here

// Include system headers here
#include <string>
#include <sigc++/trackable.h>
#include <sigc++/signal.h>

namespace Ember
{

/**
 * @author Miguel Guzman Miranda
 */
class Service: public virtual sigc::trackable
{
	//======================================================================
	// Public Constants
	//======================================================================
public:

	/**
	 * Contains the different error states that a service can be in.  <p>
	 * <ul>
	 * <li> OK          - Service working normally.  </li>
	 * <li> NOTE        - Service working, but there is something that may need user attention.  </li>
	 * <li> WARNING     - Service is working to some degree, but there was some problems encountered, or non-critical parts that are not working.  </li>
	 * <li> FAILURE     - Service is not working.  A resource it depends on is not present, or the service is unimplemented.  </li>
	 * <li> CRITICAL    - Service detected internal errors in itself or the system that can lead to data loss or other serious problems.  </li>
	 * </ul>
	 *
	 * (The difference between FAILURE and CRITICAL is basically that FAILURE
	 * means that the rest of the system may still work fine, while CRITICAL means
	 * that the whole system is likely to be unstable.)
	 */
	enum Status
	{
		OK = 0,
		NOTE = 1,
		WARNING = 2,
		FAILURE = 3,
		CRITICAL = 4
	};

private:

	/** Stores the unique name of the service */
	std::string myName;

	/** Stores the description of the service */
	std::string myDescription;

	/** Tells if the service is running or not */
	bool myRunning;

	/** Current status code */
	Status myStatus;

	/** Textual description of the current status, especially if it is some problem. */
	std::string myStatusText;

	/** Don't allow copy constructor. */
	Service(const Service &source);

public:

	/** Creates a new Service using default values. */
	Service();

	/**
	 * @brief Dtor.
	 *
	 * If the service is running when being deleted it will first be stopped. Prevent this by calling stop() yourself.
	 */
	virtual ~Service();

	/** Returns the name of this Service. */
	virtual const std::string& getName() const;

	/** Returns the description of this Service. */
	virtual const std::string& getDescription() const;

	/** Returns the status of this Service. */
	virtual Service::Status getStatus() const;

	/** Returns true if the service is currently running. */
	virtual bool isRunning() const;

	/**
	 * Returns the textual status message for this Service.
	 * It is a description of the current status,
	 * especially if there is some problem.
	 * (If everything is fine, then this can be empty, as the status code can be
	 * used to determine this.)
	 */
	virtual const std::string& getStatusText() const;

	/**
	 * This method is used to start the service.
	 * It should take care of aquiring needed resources, initializing
	 * data structures, and so on. <p>
	 *
	 * If the initialization suceeds, it should also call setRunning( true )
	 * to indicate that the service is running.  <p>
	 *
	 * If initialization fails, it should set appropriate status code and
	 * status text.  It could also write an entry into a log through the logging
	 * service.  <p>
	 *
	 * This method must be implemented by all inheriting classes.  <p>
	 *
	 *
	 * @returns success or error code
	 */
	virtual Status start() = 0;

	/**
	 * This method stops the service, and frees any used resources.
	 * If the service has no special resources that need to be freed,
	 * or de-initialization to be done, this
	 * method can be left to the default implementation (which just sets the
	 * running state to false).  <p>
	 *
	 * Otherwise this method should be overridden, and setRunning( false )
	 * should be called if the service was stopped.  <p>
	 *
	 * @ param code code which represents the cause of the service halt
	 * TODO(zzorn): What do we need it for?
	 */
	virtual void stop(int code);

	/**
	 * Emitted when the server has stopped.
	 */
	sigc::signal<void> EventStopping;

protected:

	/** Sets the name of this Service. */
	virtual void setName(const std::string& name);

	/** Sets the description of this Service. */
	virtual void setDescription(const std::string& description);

	/**
	 * Sets the description of this Service.
	 */
	virtual void setStatus(Service::Status status);

	/** Specifies wether this service is currently running or not. */
	virtual void setRunning(bool running);

	/**
	 * Sets the textual status message for this Service.
	 * It is a description of the current status,
	 * especially if there is some problem.
	 * (If everything is fine, then this can be empty, as the status code can be
	 * used to determine this.)
	 */
	virtual void setStatusText(const std::string& statusText);

}; // Service


inline Service::Service() :
	myStatus(OK), myRunning(false)
{
}

inline Service::~Service()
{
	if (myRunning) {
		stop(0);
	}
}

inline const std::string& Service::getName() const
{
	return myName;
}

inline const std::string& Service::getDescription() const
{
	return myDescription;
}

inline Service::Status Service::getStatus() const
{
	return myStatus;
}

inline bool Service::isRunning() const
{
	return myRunning;
}

inline const std::string& Service::getStatusText() const
{
	return myStatusText;
}

inline void Service::stop(int code)
{
	EventStopping.emit();
	setRunning(false);
}

inline void Service::setName(const std::string& name)
{
	myName = name;
}

inline void Service::setDescription(const std::string& description)
{
	myDescription = description;
}

inline void Service::setStatus(Service::Status status)
{
	myStatus = status;
}

inline void Service::setRunning(bool running)
{
	myRunning = running;
}

inline void Service::setStatusText(const std::string& statusText)
{
	myStatusText = statusText;
}

} // namespace Ember

#endif

