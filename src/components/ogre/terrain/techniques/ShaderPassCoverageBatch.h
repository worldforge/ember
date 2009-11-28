/*
 * ShaderPassCoverageBatch.h
 *
 *  Created on: 26 nov 2009
 *      Author: erik
 */

#ifndef EMBEROGRETERRAINTECHNIQUESSHADERPASSCOVERAGEBATCH_H_
#define EMBEROGRETERRAINTECHNIQUESSHADERPASSCOVERAGEBATCH_H_

#include "components/ogre/OgreIncludes.h"
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
	ShaderPassCoverageBatch(ShaderPass& shaderPass, Ogre::TexturePtr combinedCoverageTexture);
	virtual ~ShaderPassCoverageBatch();

	void addLayer(const TerrainPageGeometry& geometry, const TerrainPageSurfaceLayer* layer);

	std::vector<const TerrainPageSurfaceLayer*>& getLayers();
	Ogre::TexturePtr getCombinedCoverageTexture();

	virtual void finalize();

protected:

	ShaderPass& mShaderPass;

	Ogre::TexturePtr mCombinedCoverageTexture;
	Ogre::Image* mCombinedCoverageImage;
	Ogre::MemoryDataStream* mCombinedCoverageDataStream;
	LayerStore mLayers;

	void assignCombinedCoverageTexture();
	void addCoverage(Ogre::Image* coverage, unsigned int channel, unsigned short numberOfChannels);

};

}

}

}

#endif /* EMBEROGRETERRAINTECHNIQUESSHADERPASSCOVERAGEBATCH_H_ */
