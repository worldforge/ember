/*
 Copyright (C) 2004  Erik Hjortsberg

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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "EmberEntity.h"
#include "WorldEmberEntity.h"

#include "services/EmberServices.h"
#include "services/config/ConfigService.h"

#include "WorldAttachment.h"
#include "model/Model.h"
#include "terrain/TerrainParser.h"
#include "terrain/TerrainManager.h"
#include "terrain/TerrainShaderParser.h"
#include "terrain/TerrainDefPoint.h"
#include "environment/Foliage.h"
#include "environment/Environment.h"
#include "environment/CaelumEnvironment.h"
#include "environment/SimpleEnvironment.h"
#include "EmberOgre.h"
#include "Avatar.h"
#include "Convert.h"
#include "framework/Exception.h"

#include "TerrainPageDataProvider.h"
#include "components/ogre/SceneManagers/EmberPagingSceneManager/include/EmberPagingSceneManager.h"
#include "components/ogre/SceneManagers/EmberPagingSceneManager/include/EmberPagingSceneManagerAdapter.h"

#include <Eris/Timeout.h>
#include <Eris/View.h>

#include <OgreSceneManager.h>

namespace EmberOgre
{

WorldEmberEntity::WorldEmberEntity(const std::string& id, Eris::TypeInfo* ty, Eris::View* vw, Ogre::SceneManager* sceneManager) :
	EmberEntity(id, ty, vw, sceneManager), mTerrainManager(new Terrain::TerrainManager(new EmberPagingSceneManagerAdapter(static_cast<EmberPagingSceneManager*> (sceneManager)))), mFoliage(0), mEnvironment(0), mFoliageInitializer(0), mHasBeenInitialized(false), mPageDataProvider(new TerrainPageDataProvider(*mTerrainManager)), mSceneManager(static_cast<EmberPagingSceneManager*> (sceneManager))
{
	mSceneManager->registerProvider(mPageDataProvider);
	mWorldPosition.LatitudeDegrees = 0;
	mWorldPosition.LongitudeDegrees = 0;
	Ogre::SceneNode* worldNode = sceneManager->getRootSceneNode()->createChildSceneNode("entity_" + getId());
	if (worldNode) {
		setAttachment(new WorldAttachment(*this, *worldNode, *mTerrainManager));
	} else {
		throw Ember::Exception("Could not create world node.");
	}
	EmberOgre::getSingleton().EventTerrainManagerCreated.emit(*mTerrainManager);
}

WorldEmberEntity::~WorldEmberEntity()
{
	mSceneManager->registerProvider(0);
	delete mPageDataProvider;
	delete mFoliage;
	delete mEnvironment;
	delete mTerrainManager;
}

void WorldEmberEntity::init(const Atlas::Objects::Entity::RootEntity &ge, bool fromCreateOp)
{
	///create the foliage
	mFoliage = new Environment::Foliage(*mTerrainManager);
	EventFoliageCreated.emit();

	EmberEntity::init(ge, fromCreateOp);

	mEnvironment = new Environment::Environment(*mTerrainManager, new Environment::CaelumEnvironment(EmberOgre::getSingleton().getSceneManager(), EmberOgre::getSingleton().getRenderWindow(), *EmberOgre::getSingleton().getMainOgreCamera()), new Environment::SimpleEnvironment(EmberOgre::getSingleton().getSceneManager(), EmberOgre::getSingleton().getRenderWindow(), *EmberOgre::getSingleton().getMainOgreCamera()));
	EventEnvironmentCreated.emit();

	///we will wait with creating the terrain and initializing the environment until we've got a onVisibilityChanged call, since the Eris::Calendar functionality depends on the world entity object to be fully constructed and initialized to work. By waiting until onVisibilityChanged is called we guarantee that the Calendar will get the correct server time


}

void WorldEmberEntity::onAttrChanged(const std::string& str, const Atlas::Message::Element& v)
{
	//If the terrain hasn't been initialized yet (which will happen when the visibility changes, as it does right after the entity has been initialized) we'll ignore changes to the terrain, since it will be parsed anyway.
	if (str == "terrain" && !mHasBeenInitialized) {
		Eris::Entity::onAttrChanged(str, v);
	} else {
		EmberEntity::onAttrChanged(str, v);
	}
}

void WorldEmberEntity::onMoved()
{
	//It should never be possible to move the world, so we'll skip calling EmberEntity::onMoved() and go directly to Eris::Entity
	Eris::Entity::onMoved();
}

void WorldEmberEntity::onVisibilityChanged(bool vis)
{
	///we do our initialization of the terrain and environment here instead of at onInit since that way we can guarantee that Eris::Calendar will work as it should (which is used to get the correct server time)
	if (!mHasBeenInitialized) {
		mEnvironment->initialize();
		if (mTerrainManager) {
			mTerrainManager->setLightning(mEnvironment->getSun());
			bool hasValidShaders = false;
			Terrain::TerrainShaderParser terrainShaderParser(*mTerrainManager);
			if (hasAttr("terrain")) {
				Terrain::TerrainParser terrainParser;
				const Atlas::Message::Element& terrainElement = valueOfAttr("terrain");
				if (terrainElement.isMap()) {
					const Atlas::Message::MapType& terrainMap(terrainElement.asMap());
					if (terrainMap.count("surfaces")) {
						const Atlas::Message::Element& surfaceElement(terrainMap.find("surfaces")->second);
						terrainShaderParser.createShaders(surfaceElement);
						hasValidShaders = true;
					}
				}
				mTerrainManager->updateTerrain(terrainParser.parseTerrain(terrainElement, getPredictedPos()));
				if (!hasValidShaders) {
					terrainShaderParser.createDefaultShaders();
					hasValidShaders = true;
				}
			}

			if (!hasValidShaders) {
				terrainShaderParser.createDefaultShaders();
				hasValidShaders = true;
			}

		}

		///TODO: Parse world location data when it's available
		mEnvironment->setWorldPosition(mWorldPosition.LongitudeDegrees, mWorldPosition.LatitudeDegrees);

		///wait a little with initializing the foliage
		if (mFoliage) {
			mFoliageInitializer = std::auto_ptr<DelayedFoliageInitializer>(new DelayedFoliageInitializer(*mFoliage, *getView(), 1000, 15000));
		}
		mHasBeenInitialized = true;
	}

	Eris::Entity::onVisibilityChanged(vis);
}
void WorldEmberEntity::onLocationChanged(Eris::Entity *oldLocation)
{
	Eris::Entity::onLocationChanged(oldLocation);
}

void WorldEmberEntity::addArea(Terrain::TerrainArea& area)
{
	mTerrainManager->addArea(area);
}

void WorldEmberEntity::addTerrainMod(Terrain::TerrainMod* mod)
{
	mTerrainManager->addTerrainMod(mod);
}

float WorldEmberEntity::getHeight(const WFMath::Point<2>& localPosition) const
{
	//Note that there's no need to adjust the localPosition since the world always is at location 0,0,0 with not rotation
	float height = 0;

	if (mTerrainManager->getHeight(WFMath::Point<2>(localPosition.x(), localPosition.y()), height)) {
		return height;
	}
	return EmberEntity::getHeight(localPosition);
}

void WorldEmberEntity::updateTerrain(const std::vector<Terrain::TerrainDefPoint>& terrainDefinitionPoints)
{
	mTerrainManager->updateTerrain(terrainDefinitionPoints);
}

Environment::Environment* WorldEmberEntity::getEnvironment() const
{
	return mEnvironment;
}

Environment::Foliage* WorldEmberEntity::getFoliage() const
{
	return mFoliage;
}

Terrain::TerrainManager& WorldEmberEntity::getTerrainManager()
{
	return *mTerrainManager;
}

DelayedFoliageInitializer::DelayedFoliageInitializer(Environment::Foliage& foliage, Eris::View& view, unsigned int intervalMs, unsigned int maxTimeMs) :
	mFoliage(foliage), mView(view), mIntervalMs(intervalMs), mMaxTimeMs(maxTimeMs), mTimeout(new Eris::Timeout(intervalMs)), mTotalElapsedTime(0)
{
	///don't load the foliage directly, instead wait some seconds for all terrain areas to load
	///the main reason is that new terrain areas will invalidate the foliage causing a reload
	///by delaying the foliage we can thus in most cases avoid those reloads
	///wait three seconds
	mTimeout->Expired.connect(sigc::mem_fun(this, &DelayedFoliageInitializer::timout_Expired));

}

DelayedFoliageInitializer::~DelayedFoliageInitializer()
{
}

void DelayedFoliageInitializer::timout_Expired()
{
	///load the foliage if either all queues entities have been loaded, or 15 seconds has elapsed
	if (mView.lookQueueSize() == 0 || mTotalElapsedTime > mMaxTimeMs) {
		mFoliage.initialize();
	} else {
		mTotalElapsedTime += mIntervalMs;
		mTimeout->reset(mIntervalMs);
	}
}

}
