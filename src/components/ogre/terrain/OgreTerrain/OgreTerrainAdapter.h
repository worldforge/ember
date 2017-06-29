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

#include <sigc++/signal.h>
#include <memory>

namespace Ogre {
class TerrainGlobalOptions;

class TerrainGroup;

class TerrainPaging;

class PageManager;

class PagedWorld;
}

namespace Ember {
namespace OgreView {
namespace Terrain {

class EmberTerrainGroup;

class FlatTerrainDefiner;

class OgreTerrainMaterialGeneratorEmber;

class CameraFocusedGrid2DPageStrategy;

/**
 * @brief A ITerrainAdapter implementation which connects to and sets up the Ogre Terrain component.
 */
class OgreTerrainAdapter : public ITerrainAdapter {
public:
	OgreTerrainAdapter(Ogre::SceneManager& sceneManager, Ogre::Camera* mainCamera, unsigned int terrainPageSize = 257);

	virtual ~OgreTerrainAdapter();

	int getPageSize() override;

	void setPageSize(unsigned int pageSize) override;

	void setLoadRadius(Ogre::Real loadRadius) override;

	Ogre::Real getHeightAt(Ogre::Real x, Ogre::Real z) override;

	void setCamera(Ogre::Camera* camera) override;

	void loadScene() override;

	void reset() override;

	void reloadAllPages() override;

	void reloadPage(const TerrainIndex& index) override;

	void reloadPageMaterial(const TerrainIndex& index) override;

	std::string getDebugInfo() override;

	/**
	 * @brief Currently setPageDataProvider MUST be called before this. Otherwise a nullptr will be returned.
	 */
	ITerrainObserver* createObserver() override;

	void destroyObserver(ITerrainObserver* observer) override;

	std::pair<bool, Ogre::Vector3> rayIntersects(const Ogre::Ray& ray) const override;

	void setPageDataProvider(IPageDataProvider* pageDataProvider) override;

	sigc::connection bindTerrainShown(sigc::slot<void, const Ogre::TRect<Ogre::Real>>& signal) override;

private:
	Ogre::Real mLoadRadius;
	Ogre::Real mHoldRadius;

	Ogre::SceneManager& mSceneManager;

	/**
	 * @brief Signal emitted when a page has been shown for the first time.
	 * The argument is the area (in world coordinates) that was shown.
	 */
	sigc::signal<void, const Ogre::TRect<Ogre::Real>> mTerrainShownSignal;

	Ogre::TerrainMaterialGeneratorPtr mMaterialGenerator;
	Ogre::TerrainGlobalOptions* mTerrainGlobalOptions;
	Ogre::PageManager* mPageManager;
	Ogre::TerrainPaging* mTerrainPaging;
	Ogre::PagedWorld* mPagedWorld;
	Ogre::TerrainPagedWorldSection* mTerrainPagedWorldSection;
	OgreTerrainPageProvider mTerrainPageProvider;

	EmberTerrainGroup* mTerrainGroup;

	IPageDataProvider* mPageDataProvider;
	Ogre::TerrainMaterialGenerator::Profile* mMaterialProfile;
	CameraFocusedGrid2DPageStrategy* mPageStrategy;
};

} /* namespace Terrain */
} /* namespace OgreView */
} /* namespace Ember */
#endif /* OGRETERRAINADAPTER_H_ */
