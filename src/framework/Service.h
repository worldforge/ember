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
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef SERVICE_H
#define SERVICE_H

// Include other headers of the current program here

// Include library headers here

// Include system headers here
#include <string>
#include <sigc++/trackable.h>
#include <sigc++/signal.h>

namespace Ember {

/**
 * @author Miguel Guzman Miranda
 */
class Service : public virtual sigc::trackable {
private:

	/** Stores the unique name of the service */
	const std::string mName;

	/** Tells if the service is running or not */
	bool mRunning;

	/** Don't allow copy constructor. */
	Service(const Service& source);

public:

	/** Creates a new Service using default values. */
	explicit Service(std::string name);

	/**
	 * @brief Dtor.
	 *
	 * If the service is running when being deleted it will first be stopped. Prevent this by calling stop() yourself.
	 */
	virtual ~Service();

protected:


	/** Specifies whether this service is currently running or not. */
	virtual void setRunning(bool running);

}; // Service

} // namespace Ember

#endif

