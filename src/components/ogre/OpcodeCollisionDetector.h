//
// C++ Interface: OpcodeCollisionDetector
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2006
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

#include "EmberOgrePrerequisites.h"
#include "EmberEntityUserObject.h"

namespace OgreOpcode
{
	class CollisionObject;
};

namespace EmberOgre {

/**
	@author Erik Hjortsberg <erik@katastrof.nu>
*/
class OpcodeCollisionDetector
: public ICollisionDetector
{
public:
	
	OpcodeCollisionDetector(Model::Model* model);
	virtual ~OpcodeCollisionDetector();
	
	virtual void testCollision(Ogre::Ray& ray, CollisionResult& result);
	virtual void refit();
private:
	typedef std::vector<OgreOpcode::CollisionObject*> CollisionObjectStore;
	void buildCollisionObjects();
	void destroyCollisionObjects();
	CollisionObjectStore   mCollisionObjects;
	Model::Model* mModel;

};
}

#endif
