/*
 Copyright (C) 2009 Erik Ogenvik <erik@ogenvik.org>

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

#include "OceanRepresentation.h"
#include "IEnvironmentProvider.h"
#include "Environment.h"
#include "domain/EmberEntity.h"
#include "components/ogre/EmberEntityUserObject.h"
#include <OgreAny.h>

namespace Ember
{
namespace OgreView
{

namespace Environment
{

const std::string OceanRepresentation::sTypeName("OceanRepresentation");

OceanRepresentation::OceanRepresentation(EmberEntity& entity, Environment& environment) :
		mEntity(entity), mEnvironment(environment)
{
	mEnvironment.setWaterEnabled(true);
	if (mEnvironment.getWater()) {

		mEntity.Moved.connect(sigc::mem_fun(*this, &OceanRepresentation::entity_Moved));
		updateWaterPosition();

		ICollisionDetector* collisionDetector = mEnvironment.getWater()->createCollisionDetector();
		EmberEntityUserObject* userObject = new EmberEntityUserObject(entity, collisionDetector);
		EmberEntityUserObject::SharedPtr sharedUserObject(userObject);
		mEnvironment.getWater()->setUserAny(Ogre::Any(sharedUserObject));
	}
}

OceanRepresentation::~OceanRepresentation()
{
	mEnvironment.setWaterEnabled(false);
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
	if (mEnvironment.getWater()) {
		if (mEntity.getPredictedPos().isValid()) {
			mEnvironment.getWater()->setLevel(mEntity.getPredictedPos().z());
		}
	}
}

}
}
}

