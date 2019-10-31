/*
 Copyright (C) 2009 Erik Ogenvik

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

#ifndef ICONNECTIONLISTENER_H_
#define ICONNECTIONLISTENER_H_

#include <Atlas/Objects/ObjectsFwd.h>

namespace Ember
{

/**
 * @author Erik Ogenvik <erik@ogenvik.org>
 * @brief Listens for objects sent and received through a Connection.
 */
struct IConnectionListener
{

	virtual ~IConnectionListener() = default;

	/**
	 * @brief Called when an object is about to be sent.
	 * @param obj The object to send.
	 */
	virtual void sendingObject(const Atlas::Objects::Root& obj) = 0;

	/**
	 * @brief Called when an object has been received.
	 * @param obj The object received.
	 */
	virtual void receivedObject(const Atlas::Objects::Root& obj) = 0;

};

}
#endif /* ICONNECTIONLISTENER_H_ */
