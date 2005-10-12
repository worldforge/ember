//
// C++ Interface: EmberEntityUserObject
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
#ifndef EMBEROGREEMBERENTITYUSEROBJECT_H
#define EMBEROGREEMBERENTITYUSEROBJECT_H

#include "EmberOgrePrerequisites.h"

namespace OgreOpcode
{
	class CollisionObject;
};

namespace Ogre
{
	class Entity;
};

namespace EmberOgre {

class Model;
class EmberEntity;
/**
@author Erik Hjortsberg
*/
class EmberEntityUserObject : public Ogre::UserDefinedObject
{
public:

	static const std::string s_TypeName;
	typedef std::vector<OgreOpcode::CollisionObject*> CollisionObjectStore;
	
    EmberEntityUserObject(EmberEntity* emberEntity , Model* model, CollisionObjectStore collisionObject);

    ~EmberEntityUserObject();
	
	EmberEntity* getEmberEntity() const;
	Model* getModel() const ;
	CollisionObjectStore* getCollisionObjects() { return &mCollisionObjects; }
	virtual const Ogre::String & getTypeName (void) const;

private:
	EmberEntity*  mEmberEntity;
	 Model*   mModel;
	CollisionObjectStore   mCollisionObjects;

};

};

#endif
