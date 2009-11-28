/*
 * ShaderPass.h
 *
 *  Created on: 26 nov 2009
 *      Author: erik
 */

#ifndef EMBEROGRETERRAINTECHNIQUESSHADERPASS_H_
#define EMBEROGRETERRAINTECHNIQUESSHADERPASS_H_

#include "components/ogre/OgreIncludes.h"
#include <vector>
#include <string>

namespace EmberOgre
{

namespace Terrain
{
class TerrainPage;
class TerrainPageSurfaceLayer;
class TerrainPageGeometry;
class TerrainPageShadow;

namespace Techniques
{

class ShaderPassCoverageBatch;

typedef std::vector<const TerrainPageSurfaceLayer*> LayerStore;

class ShaderPass
{
public:
friend class ShaderPassCoverageBatch;
	ShaderPass(Ogre::Pass* pass, const TerrainPage& page);
	virtual ~ShaderPass();

	virtual void addLayer(const TerrainPageGeometry& geometry, const TerrainPageSurfaceLayer* layer);
	virtual void setBaseLayer(const TerrainPageSurfaceLayer* layer);
	void addShadowLayer(const TerrainPageShadow* terrainPageShadow);

	virtual bool hasRoomForLayer(const TerrainPageSurfaceLayer* layer);

	/**
	 * @brief Creates the combined final coverage textures and sets the shader params. Be sure to call this before you load the material.
	 * @param useShadows Whether to use shadows or not in the pass.
	 * @param shaderSuffix A suffix to add to the shader name. This allows you to make it use a somewhat different shader depending on graphics level etc.
	 * @return True if the creation of the pass was successful.
	 */
	virtual bool finalize(bool useShadows = true, const std::string shaderSuffix = "");

	LayerStore& getLayers();

	Ogre::Pass* getPass();

protected:
	typedef std::vector<ShaderPassCoverageBatch*> CoverageBatchStore;

	Ogre::Pass* mPass;
// 	unsigned int mCurrentLayerIndex;

	void assignCombinedCoverageTexture();
	ShaderPassCoverageBatch* getCurrentBatch();
	virtual ShaderPassCoverageBatch* createNewBatch();

	unsigned int getCoveragePixelWidth() const;
	float mScales[16];
	Ogre::TexturePtr getCombinedCoverageTexture(size_t passIndex, size_t batchIndex);
	CoverageBatchStore mCoverageBatches;
	LayerStore mLayers;
	const TerrainPageSurfaceLayer* mBaseLayer;
	const TerrainPage& mPage;
};
}

}

}

#endif /* EMBEROGRETERRAINTECHNIQUESSHADERPASS_H_ */
