//
// C++ Implementation: EmberEntityUserObject
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2005
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//
#include "EmberEntityUserObject.h"
#include "ogreopcode/include/OgreCollisionObject.h"
#include "EmberEntity.h"
#include "model/Model.h"


namespace EmberOgre {

const Ogre::String EmberEntityUserObject::s_TypeName = "EmberEntityPickerObject";


EmberEntityUserObject::EmberEntityUserObject(EmberEntity* emberEntity,  Model::Model* model,  CollisionObjectStore collisionObjects)
: mModel(model),
mEmberEntity(emberEntity),
mCollisionObjects(collisionObjects)
{
}

// CollisionObjectStore EmberEntityUserObject::getCollisionObjects() const
// {
// 	return mCollisionObjects;
// }

EmberEntityUserObject::~EmberEntityUserObject()
{
	OgreOpcode::CollisionContext* collideContext = OgreOpcode::CollisionManager::getSingletonPtr()->getDefaultContext();
	for (EmberEntityUserObject::CollisionObjectStore::iterator I = mCollisionObjects.begin(); I != mCollisionObjects.end(); ++I)
	{
		collideContext->removeObject(*I);
		OgreOpcode::CollisionManager::getSingleton().destroyShape((*I)->getShape());
		delete *I;
	}
}

void EmberEntityUserObject::refit()
{
	for (EmberEntityUserObject::CollisionObjectStore::iterator I = mCollisionObjects.begin(); I != mCollisionObjects.end(); ++I)
	{
		(*I)->refit();;
	}
}

EmberEntity*  EmberEntityUserObject::getEmberEntity() const
{
	return mEmberEntity;
}

Model::Model*  EmberEntityUserObject::getModel() const
{
	return mModel;
}

const Ogre::String & EmberEntityUserObject::getTypeName (void) const
{
	return s_TypeName;
}



};
