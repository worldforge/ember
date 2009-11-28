/*
 * Shader.h
 *
 *  Created on: 26 nov 2009
 *      Author: erik
 */

#ifndef EMBEROGRETERRAINTECHNIQUESSHADER_H_
#define EMBEROGRETERRAINTECHNIQUESSHADER_H_

#include "Base.h"
#include "components/ogre/OgreIncludes.h"

namespace EmberOgre
{

namespace Terrain
{

namespace Techniques
{

class ShaderPass;

/**
	@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class Shader : public Base
{
public:
    /**
     * @brief Ctor.
     * @param includeShadows If true, shadows will be used.
     */
	Shader(bool includeShadows, const TerrainPageGeometry& geometry, const SurfaceLayerStore& terrainPageSurfaces, const TerrainPageShadow* terrainPageShadow, const TerrainPage& page);
    virtual ~Shader();

    virtual bool prepareMaterial();
    virtual bool compileMaterial(Ogre::MaterialPtr material);

protected:
	typedef std::vector<ShaderPass*> PassStore;

	virtual ShaderPass* addPass(Ogre::Technique* technique);
	PassStore mPasses;

	/**
	 * @brief Sets whether shadows should be used.
	 */
	bool mIncludeShadows;

	void addBaseLayer(Ogre::Pass* pass, TerrainPageSurfaceLayer* layer);
	void addLayer(Ogre::Pass* pass, TerrainPageSurfaceLayer* layer);

	void reset();

};


}

}

}

#endif /* EMBEROGRETERRAINTECHNIQUESSHADER_H_ */
