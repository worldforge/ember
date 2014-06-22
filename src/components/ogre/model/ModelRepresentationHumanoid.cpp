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

#include "ModelRepresentationHumanoid.h"
#include "ModelRepresentation.h"

#include "Model.h"
#include "ModelDefinition.h"
#include "SubModel.h"
#include "ParticleSystemBinding.h"
#include "ModelAction.h"
#include "ModelMount.h"
#include "ModelPartReactivatorVisitor.h"

#include "components/ogre/mapping/EmberEntityMappingManager.h"

#include "components/ogre/sound/SoundEntity.h"

#include "components/ogre/Convert.h"
#include "components/ogre/MousePicker.h"
#include "components/ogre/EmberEntityUserObject.h"
#include "components/ogre/MeshCollisionDetector.h"
#include "domain/EmberEntity.h"
#include "components/ogre/EmberEntityFactory.h"
#include "components/ogre/EmberOgre.h"
#include "components/ogre/MotionManager.h"
#include "components/ogre/Scene.h"

#include "components/entitymapping/EntityMapping.h"
#include "components/entitymapping/EntityMappingManager.h"

#include "components/sb/SmartBodyManager.h"

#include <OgreException.h>

#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include <OgreParticleSystem.h>

#include <Eris/Types.h>
#include <Eris/Task.h>

#include <memory>

namespace Ember
{
namespace OgreView
{
namespace Model
{

std::string ModelRepresentationHumanoid::sTypeName("ModelRepresentationHumanoid");

/*const char * const ModelRepresentation::ACTION_STAND("idle");
const char * const ModelRepresentation::ACTION_RUN("run");
const char * const ModelRepresentation::ACTION_WALK("walk");
const char * const ModelRepresentation::ACTION_SWIM("swim");
const char * const ModelRepresentation::ACTION_FLOAT("float");*/

ModelRepresentationHumanoid::ModelRepresentationHumanoid(EmberEntity& entity, Model& model, Scene& scene, EntityMapping::EntityMapping& mapping, SmartBodyManager *sbManager, const std::string& sbSkName)
	: ModelRepresentation::ModelRepresentation(entity, model, scene, mapping)
{
	//Create the SmartBody character.
	mCharacter = sbManager->createCharacter(*model.getSkeletonOwnerEntity(), model.getDefinition()->getName(), sbSkName);
}

ModelRepresentationHumanoid::~ModelRepresentationHumanoid()
{
	//TODO : Destroy the Sb character ?

}

void ModelRepresentationHumanoid::initFromModel()
{
	return;
}

const std::string& ModelRepresentationHumanoid::getType() const
{
	return sTypeName;
}

const std::string& ModelRepresentationHumanoid::getTypeNameForClass()
{
	return sTypeName;
}

void ModelRepresentationHumanoid::updateAnimation(float timeSlice)
{
	return;
}

ModelRepresentation::MovementMode ModelRepresentationHumanoid::getMovementMode() const
{
	return ModelRepresentation::getMovementMode();
}

void ModelRepresentationHumanoid::setLocalVelocity(const WFMath::Vector<3>& velocity)
{
	return;
}

void ModelRepresentationHumanoid::onMovementModeChanged(MovementMode newMode)
{
	return;
}

void ModelRepresentationHumanoid::parseMovementMode(const WFMath::Vector<3>& velocity)
{
	return;
}

void ModelRepresentationHumanoid::resetAnimations()
{
	return;
}



}
}
}
