/*
 Copyright (C) 2009 erik

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

#include "ModelRepresentationManager.h"

#include "Model.h"
#include "ModelRepresentation.h"
#include "components/ogre/EmberEntity.h"
#include "components/ogre/IGraphicalRepresentation.h"


template<> EmberOgre::Model::ModelRepresentationManager* Ember::Singleton<EmberOgre::Model::ModelRepresentationManager>::ms_Singleton = 0;

namespace EmberOgre {

namespace Model {

ModelRepresentationManager::ModelRepresentationManager()
{

}

ModelRepresentationManager::~ModelRepresentationManager()
{

}

Model* ModelRepresentationManager::getModelForEntity(EmberEntity& entity) const
{
	IGraphicalRepresentation* representation = entity.getGraphicalRepresentation();
	if (representation && representation->getType() == ModelRepresentation::getTypeNameForClass()) {
		return &static_cast<ModelRepresentation*>(representation)->getModel();
	}
	return 0;
}

ModelRepresentation* ModelRepresentationManager::getRepresentationForEntity(EmberEntity& entity) const
{
	IGraphicalRepresentation* representation = entity.getGraphicalRepresentation();
	if (representation && representation->getType() == ModelRepresentation::getTypeNameForClass()) {
		return static_cast<ModelRepresentation*>(representation);
	}
	return 0;
}

}
}
