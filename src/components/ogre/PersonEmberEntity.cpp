/*
    Copyright (C) 2004  Erik Hjortsberg

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

#include "EmberEntity.h"
#include "EmberPhysicalEntity.h"
#include "model/Model.h"

#include "PersonEmberEntity.h"
namespace EmberOgre {

PersonEmberEntity::PersonEmberEntity(const std::string& id, Eris::TypeInfo* ty, Eris::View* vw, Ogre::SceneManager* sceneManager, Ogre::SceneNode* nodeWithModel) : 
EmberPhysicalEntity(id, ty, vw, sceneManager, nodeWithModel)

{
}

PersonEmberEntity::~PersonEmberEntity()
{}

void PersonEmberEntity::init(const Atlas::Objects::Entity::RootEntity &ge)
{
	EmberPhysicalEntity::init(ge);
//	mModel->setQueryFlags(EmberEntity::CM_PERSONS);
}

bool PersonEmberEntity::allowVisibilityOfMember(EmberEntity* entity)
{
	return false;
}
}
