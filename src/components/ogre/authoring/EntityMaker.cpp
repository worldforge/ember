/*
 Copyright (C) 2011 Erik Ogenvik

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

#include "EntityMaker.h"

#include "framework/LoggingInstance.h"
#include "framework/Tokeniser.h"

#include <Eris/TypeService.h>
#include <Eris/Connection.h>
#include <Eris/TypeInfo.h>
#include <Eris/Avatar.h>
#include <Eris/View.h>
#include <Eris/Entity.h>

#include <Atlas/Objects/Operation.h>

#include <wfmath/atlasconv.h>

namespace Ember {
namespace OgreView {
namespace Authoring {

EntityMaker::EntityMaker(Eris::Avatar& avatar, Eris::Connection& connection) :
		CreateEntity("createentity", this, "Create an entity."),
		Make("make", this, "Create an entity."),
		MakeMe("makeme", this, "Create an entity as a child of the avatar (i.e. in inventory)."),
		mAvatar(avatar),
		mConnection(connection) {
}

void EntityMaker::runCommand(const std::string& command, const std::string& args) {
	if (CreateEntity == command || Make == command || MakeMe == command) {
		Tokeniser tokeniser(args);
		if (tokeniser.hasRemainingTokens()) {
			std::string typeName = tokeniser.nextToken();
			Eris::TypeService* typeService = mConnection.getTypeService();
			Eris::TypeInfo* typeinfo = typeService->getTypeByName(typeName);
			if (typeinfo) {
				std::string parentEntityId = mAvatar.getEntity()->getLocation()->getId();
				if (MakeMe == command) {
					parentEntityId = mAvatar.getEntity()->getId();
				} else {
					if (tokeniser.hasRemainingTokens()) {
						parentEntityId = tokeniser.nextToken();
					}
				}
				createEntityOfType(typeinfo, parentEntityId);
			}
		}
	}
}

void EntityMaker::createEntityOfType(Eris::TypeInfo* typeinfo, const std::string& parentEntityId, const std::string& name) {
	Atlas::Objects::Operation::Create c;
	c->setFrom(mAvatar.getId());
	//if the avatar is a "creator", i.e. and admin, we will set the TO property
	//this will bypass all of the server's filtering, allowing us to create any entity and have it have a working mind too
	if (mAvatar.getIsAdmin()) {
		c->setTo(mAvatar.getEntityId());
	}

	Atlas::Message::MapType msg;
	msg["loc"] = parentEntityId;

	WFMath::Point<3> pos = WFMath::Point<3>::ZERO();
	WFMath::Quaternion orientation = WFMath::Quaternion::Identity();

	//Only place it if we're creating the new entity in the same location as the avatar
	if (parentEntityId == mAvatar.getEntity()->getLocation()->getId()) {
		//Place the new entity two meters in front of the avatar.
		WFMath::Vector<3> vector(0, 0, 2);

		//We need to constraint the orientation to only around the y axis.
		WFMath::Vector<3> rotator(0.0, 0.0, 1.0f);
		rotator.rotate(mAvatar.getEntity()->getOrientation());
		auto atan = std::atan2(rotator.x(), rotator.z());
		orientation.rotation(1, atan);

		pos = mAvatar.getEntity()->getPosition() + (vector.rotate(orientation));
	}

	msg["pos"] = pos.toAtlas();
	msg["orientation"] = orientation.toAtlas();

	if (!name.empty()) {
		msg["name"] = name;
	}
	msg["parent"] = typeinfo->getName();

	c->setArgsAsList(Atlas::Message::ListType(1, msg));
	mConnection.send(c);
	std::stringstream ss;
	ss << pos;
	S_LOG_INFO("Trying to create entity of type " << typeinfo->getName() << " at position " << ss.str());

}
}
}
}
