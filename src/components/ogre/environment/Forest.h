//
// C++ Interface: Forest
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@iteam.se>, (C) 2008
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
#ifndef EMBEROGRE_ENVIRONMENTFOREST_H
#define EMBEROGRE_ENVIRONMENTFOREST_H

#include <OgreMath.h>
#include <OgreFrameListener.h>

namespace PagedGeometry {
class PagedGeometry;
class TreeLoader3D;
}

namespace Ogre
{
	class Entity;
	class Vector3;
}
namespace EmberOgre {

namespace Environment {

/**
	@author Erik Hjortsberg <erik.hjortsberg@iteam.se>
*/
class Forest : public Ogre::FrameListener
{
public:
    Forest();

    ~Forest();
    
    void initialize();
    
	void addTree(Ogre::Entity *entity, const Ogre::Vector3 &position, Ogre::Degree yaw = Ogre::Degree(0), Ogre::Real scale = 1.0f);
    
	bool frameStarted(const Ogre::FrameEvent & evt);

protected:


	PagedGeometry::PagedGeometry *mTrees;
	PagedGeometry::TreeLoader3D *mTreeLoader;

};

}

}

#endif
