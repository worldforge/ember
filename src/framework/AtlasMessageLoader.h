//
// Copyright (C) 2004-2005 Alistair Riddoch
// Copyright (C) 2012 Erik Ogenvik
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

#ifndef COMMON_ATLAS_FILE_LOADER_H
#define COMMON_ATLAS_FILE_LOADER_H

#include <Atlas/Objects/Decoder.h>
#include <Atlas/Codec.h>

#include <iostream>

namespace Ember
{

/**
 * @brief Loads messages from a codec into a map.
 */
class AtlasMessageLoader: public Atlas::Objects::ObjectsDecoder
{
private:

	/**
	 * @brief Counter for messages read from input.
	 */
	int mCount;

	/**
	 * @brief Store for the messages loaded.
	 */
	std::map<std::string, Atlas::Objects::Root> & mMessages;

	/**
	 * @brief Called from the base class when a complete message has been decoded.
	 *
	 * @param obj
	 */
	void objectArrived(const Atlas::Objects::Root & obj) override;
public:

	/**
	 * @brief Ctor.
	 * @param messages The map into which decoded messages will be put.
	 */
	AtlasMessageLoader(std::map<std::string, Atlas::Objects::Root> & messages);

	/**
	 * @brief Dtor.
	 */
	~AtlasMessageLoader() override;

	/**
	 * @brief Read only accessor for the number of messages loaded
	 */
	int count();
};
}

#endif // COMMON_ATLAS_FILE_LOADER_H
