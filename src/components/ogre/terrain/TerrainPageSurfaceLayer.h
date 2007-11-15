//
// C++ Interface: TerrainPageSurfaceLayer
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
#ifndef EMBEROGRETERRAINPAGESURFACELAYER_H
#define EMBEROGRETERRAINPAGESURFACELAYER_H

#include "../EmberOgrePrerequisites.h"
namespace Mercator
{
class Shader;
class Surface;
class Segment;
}

namespace Ogre
{
class Image;
}



namespace EmberOgre {
namespace Terrain {

class TerrainPageSurface;

/**
	@author Erik Hjortsberg <erik.hjortsberg@iteam.se>
*/
class TerrainPageSurfaceLayer{
public:
    TerrainPageSurfaceLayer(TerrainPageSurface& terrainPageSurface, int surfaceIndex, Mercator::Shader* shader);

    virtual ~TerrainPageSurfaceLayer();
    
    void updateCoverageImage();
    
    inline Ogre::Image* getCoverageImage();
    const std::string& getCoverageTextureName() const;
    
    const std::string& getDiffuseTextureName() const;
    void setDiffuseTextureName(const std::string& textureName);
    const std::string& getSpecularTextureName() const;
    void setSpecularTextureName(const std::string& textureName);
    const std::string& getNormalTextureName() const;
    void setNormalTextureName(const std::string& textureName);
    
    unsigned int getPixelWidth() const;
    
	bool intersects();
	
	Mercator::Shader* getShader();
	int getSurfaceIndex();
	Mercator::Surface* getSurfaceForSegment(Mercator::Segment* segment) const;

	float getScale() const;
	void setScale(float scale);

protected:
	TerrainPageSurface& mTerrainPageSurface;
	Mercator::Shader* mShader;
	Ogre::Image* mCoverageImage;
	Ogre::TexturePtr mTexture;
	
	std::string mDiffuseTextureName;
	std::string mSpecularTextureName;
	std::string mNormalTextureName;
	
	Ogre::MemoryDataStream* mCoverageDataStream;
	Ogre::DataStreamPtr mCoverageDataStreamPtr;
	int mSurfaceIndex;
	
	float mScale;
	

	void fillAlphaLayer(unsigned char* finalImagePtr, unsigned char* wfImagePtr, unsigned int channel, int startX, int startY, unsigned short numberOfChannels);

};

Ogre::Image* TerrainPageSurfaceLayer::getCoverageImage()
{
	return mCoverageImage;
}

}
}

#endif
