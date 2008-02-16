//
// C++ Interface: TerrainPageShadow
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
#ifndef EMBEROGRETERRAINPAGESHADOW_H
#define EMBEROGRETERRAINPAGESHADOW_H
#include "../EmberOgrePrerequisites.h"

namespace EmberOgre {
namespace Terrain {

class TerrainPage;
class TerrainGenerator;

class ITerrainPageShadowTechnique
{
public:
    virtual void createShadowData(TerrainPage& page, TerrainGenerator* generator, unsigned char* data, const Ogre::Vector3& lightDirection, const Ogre::ColourValue& lightColour) = 0;

protected:
};

class SimpleTerrainPageShadowTechnique : public ITerrainPageShadowTechnique
{
public:
    virtual void createShadowData(TerrainPage& page, TerrainGenerator* generator, unsigned char* data, const Ogre::Vector3& lightDirection, const Ogre::ColourValue& lightColour);

protected:
};

/**
	@author Erik Hjortsberg <erik.hjortsberg@iteam.se>
*/
class TerrainPageShadow
{
public:
    TerrainPageShadow(TerrainPage& terrainPage);

    virtual ~TerrainPageShadow();
    
    void setShadowTechnique(ITerrainPageShadowTechnique* shadowTechnique);
    
    void setLightDirection(const Ogre::Vector3& lightDirection);
    
    void createShadowData(unsigned char* data);
    
    Ogre::TexturePtr getTexture();
	void updateShadow();
	void createImage();
	
	Ogre::Image* getImage() const;
   
protected:
	TerrainPage& mTerrainPage;
	Ogre::Vector3 mLightDirection;
	Ogre::Image* mImage;
	Ogre::TexturePtr mTexture;
	Ogre::MemoryDataStream* mShadowChunk;
	
	ITerrainPageShadowTechnique* mShadowTechnique;
	
};

}
}

#endif
