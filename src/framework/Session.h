/*
 Copyright (C) 2014 Erik Ogenvik

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

#ifndef SESSION_H_
#define SESSION_H_

#include <Eris/EventService.h>
#include <boost/asio/io_service.hpp>
#include <boost/noncopyable.hpp>
#include <memory>

namespace Ember {

/**
 * @brief Acts as a single entrypoint to Eris.
 *
 * This class wraps up both an instance of boost::asio::io_service as well as
 * an instance of EventService. These two classes are the two required classes for
 * the Eris system to work.
 *
 * Your application should create an instance of this to use throughout the application's
 * life.
 */
struct Session : boost::noncopyable {
	Session() : m_event_service(m_io_service) {
	}

	boost::asio::io_service m_io_service;
	Eris::EventService m_event_service;

};

}
#endif /* SESSION_H_ */
