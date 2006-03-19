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

namespace Model {
	class Model;
}
class EmberEntity;
/**
@author Erik Hjortsberg

Instances of this class can be attached to scene nodes in the ogre system. They will allow for the Ember system to be accessed directly from Ogre, without having to do lookups.
This is generally mostly used for mouse picking and collision handling.

*/
class EmberEntityUserObject : public Ogre::UserDefinedObject
{
public:

	/**
	The type of UserDefinedObject
	*/
	static const std::string s_TypeName;
	typedef std::vector<OgreOpcode::CollisionObject*> CollisionObjectStore;
	
    /**
     * Constructor. 
     * @param emberEntity A valid EmberEntity instance.
     * @param model A valid Model instance.
     * @param collisionObject A valid vector of collision objects.
     * @return 
     */
    EmberEntityUserObject(EmberEntity* emberEntity , Model::Model* model, CollisionObjectStore collisionObject);

    ~EmberEntityUserObject();
	
	/**
	 *    Gets the EmberEntity contained.
	 * @return 
	 */
	EmberEntity* getEmberEntity() const;
	
	/**
	 *    Gets the Model instance.
	 * @return 
	 */
	Model::Model* getModel() const ;
	
	/**
	 *    Gets a pointer to a vector of CollisionObjects. This can be used for checking collisions.
	 * @return 
	 */
	CollisionObjectStore* getCollisionObjects() { return &mCollisionObjects; }
	
	/**
	 *    Overloaded method for getting the type name of this instance.
	 * @param  
	 * @return 
	 */
	virtual const Ogre::String & getTypeName (void) const;
	
	void refit();

private:
	EmberEntity*  mEmberEntity;
	Model::Model*   mModel;
	CollisionObjectStore   mCollisionObjects;

};

};

#endif
