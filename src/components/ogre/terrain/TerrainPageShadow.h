//
// C++ Interface: TerrainPageShadow
//
// Description:
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2007
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

#include <OgreTexture.h>
#include <OgreVector3.h>

namespace Ogre {
	class ColourValue;
	class Image;
	class Vector2;
}

namespace EmberOgre {
namespace Terrain {

class TerrainPage;
class TerrainPageGeometry;

class ITerrainPageShadowTechnique
{
public:
    virtual void createShadowData(const TerrainPage& page, const TerrainPageGeometry& geometry, unsigned char* data, const Ogre::Vector3& lightDirection, const Ogre::ColourValue& lightColour) = 0;

protected:
};

class SimpleTerrainPageShadowTechnique : public ITerrainPageShadowTechnique
{
public:
    virtual void createShadowData(const TerrainPage& page, const TerrainPageGeometry& geometry, unsigned char* data, const Ogre::Vector3& lightDirection, const Ogre::ColourValue& lightColour);

protected:
};

/**
	@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class TerrainPageShadow
{
public:
	TerrainPageShadow(const TerrainPage& terrainPage);

	virtual ~TerrainPageShadow();

	void setShadowTechnique(ITerrainPageShadowTechnique* shadowTechnique);

	void setLightDirection(const Ogre::Vector3& lightDirection);

	void createShadowData(const TerrainPageGeometry& geometry);

	Ogre::TexturePtr getTexture() const;
	void updateShadow(const TerrainPageGeometry& geometry);
	void createImage();

	Ogre::Image* getImage() const;

	void getShadowColourAt(const Ogre::Vector2& position, Ogre::uint32& colour) const;
	void getShadowColourAt(const Ogre::Vector2& position, Ogre::ColourValue& colour) const;


protected:
	const TerrainPage& mTerrainPage;
	Ogre::Vector3 mLightDirection;
	Ogre::Image* mImage;
	Ogre::TexturePtr mTexture;
	Ogre::MemoryDataStream* mShadowChunk;

	ITerrainPageShadowTechnique* mShadowTechnique;

};

}
}

#endif
