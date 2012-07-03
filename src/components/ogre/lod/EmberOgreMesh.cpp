/*
 * Copyright (C) 2012 Peter Szucs <peter.szucs.dev@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "EmberOgreMesh.h"
#include "LodManager.h"

#include "framework/LoggingInstance.h"

namespace Ember
{
namespace OgreView
{
namespace Lod
{

EmberOgreMesh::EmberOgreMesh(Ogre::ResourceManager* creator,
                             const Ogre::String& name,
                             Ogre::ResourceHandle handle,
                             const Ogre::String& group,
                             bool isManual,
                             Ogre::ManualResourceLoader* loader) :
	Ogre::Mesh(creator, name, handle, group, isManual, loader)
{
	S_LOG_INFO("Loading mesh " << mName << ".");
}

void EmberOgreMesh::loadImpl()
{
	// Load the mesh.
	Ogre::Mesh::loadImpl();

	// If we loaded the Lod from the mesh file, then skip it.
	if (getNumLodLevels() == 1) {
		LodManager::getSingleton().LoadLod(*this);
	}
}

}
}
}
