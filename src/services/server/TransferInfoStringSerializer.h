/*
 Copyright (C) 2010 Erik Hjortsberg <erik.hjortsberg@gmail.com>

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

#ifndef EMBER_TRANSFERINFOSTRINGSERIALIZER_H_
#define EMBER_TRANSFERINFOSTRINGSERIALIZER_H_



#include <vector>
#include <iostream>

namespace Ember
{
class AvatarTransferInfo;
/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 *
 * @brief Handles serialization of server transfer info objects.
 */
class TransferInfoStringSerializer
{
public:
	typedef std::vector<AvatarTransferInfo> TransferInfoStore;

	/**
	 * @brief Serializes transfer info objects to an output stream.
	 * @param infoObjects The objects to serialize.
	 * @param ostream A stream to which the serialized objects should be written. This needs to be valid.
	 * @return True if the serialization was successful.
	 */
	bool serialize(const TransferInfoStore& infoObjects, std::iostream& ostream);

	/**
	 * @brief Deserializes an input stream into transfer info objects.
	 * @param infoObjects A list to fill with transfer info objects.
	 * @param ostream A valid input stream of serialized transfer info data.
	 * @return True if the deserialization was successful.
	 */
	bool deserialize(TransferInfoStore& infoObjects, std::iostream& istream);
};

}

#endif /* TRANSFERINFOSTRINGSERIALIZER_H_ */
