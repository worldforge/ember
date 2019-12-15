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
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef TERRAINPAGEDATAPROVIDER_H_
#define TERRAINPAGEDATAPROVIDER_H_

#include "domain/Types.h"
#include <OgrePrerequisites.h>
#include <OgreMaterial.h>

#include <memory>

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
struct IPageData
{
	virtual ~IPageData() = default;

	virtual Ogre::MaterialPtr getMaterial() = 0;
	virtual Ogre::MaterialPtr getCompositeMapMaterial() = 0;
};


/**
 * @brief Provides IPageData instances for the terrain rendering system.
 */
struct IPageDataProvider
{
	virtual ~IPageDataProvider() = default;
	//TODO SK: fix ogre index to be consistent
	typedef std::pair<long, long> OgreIndex;
	virtual std::unique_ptr<IPageData> getPageData(const OgreIndex& index) = 0;
	virtual int getPageIndexSize() const = 0;
	virtual void setUpTerrainPageAtIndex(const OgreIndex& ogreIndexPosition, std::shared_ptr<::Ember::OgreView::Terrain::ITerrainPageBridge> bridge) = 0;
	virtual void removeBridge(const OgreIndex& ogreIndexPosition) = 0;
};



/**
 * @brief Page data as provided by the Mercator based terrain system.
 */
class TerrainPageData : public IPageData
{
public:
	explicit TerrainPageData(Terrain::TerrainPage* page);

	~TerrainPageData() override = default;
	Ogre::MaterialPtr getMaterial() override;
	Ogre::MaterialPtr getCompositeMapMaterial() override;
private:
	Terrain::TerrainPage* mPage;

};

/**
 * @brief Provides terrain data from the Mercator based terrain system.
 */
class TerrainPageDataProvider : public IPageDataProvider
{
public:
	explicit TerrainPageDataProvider(Terrain::TerrainHandler& hander);
	~TerrainPageDataProvider() override = default;

	std::unique_ptr<IPageData> getPageData(const OgreIndex& ogreIndexPosition) override;
	int getPageIndexSize() const override;
	void setUpTerrainPageAtIndex(const OgreIndex& ogreIndexPosition, std::shared_ptr<Terrain::ITerrainPageBridge> bridge) override;
	void removeBridge(const OgreIndex& ogreIndexPosition) override;

protected:
	Terrain::TerrainHandler& mHandler;

	/**
	 * @brief Converts the ogre index position to wf local index.
	 */
	TerrainIndex convertToWFTerrainIndex(const OgreIndex& ogreIndexPosition);

};

}

}

#endif /* TERRAINPAGEDATAPROVIDER_H_ */
