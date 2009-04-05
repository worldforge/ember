//
// C++ Interface: OpcodeCollisionDetector
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2006
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
#ifndef EMBEROGREOPCODECOLLISIONDETECTOR_H
#define EMBEROGREOPCODECOLLISIONDETECTOR_H

#include "EmberEntityUserObject.h"

namespace OgreOpcode
{
	class CollisionObject;
	namespace Details 
	{
		class OgreOpcodeDebugger;
	}
};

namespace EmberOgre {

class OpcodeCollisionDetector;
class OpcodeCollisionDetectorVisualizerInstance;

namespace Model
{
class Model;
}

/**
	@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class OpcodeCollisionDetector
: public ICollisionDetector
{
public:
	friend class OpcodeCollisionDetectorVisualizerInstance;
	
	OpcodeCollisionDetector(Model::Model* model);
	virtual ~OpcodeCollisionDetector();
	
	virtual void testCollision(Ogre::Ray& ray, CollisionResult& result);
	virtual void refit();
	/**
	 *    Called when the entity changes, such as a subentity being hidden or shown. Implementations must reload the collision data.
	 */
	virtual void reload();	
	
	virtual void setVisualize(bool visualize);
	virtual bool getVisualize() const;
private:
	typedef std::vector<OgreOpcode::CollisionObject*> CollisionObjectStore;
	void buildCollisionObjects();
	void destroyCollisionObjects();
	CollisionObjectStore mCollisionObjects;
	Model::Model* mModel;
	OpcodeCollisionDetectorVisualizerInstance* mVisualizer;
};
}

#endif
