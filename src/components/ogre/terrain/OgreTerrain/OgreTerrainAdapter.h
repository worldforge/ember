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
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef OGRETERRAINADAPTER_H_
#define OGRETERRAINADAPTER_H_

#include "../ITerrainAdapter.h"
#include "OgreTerrainPageProvider.h"

#include <OgreTerrainPagedWorldSection.h>

#include <sigc++/signal.h>
#include <memory>
#include <boost/noncopyable.hpp>

namespace Ogre {
class TerrainGlobalOptions;

class TerrainGroup;

class TerrainPaging;

class PageManager;

class PagedWorld;
}

namespace Ember {
class EmberEntity;
namespace OgreView {
namespace Terrain {

class EmberTerrainGroup;

class FlatTerrainDefiner;

class OgreTerrainMaterialGeneratorEmber;

class CameraFocusedGrid2DPageStrategy;

/**
 * @brief A ITerrainAdapter implementation which connects to and sets up the Ogre Terrain component.
 */
class OgreTerrainAdapter : public ITerrainAdapter, public boost::noncopyable {
public:
	OgreTerrainAdapter(Ogre::SceneManager& sceneManager, int terrainPageSize);

	~OgreTerrainAdapter() override;

	int getPageSize() override;

	void setPageSize(int pageSize) override;

	void setLoadRadius(Ogre::Real loadRadius) override;

	bool getHeightAt(Ogre::Real x, Ogre::Real z, float& height) override;

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
	std::unique_ptr<ITerrainObserver> createObserver() override;

	std::pair<EmberEntity*, Ogre::Vector3> rayIntersects(const Ogre::Ray& ray) const override;

	void setPageDataProvider(IPageDataProvider* pageDataProvider) override;

	sigc::connection bindTerrainShown(sigc::slot<void, const Ogre::TRect<Ogre::Real>>& signal) override;

	void setTerrainEntity(EmberEntity* entity) override;

private:
	Ogre::Real mLoadRadius;
	Ogre::Real mHoldRadius;

	/**
	 * @brief Signal emitted when a page has been shown for the first time.
	 * The argument is the area (in world coordinates) that was shown.
	 */
	sigc::signal<void, const Ogre::TRect<Ogre::Real>&> mTerrainShownSignal;

	/**
	 * @brief Signal emitted when an area of the terrain has been updated.
	 * The argument is the area (in world coordinates) that was updated.
	 */
	sigc::signal<void, const Ogre::TRect<Ogre::Real>&> mTerrainAreaUpdated;

	Ogre::TerrainMaterialGeneratorPtr mMaterialGenerator;
	std::unique_ptr<Ogre::TerrainGlobalOptions> mTerrainGlobalOptions;
	std::unique_ptr<Ogre::PageManager> mPageManager;
	std::unique_ptr<Ogre::TerrainPaging> mTerrainPaging;
	Ogre::PagedWorld* mPagedWorld;
	Ogre::TerrainPagedWorldSection* mTerrainPagedWorldSection;
	OgreTerrainPageProvider mTerrainPageProvider;

	/**
	 * Ownership is passed to the Ogre terrain engine.
	 */
	Ogre::TerrainGroup* mTerrainGroup;

	IPageDataProvider* mPageDataProvider;
	std::unique_ptr<Ogre::TerrainMaterialGenerator::Profile> mMaterialProfile;
	std::unique_ptr<CameraFocusedGrid2DPageStrategy> mPageStrategy;
	EmberEntity* mEntity;


	void setOgrePageSize(int pageSize);
};

} /* namespace Terrain */
} /* namespace OgreView */
} /* namespace Ember */
#endif /* OGRETERRAINADAPTER_H_ */
