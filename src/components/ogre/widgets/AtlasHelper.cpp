/*
 Copyright (C) 2009 Erik Hjortsberg <erik.hjortsberg@gmail.com>

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

#include "AtlasHelper.h"
#include "framework/LoggingInstance.h"
#include <Atlas/Message/Element.h>
#include <Atlas/Objects/Root.h>
#include <Atlas/Objects/Operation.h>
#include <Atlas/Objects/Encoder.h>
#include <Atlas/Message/QueuedDecoder.h>
#include <Atlas/Codecs/Bach.h>
#include <Atlas/Codecs/XML.h>
#include <Atlas/Formatter.h>
#include <sstream>
#include <memory>

namespace EmberOgre
{

namespace Gui
{

std::string AtlasHelper::serialize(const Atlas::Objects::Root& obj, const std::string& codecType)
{
	std::stringstream ss;
	Atlas::Message::QueuedDecoder decoder;
	std::auto_ptr<Atlas::Codec> codec;

	if (codecType == "bach") {
		codec.reset(new Atlas::Codecs::Bach(ss, decoder));
	} else if (codecType == "xml") {
		codec.reset(new Atlas::Codecs::XML(ss, decoder));
	} else {
		S_LOG_WARNING("Could not recognize codec type '" << codecType << "'. Supported types are: xml, bach");
		return "";
	}

	Atlas::Objects::ObjectsEncoder encoder(*codec);
	encoder.streamObjectsMessage(obj);
	ss << std::flush;
	return ss.str();
}
}
}

