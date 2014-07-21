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

#ifndef EMBEROGREEMBERENTITYHUMANOIDMODELACTION_H
#define EMBEROGREEMBERENTITYHUMANOIDMODELACTION_H

#include "EmberEntityModelAction.h"

namespace Ember
{
class EmberEntity;

namespace EntityMapping
{
class EntityMapping;
}

namespace OgreView
{
class Scene;

namespace Model
{
class Model;
class ModelRepresentation;
}

/**
 * @brief Show a certain humanoid Model.
 * @author Céline Noël <celine.noel.7294@gmail.com>
 */
class EmberEntityHumanoidModelAction: public EmberEntityModelAction
{
public:

	/**
	 * @brief Ctor.
	 */
	EmberEntityHumanoidModelAction(EmberEntity& entity, const std::string& modelName, Scene& scene, EntityMapping::EntityMapping& modelMapping);

	/**
	 * @brief Dtor.
	 */
	virtual ~EmberEntityHumanoidModelAction();


protected:
	
	/**
	 * @brief Creates the corresponding ModelRepresentationHumanoid.
	 */
	Model::ModelRepresentation* createModelRepresentation(Model::Model& model);
};

}

}

#endif