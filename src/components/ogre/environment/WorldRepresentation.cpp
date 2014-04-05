/*
 Copyright (C) 2012 Erik Ogenvik

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
#include "WorldRepresentation.h"
#include "Environment.h"
#include "domain/EmberEntity.h"

namespace Ember
{
namespace OgreView
{
namespace Environment
{

const std::string WorldRepresentation::sTypeName("WorldRepresentation");

WorldRepresentation::WorldRepresentation(EmberEntity& entity, Environment& environment) :
		mEntity(entity), mEnvironment(environment)
{
	mEnvironment.setFirmamentEnabled(true);
	if (entity.getPredictedPos().isValid()) {
		mEnvironment.setWorldPosition(entity.getPredictedPos().x(), entity.getPredictedPos().y());
	} else {
		mEnvironment.setWorldPosition(0, 0);
	}
}

WorldRepresentation::~WorldRepresentation()
{
	mEnvironment.setFirmamentEnabled(false);
}

const std::string& WorldRepresentation::getType() const
{
	return sTypeName;
}

const std::string& WorldRepresentation::getTypeNameForClass()
{
	return sTypeName;
}

void WorldRepresentation::setVisualize(const std::string& visualization, bool visualize)
{

}
bool WorldRepresentation::getVisualize(const std::string& visualization) const
{
	return false;
}

}
}
}
