/*
 Copyright (C) 2010 Erik Ogenvik <erik@ogenvik.org>

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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "TransferInfoStringSerializer.h"
#include "AvatarTransferInfo.h"

#include "framework/LoggingInstance.h"

#include <Atlas/Message/Element.h>
#include <Atlas/Message/MEncoder.h>
#include <Atlas/Message/QueuedDecoder.h>
#include <Atlas/Codecs/Bach.h>
#include <Atlas/Codecs/XML.h>
#include <Atlas/Formatter.h>

namespace Ember
{

bool TransferInfoStringSerializer::serialize(const TransferInfoStore& infoObjects, std::iostream& ostream)
{
	Atlas::Message::ListType infos;
	for (TransferInfoStore::const_iterator I = infoObjects.begin(); I != infoObjects.end(); ++I) {
		const AvatarTransferInfo& transferInfo = *I;
		Atlas::Message::MapType info;
		info["host"] = transferInfo.getTransferInfo().getHost();
		info["port"] = transferInfo.getTransferInfo().getPort();
		info["key"] = transferInfo.getTransferInfo().getPossessKey();
		info["entityid"] = transferInfo.getTransferInfo().getPossessEntityId();
		info["avatarname"] = transferInfo.getAvatarName();
		info["timestamp"] = (transferInfo.getTimestamp() - WFMath::TimeStamp::epochStart()).milliseconds();
		infos.push_back(info);
	}

	Atlas::Message::QueuedDecoder decoder;
	Atlas::Codecs::Bach codec(ostream, ostream, decoder);

	Atlas::Message::Encoder encoder(codec);

	Atlas::Message::MapType map;
	map["teleports"] = infos;
	Atlas::Formatter formatter(ostream, codec);
	formatter.streamBegin();
	encoder.streamMessageElement(map);
	formatter.streamEnd();

	return true;
}

bool TransferInfoStringSerializer::deserialize(TransferInfoStore& infoObjects, std::iostream& istream)
{
	try {
		Atlas::Message::QueuedDecoder decoder;

		istream.seekg(0);
		Atlas::Codecs::Bach codec(istream, istream, decoder);
		// Read whole stream into decoder queue
		while (!istream.eof()) {
			codec.poll(true);
		}

		// Read decoder queue
		if (decoder.queueSize() > 0) {

			Atlas::Message::MapType map = decoder.popMessage();
			Atlas::Message::MapType::const_iterator I = map.find("teleports");
			if (I != map.end()) {
				if (I->second.isList()) {
					Atlas::Message::ListType infos = I->second.asList();
					for (Atlas::Message::ListType::const_iterator J = infos.begin(); J != infos.end(); ++J) {
						Atlas::Message::Element infoElement = *J;
						if (infoElement.isMap()) {
							Atlas::Message::MapType info = infoElement.asMap();
							const std::string& host = info["host"].asString();
							int port = info["port"].asInt();
							const std::string& key = info["key"].asString();
							const std::string& entityId = info["entityid"].asString();
							const std::string& avatarName = info["avatarname"].asString();
							long timestamp = info["timestamp"].asInt();
							infoObjects.emplace_back(avatarName, WFMath::TimeStamp::epochStart() + WFMath::TimeDiff(timestamp), Eris::TransferInfo(host, port, key, entityId));
						}
					}
				}
			}
		}
	} catch (const std::exception& ex) {
		S_LOG_FAILURE("Couldn't deserialize transfer info objects." << ex);
		return false;
	}
	S_LOG_VERBOSE("Read " << infoObjects.size() << " transfer info objects from storage.");
	return true;
}

}
