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

#include "Connection.h"
#include "IConnectionListener.h"
#include "framework/LoggingInstance.h"

#include "framework/Session.h"
namespace Ember
{
Connection::Connection(Session& session, const std::string& clientName, const std::string& host, short port, std::unique_ptr<IConnectionListener> listener) :
		Eris::Connection(session.m_io_service, session.m_event_service, clientName, host, port),
		mListener(std::move(listener))
{
}

Connection::Connection(Session& session, const std::string& clientName, const std::string& socket, std::unique_ptr<IConnectionListener> listener) :
		Eris::Connection(session.m_io_service, session.m_event_service, clientName, socket),
		mListener(std::move(listener))
{
}

Connection::~Connection() = default;

void Connection::send(const Atlas::Objects::Root &obj)
{
	if (mListener) {
		try {
			mListener->sendingObject(obj);
		} catch (const std::exception& ex) {
			S_LOG_WARNING("Error when logging sending of object."<< ex);
		}
	}
	Eris::Connection::send(obj);
}

void Connection::objectArrived(Atlas::Objects::Root obj)
{
	if (mListener) {
		try {
			mListener->receivedObject(obj);
		} catch (const std::exception& ex) {
			S_LOG_WARNING("Error when logging receiving of object."<< ex);
		}
	}
	Eris::Connection::objectArrived(std::move(obj));
}

}
