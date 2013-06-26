/*
 Copyright (C) 2009 erik

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

#ifndef TERRAINPAGEDATAPROVIDER_H_
#define TERRAINPAGEDATAPROVIDER_H_

#include "domain/Types.h"
#include <OgrePrerequisites.h>
#include <OgreMaterial.h>

namespace Ember
{
namespace OgreView
{

namespace Terrain
{
class TerrainHandler;
class TerrainPage;
class ITerrainPageBridge;
}

/**
 * @brief Page data for one page in the terrain rendering system.
 */
class IPageData
{
public:
	virtual ~IPageData() {}
	virtual Ogre::MaterialPtr getMaterial() = 0;
};


/**
 * @brief Provides IPageData instances for the terrain rendering system.
 */
class IPageDataProvider
{
public:
	virtual ~IPageDataProvider() {}
	typedef std::pair<unsigned int, unsigned int> OgreIndex;
	virtual IPageData* getPageData(const OgreIndex& index) = 0;
	virtual int getPageIndexSize() const = 0;
	virtual void setUpTerrainPageAtIndex(const OgreIndex& ogreIndexPosition, ::Ember::OgreView::Terrain::ITerrainPageBridge* bridge) = 0;
	virtual void removeBridge(const OgreIndex& ogreIndexPosition) = 0;
};



/**
 * @brief Page data as provided by the Mercator based terrain system.
 */
class TerrainPageData : public IPageData
{
public:
	TerrainPageData(Terrain::TerrainPage* page);
	virtual ~TerrainPageData();
	virtual Ogre::MaterialPtr getMaterial();
private:
	Terrain::TerrainPage* mPage;

};

/**
 * @brief Provides terrain data from the Mercator based terrain system.
 */
class TerrainPageDataProvider : public IPageDataProvider
{
public:
	TerrainPageDataProvider(Terrain::TerrainHandler& hander);
	virtual ~TerrainPageDataProvider();

	virtual IPageData* getPageData(const OgreIndex& ogreIndexPosition);
	virtual int getPageIndexSize() const;
	virtual void setUpTerrainPageAtIndex(const OgreIndex& ogreIndexPosition, Terrain::ITerrainPageBridge* bridge);
	virtual void removeBridge(const OgreIndex& ogreIndexPosition);

protected:
	Terrain::TerrainHandler& mHandler;

	/**
	 * @brief Converts the ogre index position to wf local index.
	 */
	Domain::TerrainIndex convertToWFTerrainIndex(const OgreIndex& ogreIndexPosition);

};

}

}

#endif /* TERRAINPAGEDATAPROVIDER_H_ */
