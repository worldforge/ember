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

#ifndef EMBEROGRETERRAINTECHNIQUESSHADERPASSBLENDMAPBATCH_H_
#define EMBEROGRETERRAINTECHNIQUESSHADERPASSBLENDMAPBATCH_H_

#include "components/ogre/OgreIncludes.h"
#include "components/ogre/terrain/OgreImage.h"
#include <vector>
#include <OgreTexture.h>

namespace Ember
{
namespace OgreView
{

namespace Terrain
{

class TerrainPageGeometry;
class TerrainPageSurfaceLayer;

namespace Techniques
{

class ShaderPass;

typedef std::vector<const TerrainPageSurfaceLayer*> LayerStore;

/**
 * @brief A helper for the Shader technique. Combines several blend map textures into batches and sets the appropriate pass parameters.
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 */
class ShaderPassBlendMapBatch
{
public:
	ShaderPassBlendMapBatch(ShaderPass& shaderPass, unsigned int imageSize, bool useNormalMapping = false);
	virtual ~ShaderPassBlendMapBatch();

	void addLayer(const TerrainPageGeometry& geometry, const TerrainPageSurfaceLayer* layer);

	std::vector<const TerrainPageSurfaceLayer*>& getLayers();
	Image& getCombinedBlendMapImage();

	virtual void finalize(Ogre::Pass& pass, Ogre::TexturePtr texture);

protected:

	ShaderPass& mShaderPass;

	OgreImage mCombinedBlendMapImage;
	LayerStore mLayers;
	bool mUseNormalMapping;


	/**
	 * @brief A collection of names of textures which have already been synced with the image, to prevent us from blitting when we don't need.
	 */
	std::vector<std::string> mSyncedTextures;

	void assignCombinedBlendMapTexture(Ogre::TexturePtr texture);
	void addBlendMap(const TerrainPageGeometry& geometry, const TerrainPageSurfaceLayer* layer, unsigned int channel);

};

}

}

}

}

#endif /* EMBEROGRETERRAINTECHNIQUESSHADERPASSBLENDMAPBATCH_H_ */
