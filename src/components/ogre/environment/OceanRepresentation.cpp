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

#include "OceanRepresentation.h"
#include "Environment.h"
#include "components/ogre/EmberEntity.h"
#include "components/ogre/EmberEntityUserObject.h"
#include <OgreAny.h>
#include <boost/smart_ptr.hpp>

namespace EmberOgre
{

namespace Environment
{

std::string OceanRepresentation::sTypeName("OceanRepresentation");

OceanRepresentation::OceanRepresentation(EmberEntity& entity, IWater& water) :
	mEntity(entity), mWater(water)
{
	mEntity.Moved.connect(sigc::mem_fun(*this, &OceanRepresentation::entity_Moved));
	updateWaterPosition();

	ICollisionDetector* collisionDetector = mWater.createCollisionDetector();
	EmberEntityUserObject* userObject = new EmberEntityUserObject(entity, collisionDetector);
	EmberEntityUserObject::SharedPtr sharedUserObject(userObject);
	water.setUserAny(Ogre::Any(sharedUserObject));
}

OceanRepresentation::~OceanRepresentation()
{
}

const std::string& OceanRepresentation::getType() const
{
	return sTypeName;
}

const std::string& OceanRepresentation::getTypeNameForClass()
{
	return sTypeName;
}

void OceanRepresentation::setVisualize(const std::string& visualization, bool visualize)
{

}
bool OceanRepresentation::getVisualize(const std::string& visualization) const
{
	return false;
}

void OceanRepresentation::entity_Moved()
{
	updateWaterPosition();
}

void OceanRepresentation::updateWaterPosition()
{
	mWater.setLevel(mEntity.getPredictedPos().z());
}

}
}

