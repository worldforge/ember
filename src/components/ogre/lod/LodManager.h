/*
 * Copyright (c) 2013 Peter Szucs <peter.szucs.dev@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
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

	template<typename T>
	void loadUserLodImpl(T it, T itEnd, Ogre::Mesh* mesh);
	template<typename T>
	void loadAutomaticLodImpl(T it, T itEnd, Ogre::LodConfig& lodConfig);
};

}
}
}
#endif // ifndef LODMANAGER_H
