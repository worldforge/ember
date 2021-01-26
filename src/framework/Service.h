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

#include <boost/noncopyable.hpp>
#include <string>
#include <sigc++/trackable.h>
#include <sigc++/signal.h>

namespace Ember {

/**
 * @author Miguel Guzman Miranda
 */
class Service : public virtual sigc::trackable, public boost::noncopyable {
private:

	/** Stores the unique name of the service */
	const std::string mName;

public:

	/** Creates a new Service using default values. */
	explicit Service(std::string name);

	virtual ~Service() = default;



}; // Service

} // namespace Ember

#endif

