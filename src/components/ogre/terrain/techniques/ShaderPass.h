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


#ifndef EMBEROGRETERRAINTECHNIQUESSHADERPASS_H_
#define EMBEROGRETERRAINTECHNIQUESSHADERPASS_H_

#include "components/ogre/OgreIncludes.h"
#include <wfmath/point.h>
#include <vector>
#include <string>
#include <set>

namespace Ember {
namespace OgreView {

namespace Terrain {
class TerrainPage;

class TerrainPageSurfaceLayer;

class TerrainPageGeometry;

class TerrainPageShadow;

namespace Techniques {

class ShaderPassBlendMapBatch;

typedef std::vector<const TerrainPageSurfaceLayer*> LayerStore;

class ShaderPass {
public:
	friend class ShaderPassBlendMapBatch;

	ShaderPass(Ogre::SceneManager& sceneManager, int blendMapPixelWidth, const WFMath::Point<2>& position, bool useNormalMapping = false);

	virtual ~ShaderPass();

	virtual void addLayer(const TerrainPageGeometry& geometry, const TerrainPageSurfaceLayer* layer);

	virtual void setBaseLayer(const TerrainPageSurfaceLayer* layer);

	void addShadowLayer();

	virtual bool hasRoomForLayer(const TerrainPageSurfaceLayer* layer);

	/**
	 * @brief Creates the combined final blend maps and sets the shader params. Be sure to call this before you load the material.
     * @param managedTextures A set of textures created in the process. These will be destroyed when the page is destroyed.
	 * @param useShadows Whether to use shadows or not in the pass.
	 * @param shaderSuffix A suffix to add to the shader name. This allows you to make it use a somewhat different shader depending on graphics level etc.
	 * @return True if the creation of the pass was successful.
	 */
	bool finalize(Ogre::Pass& pass, std::set<std::string>& managedTextures, bool useShadows = true, const std::string& shaderSuffix = "") const;

	LayerStore& getLayers();

protected:

	ShaderPassBlendMapBatch* getCurrentBatch();

	virtual std::unique_ptr<ShaderPassBlendMapBatch> createNewBatch();

	unsigned int getBlendMapPixelWidth() const;

	Ogre::TexturePtr getCombinedBlendMapTexture(size_t passIndex, size_t batchIndex, std::set<std::string>& managedTextures) const;

	float mScales[16];
	std::vector<std::unique_ptr<ShaderPassBlendMapBatch>> mBlendMapBatches;
	LayerStore mLayers;
	const TerrainPageSurfaceLayer* mBaseLayer;
	Ogre::SceneManager& mSceneManager;
	int mBlendMapPixelWidth;
	WFMath::Point<2> mPosition;

	int mShadowLayers;

	bool mUseNormalMapping;
};
}

}

}

}

#endif /* EMBEROGRETERRAINTECHNIQUESSHADERPASS_H_ */
