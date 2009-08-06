/*
 Copyright (C) 2009 Erik Hjortsberg

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

#ifndef CONNECTION_H_
#define CONNECTION_H_

#include <Eris/Connection.h>

namespace Ember
{

class IConnectionListener;

/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 * @brief An extension of the base Eris connection type which will interact with IConnectionListener to allow for handing of objects being sent and received.
 */
class Connection: public Eris::Connection
{
public:
	/**
	 * @brief Ctor.
	 * @param cnm
	 * @param host
	 * @param port
	 * @param debug
	 * @param listener An optional listener. Ownership will be transferred to this instance.
	 */
	Connection(const std::string &cnm, const std::string& host, short port, bool debug, IConnectionListener* listener);
	virtual ~Connection();

	virtual void send(const Atlas::Objects::Root &obj);
protected:
	virtual void objectArrived(const Atlas::Objects::Root& obj);

	/**
	 * @brief An optional listener.
	 * Owned by this instance.
	 */
	IConnectionListener* mListener;

};

}

#endif /* CONNECTION_H_ */
