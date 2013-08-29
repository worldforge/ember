/*
 Copyright (C) 2009 Erik Hjortsberg <erik.hjortsberg@gmail.com>

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */


#ifndef EMBEROGRETERRAINTECHNIQUESSHADER_H_
#define EMBEROGRETERRAINTECHNIQUESSHADER_H_

#include "Base.h"
#include "components/ogre/OgreIncludes.h"

namespace Ember
{
namespace OgreView
{

namespace Terrain
{

namespace Techniques
{

class ShaderPass;

/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 *
 * @brief A shader based technique.
 *
 * This technique will use shaders for drawing the terrain. The coverage for the different layers will be combined into one combined texture, where each channel represents one layer.
 *
*/
class Shader : public Base
{
public:

    /**
     * @brief Ctor.
     * @param includeShadows If true, shadows will be used.
     * @param geometry The geometry to operate on.
     * @param terrainPageSurfaces The surfaces to generate a rendering technique for.
     * @param terrainPageShadow An optional shadow.
     * @param sceneManager The scene manager which will hold the terrain.
	 * @param useNormalMapping Whether to use normal mapping.
     */
	Shader(bool includeShadows, const TerrainPageGeometryPtr& geometry, const SurfaceLayerStore& terrainPageSurfaces, const TerrainPageShadow* terrainPageShadow, Ogre::SceneManager& sceneManager, bool useNormalMapping = false);

	/**
	 * @brief Dtor.
	 */
    virtual ~Shader();

    virtual bool prepareMaterial();
    virtual bool compileMaterial(Ogre::MaterialPtr material);

protected:
	typedef std::vector<ShaderPass*> PassStore;

	/**
	 * @brief Sets whether shadows should be used.
	 */
	bool mIncludeShadows;

	/**
	 * @brief The scene manager in which the material will be used.
	 */
	Ogre::SceneManager& mSceneManager;

	/**
	 * @brief A collection of passes for the material.
	 */
	PassStore mPasses;

	/**
	 * @brief A collection of passes for the material, only used if normal mapping enabled.
	 */
	PassStore mPassesNormalMapped;

	/**
	 * @brief Whether to use normal mapping.
	 */
	bool mUseNormalMapping;

	virtual ShaderPass* addPass();

	virtual ShaderPass* addPassNormalMapped();

	virtual void initializePass(ShaderPass* pass);

	/**
	 * @brief Adds the first layer.
	 * @param pass The pass which is used to draw this layer.
	 * @param layer The layer definition.
	 */
	void addBaseLayer(Ogre::Pass* pass, TerrainPageSurfaceLayer* layer);

	/**
	 * @brief Adds a layer, apart from the first layer.
	 * @param pass The pass which is used to draw this layer.
	 * @param layer The layer definition.
	 */
	void addLayer(Ogre::Pass* pass, TerrainPageSurfaceLayer* layer);

	/**
	 * @brief Resets the technique, removing all passes.
	 */
	void reset();

};


}

}

}

}

#endif /* EMBEROGRETERRAINTECHNIQUESSHADER_H_ */
