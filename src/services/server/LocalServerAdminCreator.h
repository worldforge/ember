/*
 Copyright (C) 2013 Erik Ogenvik

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

#ifndef LOCALSERVERADMINCREATOR_H_
#define LOCALSERVERADMINCREATOR_H_

#include <Atlas/Objects/ObjectsFwd.h>
#include <sigc++/trackable.h>

namespace Eris
{
class Account;
}

namespace Ember
{
class ServerService;

/**
 * @author Erik Ogenvik
 * @brief Handles the flow of creating a "sys" account and logging in the user.
 *
 * This is meant to be used when the connected to a local server through a socket
 * and the user wants to enter the world as a creator entity.
 *
 * Note that you need to delete any instance as soon as the user has logged in to
 * prevent it from interacting with any other attempt to log in to a server.
 */
class LocalServerAdminCreator: public sigc::trackable
{
public:
	/**
	 * @brief Ctor.
	 * @param serverService The server service.
	 */
	explicit LocalServerAdminCreator(ServerService& serverService);

	/**
	 * @brief Dtor.
	 */
	~LocalServerAdminCreator() = default;
private:

	/**
	 * @brief The server service.
	 */
	ServerService& mServerService;

	void server_GotAccount(Eris::Account* account);
	void server_LoginSuccess(Eris::Account* account);

};

}
#endif /* LOCALSERVERADMINCREATOR_H_ */
