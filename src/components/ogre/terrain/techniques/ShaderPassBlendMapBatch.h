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

#ifndef EMBEROGRETERRAINTECHNIQUESSHADERPASSBLENDMAPBATCH_H_
#define EMBEROGRETERRAINTECHNIQUESSHADERPASSBLENDMAPBATCH_H_

#include "components/ogre/OgreIncludes.h"
#include "components/ogre/terrain/OgreImage.h"
#include <vector>
#include <OgreTexture.h>

namespace Ember::OgreView::Terrain {

class TerrainPageGeometry;

class TerrainPageSurfaceLayer;

namespace Techniques {

class ShaderPass;

typedef std::vector<const TerrainPageSurfaceLayer*> LayerStore;

/**
 * @brief A helper for the Shader technique. Combines several blend map textures into batches and sets the appropriate pass parameters.
 * @author Erik Ogenvik <erik@ogenvik.org>
 */
class ShaderPassBlendMapBatch {
public:
	ShaderPassBlendMapBatch(ShaderPass& shaderPass, unsigned int imageSize);

	void addLayer(const TerrainPageGeometry& geometry, const TerrainPageSurfaceLayer* layer);

	std::vector<const TerrainPageSurfaceLayer*>& getLayers();

	void finalize(Ogre::Pass& pass, const Ogre::TexturePtr& texture, bool useNormalMapping);

private:

	OgreImage mCombinedBlendMapImage;
	LayerStore mLayers;

	/**
	 * @brief A collection of names of textures which have already been synced with the image, to prevent us from blitting when we don't need.
	 */
	std::vector<std::string> mSyncedTextures;

	void assignCombinedBlendMapTexture(Ogre::TexturePtr texture);

	void addBlendMap(const TerrainPageGeometry& geometry, const TerrainPageSurfaceLayer* layer, unsigned int channel);

};

}

}





#endif /* EMBEROGRETERRAINTECHNIQUESSHADERPASSBLENDMAPBATCH_H_ */
