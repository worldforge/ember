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

#ifndef LODMANAGER_H
#define LODMANAGER_H

#include "LodDefinition.h"
#include "XMLLodDefinitionSerializer.h"
#include "components/ogre/EmberOgrePrerequisites.h"
#include "framework/Singleton.h"

#include <string>

namespace Ember
{
namespace OgreView
{
namespace Lod
{

/**
 * @brief LodManager will assign Lod settings to meshes.
 */
class LodManager :
	public Ember::Singleton<LodManager>
{
public:

	/**
	 * @brief Ctor.
	 */
	LodManager();

	/**
	 * @brief Dtor.
	 */
	virtual ~LodManager();

	/**
	 * @brief Loads Lod settings to the passed mesh.
	 *
	 * @param mesh The mesh which needs Lod.
	 */
	void loadLod(Ogre::MeshPtr mesh);

	/**
	 * @brief Converts a *.mesh to a *.loddef name.
	 */
	std::string convertMeshNameToLodName(std::string meshName);

	/**
	 * @brief Loads LodDefinition data into the mesh.
	 */
	void loadLod(Ogre::MeshPtr mesh, const LodDefinition& definition);

private:

	/**
	 * @brief Loads Automatic Mesh Lod Management System.
	 */
	void loadAutomaticLod(Ogre::MeshPtr mesh);

};
}
}
}
#endif // ifndef LODMANAGER_H
