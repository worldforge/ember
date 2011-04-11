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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "TransferInfoStringSerializer.h"

#include "framework/Tokeniser.h"
#include <Eris/TransferInfo.h>
#include <sstream>
#include <string>
#include <stdlib.h>
namespace Ember
{

TransferInfoStringSerializer::TransferInfoStringSerializer()
{
}

TransferInfoStringSerializer::~TransferInfoStringSerializer()
{
}

bool TransferInfoStringSerializer::serialize(const std::vector<Eris::TransferInfo>& infoObjects, std::ostream& ostream)
{
	for (TransferInfoStore::const_iterator I = infoObjects.begin(); I != infoObjects.end(); ++I) {
		const Eris::TransferInfo& transferInfo = *I;
		ostream << transferInfo.getHost() << "," << transferInfo.getPort() << "," << transferInfo.getPossessEntityId() << "," << transferInfo.getPossessKey() << ";";
	}
	return true;
}

bool TransferInfoStringSerializer::deserialize(std::vector<Eris::TransferInfo>& infoObjects, std::istream& istream)
{
	std::stringstream ss;
	ss << istream;
	Tokeniser tokeniser(ss.str(), ";");
	while (tokeniser.hasRemainingTokens()) {
		std::string token = tokeniser.nextToken();
		if (token.size()) {
			std::vector<std::string> data = Tokeniser::split(token, ",");
			if (data.size() == 4) {
				infoObjects.push_back(Eris::TransferInfo(data[0], atoi(data[1].c_str()), data[2], data[3]));
			}
		}
	}
	return true;
}

}
