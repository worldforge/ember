//
// C++ Interface: MeshSerializerListener
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2009
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
#ifndef EMBEROGREMESHSERIALIZERLISTENER_H
#define EMBEROGREMESHSERIALIZERLISTENER_H

#include <OgreMeshSerializer.h>

namespace EmberOgre {

/**
@brief Allows the usage of relative skeleton names.

@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class MeshSerializerListener : public Ogre::MeshSerializerListener
{
public:
    MeshSerializerListener();

    ~MeshSerializerListener();
    
	virtual void processMaterialName(Ogre::Mesh *mesh, Ogre::String *name);
	virtual void processSkeletonName(Ogre::Mesh *mesh, Ogre::String *name);
};

}

#endif
