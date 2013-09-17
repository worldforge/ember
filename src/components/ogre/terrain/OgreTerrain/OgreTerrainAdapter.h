/*
 Copyright (C) 2013 Samuel Kogler

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

#ifndef OGRETERRAINADAPTER_H_
#define OGRETERRAINADAPTER_H_

#include "../ITerrainAdapter.h"
#include "OgreTerrainPageProvider.h"

#include <OgreTerrainPagedWorldSection.h>

#include <memory>

namespace Ogre
{
class TerrainGlobalOptions;
class TerrainGroup;
class TerrainPaging;
class PageManager;
class PagedWorld;
}

namespace Ember
{
namespace OgreView
{
namespace Terrain
{

class FlatTerrainDefiner;
class OgreTerrainMaterialGeneratorEmber;
/**
 * @brief A ITerrainAdapter implementation which connects to and sets up the Ogre Terrain component.
 */
class OgreTerrainAdapter: public ITerrainAdapter
{
public:
	OgreTerrainAdapter(Ogre::SceneManager& sceneManager, Ogre::Camera* mainCamera, unsigned int terrainPageSize = 257);

	virtual ~OgreTerrainAdapter();

	virtual int getPageSize();

	virtual void setPageSize(unsigned int pageSize);

	virtual Ogre::Real getHeightAt(const Ogre::Real x, const Ogre::Real z);

	virtual void setWorldPagesDimensions(int numberOfPagesWidth, int numberOfPagesHeight, int widthOffsetInPages, int heightOffsetInPages);

	virtual void setCamera(Ogre::Camera* camera);

	virtual void setResourceGroupName(const std::string& groupName);

	virtual void loadOptions(const std::string& filePath);

	virtual void resize(Ogre::AxisAlignedBox newSize, int levels);

	virtual void loadScene();

	virtual void reset();

	virtual void setOption(const std::string& strKey, const void* pValue);

	virtual void getOption(const std::string& strKey, void* pDestValue);

	virtual void setUninitializedHeight(float height);

	virtual void reloadAllPages();

	virtual void reloadPage(const Domain::TerrainIndex& index);

	virtual void reloadPageMaterial(const Domain::TerrainIndex& index);

	virtual void loadFirstPage();

	virtual std::string getDebugInfo();

	virtual ITerrainObserver* createObserver();

	virtual void destroyObserver(ITerrainObserver* observer);

	virtual std::pair<bool, Ogre::Vector3> rayIntersects(const Ogre::Ray& ray) const;

	virtual void setPageDataProvider(IPageDataProvider* pageDataProvider);

private:
	Ogre::Real mLoadRadius;
	Ogre::Real mHoldRadius;

	Ogre::SceneManager& mSceneManager;

	Ogre::PageManager* mPageManager;
	Ogre::TerrainPaging* mTerrainPaging;
	Ogre::PagedWorld* mPagedWorld;
	Ogre::TerrainPagedWorldSection* mTerrainPagedWorldSection;
	OgreTerrainPageProvider mTerrainPageProvider;

	Ogre::TerrainGlobalOptions* mTerrainGlobalOptions;
	Ogre::TerrainGroup* mTerrainGroup;

	IPageDataProvider* mPageDataProvider;
	OgreTerrainMaterialGeneratorEmber* mMaterialGenerator;
};

class FlatTerrainDefiner: public Ogre::TerrainPagedWorldSection::TerrainDefiner
{
public:
	virtual void define(Ogre::TerrainGroup *terrainGroup, long x, long y) {
		terrainGroup->defineTerrain(x, y, &terrainGroup->getDefaultImportSettings());
		terrainGroup->loadTerrain(x, y, false);
	}
};


} /* namespace Terrain */
} /* namespace OgreView */
} /* namespace Ember */
#endif /* OGRETERRAINADAPTER_H_ */
