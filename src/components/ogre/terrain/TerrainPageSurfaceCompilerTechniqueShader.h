//
// C++ Interface: TerrainPageSurfaceCompilerTechniqueShader
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@iteam.se>, (C) 2007
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//
#ifndef EMBEROGRE_TERRAINTERRAINPAGESURFACECOMPILERTECHNIQUESHADER_H
#define EMBEROGRE_TERRAINTERRAINPAGESURFACECOMPILERTECHNIQUESHADER_H

#include "../EmberOgrePrerequisites.h"
#include "TerrainPageSurfaceCompiler.h"
#include "TerrainPage.h"


namespace EmberOgre {

namespace Terrain {

class TerrainPageSurfaceLayer;
class TerrainPageShadow;

class TerrainPageSurfaceCompilerShaderPass
{
public:
	TerrainPageSurfaceCompilerShaderPass(Ogre::Pass* pass, Ogre::TexturePtr combinedCoverageTexture);
	
	void addLayer(TerrainPageSurfaceLayer* layer);
	void addBaseLayer(TerrainPageSurfaceLayer* layer);
	void addShadowLayer(TerrainPageShadow* terrainPageShadow);
	
	bool hasRoomForLayer(TerrainPageSurfaceLayer* layer);
	
	
	/**
	 *    Creates the combined final coverage textures and sets the shader params. Be sure to call this before you load the material.
	 */
	void finalize();
	
protected:
	Ogre::Image* mCombinedCoverageImage;
	Ogre::MemoryDataStream* mCombinedCoverageDataStream;
	Ogre::DataStreamPtr mCombinedCoverageDataStreamPtr;
	
	Ogre::Pass* mPass;
	Ogre::TexturePtr mCombinedCoverageTexture;
	unsigned int mCurrentLayerIndex;
	
	void addCoverage(Ogre::Image* coverage, unsigned int channel, unsigned short numberOfChannels);
	void assignCombinedCoverageTexture();

	unsigned int getCoveragePixelWidth() const;
	float mScales[16];
	Ogre::TexturePtr getCombinedCoverageTexture(size_t passIndex);

};

/**
	@author Erik Hjortsberg <erik.hjortsberg@iteam.se>
*/
class TerrainPageSurfaceCompilerTechniqueShader : public TerrainPageSurfaceCompilerTechnique
{
public:
    TerrainPageSurfaceCompilerTechniqueShader();

    virtual ~TerrainPageSurfaceCompilerTechniqueShader();
    
	virtual void compileMaterial(Ogre::MaterialPtr material, std::map<int, TerrainPageSurfaceLayer*>& terrainPageSurfaces, TerrainPageShadow* terrainPageShadow);
    
    virtual void setPage(TerrainPage* page);

protected:
	typedef std::vector<TerrainPageSurfaceCompilerShaderPass*> PassStore;
	
	TerrainPage* mPage;
	Ogre::TexturePtr getCombinedCoverageTexture(size_t passIndex);
	TerrainPageSurfaceCompilerShaderPass* addPass(Ogre::Technique* technique);
	PassStore mPasses;
	
	void addBaseLayer(Ogre::Pass* pass, TerrainPageSurfaceLayer* layer);
	void addLayer(Ogre::Pass* pass, TerrainPageSurfaceLayer* layer);
	

};

}

}

#endif
