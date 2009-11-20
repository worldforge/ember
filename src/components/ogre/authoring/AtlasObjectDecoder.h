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

#ifndef ATLASOBJECTDECODER_H_
#define ATLASOBJECTDECODER_H_

#include <Atlas/Objects/Root.h>
#include <Atlas/Objects/SmartPtr.h>
#include <Atlas/Objects/Decoder.h>
#include <sigc++/signal.h>

namespace EmberOgre
{

namespace Authoring
{

/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 * @brief A simple Atlas decoder which will emit a signal each time a new object is decodes, as well as retain the last decoded object.
 * Use this when you want to decode atlas data into objects.
 */
class AtlasObjectDecoder : public Atlas::Objects::ObjectsDecoder
{
public:
	/**
	 * @brief Ctor.
	 */
	AtlasObjectDecoder();

	/**
	 * @brief Dtor.
	 */
	virtual ~AtlasObjectDecoder();

	/**
	 * @brief Gets the last decoded atlas object.
	 * @returns The last decoded atlas object.
	 */
	Atlas::Objects::Root getLastObject();

	/**
	 * @brief Emitted when an object has been decoded.
	 */
	sigc::signal<void, const Atlas::Objects::Root&> EventObjectArrived;

private:

    virtual void objectArrived(const Atlas::Objects::Root& obj);

    /**
     * @brief Retain a copy of the last decoded object.
     */
    Atlas::Objects::Root mLastObject;
};

}

}

#endif /* ATLASOBJECTDECODER_H_ */
