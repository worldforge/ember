/*
 * ShaderNormalMappedPass.h
 *
 *  Created on: 26 nov 2009
 *      Author: erik
 */

#ifndef EMBEROGRETERRAINTECHNIQUESSHADERNORMALMAPPEDPASS_H_
#define EMBEROGRETERRAINTECHNIQUESSHADERNORMALMAPPEDPASS_H_

#include "ShaderPass.h"

namespace EmberOgre
{

namespace Terrain
{

class TerrainPageSurfaceLayer;

namespace Techniques
{

class ShaderPassCoverageBatch;

class ShaderNormalMappedPass : public ShaderPass
{
public:
	ShaderNormalMappedPass(Ogre::Pass* pass, const TerrainPage& page);
	virtual ~ShaderNormalMappedPass() {}

// 	virtual void addLayer(TerrainPageSurfaceLayer* layer);
// 	virtual void setBaseLayer(TerrainPageSurfaceLayer* layer);

	virtual bool hasRoomForLayer(const TerrainPageSurfaceLayer* layer);


	/**
	 *    Creates the combined final coverage textures and sets the shader params. Be sure to call this before you load the material.
	 */
	virtual bool finalize();


protected:
	virtual ShaderPassCoverageBatch* createNewBatch();

};

}

}

}

#endif /* EMBEROGRETERRAINTECHNIQUESSHADERNORMALMAPPEDPASS_H_ */
