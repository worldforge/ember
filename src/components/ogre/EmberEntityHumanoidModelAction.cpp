/*
 Copyright (C) 2014 Céline NOËL <celine.noel.7294@gmail.com>

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

#include "EmberEntityHumanoidModelAction.h"

#include "components/ogre/model/ModelRepresentationHumanoid.h"
#include "components/ogre/EmberOgre.h"


namespace Ember
{

namespace OgreView
{

EmberEntityHumanoidModelAction::EmberEntityHumanoidModelAction(EmberEntity& entity, const std::string& modelName, Scene& scene, EntityMapping::EntityMapping& modelMapping)
	: EmberEntityModelAction::EmberEntityModelAction(entity, modelName, scene, modelMapping)
{
}

EmberEntityHumanoidModelAction::~EmberEntityHumanoidModelAction()
{
}

Model::ModelRepresentation* EmberEntityHumanoidModelAction::createModelRepresentation(Model::Model& model)
{
	SmartBodyManager& sbManager = OgreView::EmberOgre::getSingleton().getSmartBodyManager();
	Model::ModelRepresentationHumanoid *representation = new Model::ModelRepresentationHumanoid(mEntity, model, mScene, mMapping, sbManager);
	
	return representation;
}

}
}