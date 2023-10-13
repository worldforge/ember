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

#include "ForestRenderingTechnique.h"
#include "environment/Forest.h"
#include "model/ModelRepresentation.h"

namespace Ember
{
namespace OgreView
{

ForestRenderingTechnique::ForestRenderingTechnique(Environment::Forest& forest) :
	mForest(forest)
{

}

void ForestRenderingTechnique::registerEntity(EmberEntity& entity)
{
	Model::ModelRepresentation* modelRepresentation = Model::ModelRepresentation::getRepresentationForEntity(entity);
	if (modelRepresentation) {
		mForest.addEmberEntity(modelRepresentation);
	}
}

void ForestRenderingTechnique::deregisterEntity(EmberEntity& entity) {
	mForest.removeEmberEntity(&entity);
}


}
}
