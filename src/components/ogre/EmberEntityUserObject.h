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



namespace Ogre
{
	class Entity;
};

namespace EmberOgre {

namespace Model {
	class Model;
}
class EmberEntity;


struct CollisionResult
{
	bool collided;
	Ogre::Vector3 position;
	Ogre::Real distance;
};

/**
@author Erik Hjortsberg

* Interface for collision detectors, responsible for determining collision information for the entity that they are attached to.
*/
class ICollisionDetector
{
public:
	virtual ~ICollisionDetector() {};
	
	/**
	 *    Testa whether the provided ray hits the entity.
	 * @param ray The ray to test.
	 * @param result The result of the collision. If the ray hits, the collision detector must update this object.
	 */
	virtual void testCollision(Ogre::Ray& ray, CollisionResult& result) = 0;
	/**
	 *    Refits the collision mesh against the entity. This is called to ensure that the collision mesh fits animated entities.
	 */
	virtual void refit() = 0;
	
	
	/**
	 *    Called when the entity changes, such as a subentity being hidden or shown. Implementations must reload the collision data.
	 */
	virtual void reload() = 0;
	
	/**
	 *    Sets whether the collision data should be visualized for debugging purposes.
	 * @param visualize 
	 */
	virtual void setVisualize(bool visualize) = 0;
	/**
	 *    Gets whether the collision data should be visualized for debugging purposes.
	 * @return 
	 */
	virtual bool getVisualize() const = 0;

};


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
// 	typedef std::vector<OgreOpcode::CollisionObject*> CollisionObjectStore;
	
    /**
     * Constructor. 
     * @param emberEntity A valid EmberEntity instance.
     * @param model A valid Model instance.
     * @param collisionObject A valid vector of collision objects.
     * @return 
     */
    EmberEntityUserObject(EmberEntity* emberEntity, Model::Model* model, ICollisionDetector* collisionDetector);

    virtual ~EmberEntityUserObject();
	
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
//	CollisionObjectStore* getCollisionObjects() { return &mCollisionObjects; }
	
	/**
	 *    Overloaded method for getting the type name of this instance.
	 * @param  
	 * @return 
	 */
	virtual const Ogre::String & getTypeName (void) const;
	
	void refit();
	
	inline ICollisionDetector* getCollisionDetector() const;

private:
	EmberEntity*  mEmberEntity;
	Model::Model*   mModel;
	//CollisionObjectStore   mCollisionObjects;
	ICollisionDetector* mCollisionDetector;

};
	
ICollisionDetector* EmberEntityUserObject::getCollisionDetector() const
{
	return mCollisionDetector;
}

};

#endif
