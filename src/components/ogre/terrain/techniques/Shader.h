/*
 Copyright (C) 2009 Erik Ogenvik <erik@ogenvik.org>

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
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
 * @author Erik Ogenvik <erik@ogenvik.org>
 *
 * @brief A shader based technique.
 *
 * This technique will use shaders for drawing the terrain. The blend map for the different layers will be combined into one combined texture, where each channel represents one layer.
 *
*/
class Shader : public Base
{
public:

	/**
	 * @brief The alias name for the normal texture used in shaders.
	 */
	static const std::string NORMAL_TEXTURE_ALIAS;

	/**
	 * @brief The alias name for the composite map texture used in shaders.
	 */
	static const std::string COMPOSITE_MAP_ALIAS;

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
    ~Shader() override;

    virtual bool prepareMaterial();
    virtual bool compileMaterial(Ogre::MaterialPtr material, std::set<std::string>& managedTextures) const;

	virtual bool compileCompositeMapMaterial(Ogre::MaterialPtr material, std::set<std::string>& managedTextures) const;

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

	/**
	 * @brief Whether to use a composite map.
	 *
	 * The composite map should only be used if many layers are active.
	 * The downside to using a composite map is that it uses a lot of texture memory. Hence it should only
	 * be activated when there's a significant performance boost from using it.
	 */
	bool mUseCompositeMap;

	/**
	 * @brief Adds a new pass to the list of passes.
	 * @return The new pass.
	 */
	ShaderPass* addPass();

	/**
	 * @brief Adds a new normal mapped pass to the list of normal mapped passes.
	 * @return The new pass.
	 */
	ShaderPass* addPassNormalMapped();

	/**
	 * @brief Build all required passes.
	 * @param normalMapped Whether the passes should use normal mapped shaders.
	 */
	void buildPasses(bool normalMapped);

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
