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

#ifndef EMBEROGRETERRAINTECHNIQUESSHADERPASSCOVERAGEBATCH_H_
#define EMBEROGRETERRAINTECHNIQUESSHADERPASSCOVERAGEBATCH_H_

#include "components/ogre/OgreIncludes.h"
#include "components/ogre/terrain/OgreImage.h"
#include <vector>
#include <OgreTexture.h>

namespace EmberOgre
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
	@brief A shader enabled technique.
	This is the preferred technique for all more modern cards which can handle shaders. The coverage textures are baked into one single texture, where each coverage texture represents one channel. The shaders named "splatting_fragment_*" are then used for rendering.

	@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class ShaderPassCoverageBatch
{
public:
	ShaderPassCoverageBatch(ShaderPass& shaderPass, unsigned int imageSize);
	virtual ~ShaderPassCoverageBatch();

	void addLayer(const TerrainPageGeometry& geometry, const TerrainPageSurfaceLayer* layer);

	std::vector<const TerrainPageSurfaceLayer*>& getLayers();
	Image& getCombinedCoverageImage();

	virtual void finalize(Ogre::Pass& pass, Ogre::TexturePtr texture);

protected:

	ShaderPass& mShaderPass;

	OgreImage mCombinedCoverageImage;
	LayerStore mLayers;


	/**
	 * @brief A collection of names of textures which have already been synced with the image, to prevent us from blitting when we don't need.
	 */
	std::vector<std::string> mSyncedTextures;

	void assignCombinedCoverageTexture(Ogre::TexturePtr texture);
	void addCoverage(const TerrainPageSurfaceLayer* layer, unsigned int channel);

};

}

}

}

#endif /* EMBEROGRETERRAINTECHNIQUESSHADERPASSCOVERAGEBATCH_H_ */
