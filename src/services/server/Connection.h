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

#ifndef CONNECTION_H_
#define CONNECTION_H_

#include <Eris/Connection.h>

namespace Eris
{
class Session;
}

namespace Ember
{

struct IConnectionListener;

/**
 * @author Erik Ogenvik <erik@ogenvik.org>
 * @brief An extension of the base Eris connection type which will interact with IConnectionListener to allow for handing of objects being sent and received.
 */
class Connection: public Eris::Connection
{
public:

	/**
	 * @brief Ctor. for connecting to a remote host.
	 * @param clientName The name of this client.
	 * @param host The remote host.
	 * @param port The port on the remote host.
	 * @param debug Whether we should connect in debug mode.
	 * @param listener An optional listener. Ownership will be transferred to this instance.
	 */
	Connection(Eris::Session& session, const std::string &clientName, const std::string& host, short port, std::unique_ptr<IConnectionListener> listener);

	/**
	 * @brief Ctor. for connecting to a local socket.
	 * @param clientName The name of this client.
	 * @param socket Path to a local socket.
	 * @param debug Whether we should connect in debug mode.
	 * @param listener An optional listener. Ownership will be transferred to this instance.
	 */
	Connection(Eris::Session& session, const std::string &clientName, const std::string& socket, std::unique_ptr<IConnectionListener> listener);

	~Connection() override;

	void send(const Atlas::Objects::Root &obj) override;
protected:
	void objectArrived(const Atlas::Objects::Root& obj) override;

	/**
	 * @brief An optional listener.
	 */
	std::unique_ptr<IConnectionListener> mListener;

};

}

#endif /* CONNECTION_H_ */
