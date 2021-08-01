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

#ifndef ATLASOBJECTDECODER_H_
#define ATLASOBJECTDECODER_H_

#include <Atlas/Objects/Root.h>
#include <Atlas/Objects/SmartPtr.h>
#include <Atlas/Objects/Decoder.h>
#include <sigc++/signal.h>

namespace Ember
{

/**
 * @author Erik Ogenvik <erik@ogenvik.org>
 * @brief A simple Atlas decoder which will emit a signal each time a new object is decodes, as well as retain the last decoded object.
 * Use this when you want to decode atlas data into objects.
 */
class AtlasObjectDecoder : public Atlas::Objects::ObjectsDecoder
{
public:
	/**
	 * @brief Ctor.
	 */
	explicit AtlasObjectDecoder(const Atlas::Objects::Factories& factories);

	/**
	 * @brief Dtor.
	 */
	~AtlasObjectDecoder() override;

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

    void objectArrived(Atlas::Objects::Root obj) override;

    /**
     * @brief Retain a copy of the last decoded object.
     */
    Atlas::Objects::Root mLastObject;
};

}

#endif /* ATLASOBJECTDECODER_H_ */
