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
#include "terrain/TerrainGenerator.h"
#include "environment/Foliage.h"
#include "environment/Environment.h"
#include "environment/CaelumEnvironment.h"
#include "environment/SimpleEnvironment.h"
#include "EmberOgre.h"
#include "Avatar.h"
#include "Convert.h"
#include "framework/Exception.h"

#include <Mercator/Shader.h>
#include <Mercator/FillShader.h>
#include <Mercator/ThresholdShader.h>
#include <Mercator/DepthShader.h>
#include <Mercator/GrassShader.h>
#include <Mercator/ShaderFactory.h>

#include <Eris/Timeout.h>
#include <Eris/View.h>

#include <OgreSceneManager.h>

namespace EmberOgre {

WorldEmberEntity::WorldEmberEntity(const std::string& id, Eris::TypeInfo* ty, Eris::View* vw, Ogre::SceneManager* sceneManager) :
EmberEntity(id, ty, vw, sceneManager)
, mTerrainGenerator(0)
, mFoliage(0)
, mEnvironment(0)
, mTerrainParser(0)
, mFoliageInitializer(0)
, mHasBeenInitialized(false)
{
	mTerrainGenerator = EmberOgre::getSingleton().getTerrainGenerator();
	mWorldPosition.LatitudeDegrees = 0;
	mWorldPosition.LongitudeDegrees = 0;
	Ogre::SceneNode* worldNode = sceneManager->getRootSceneNode()->createChildSceneNode("entity_" + getId());
	if (worldNode) {
		setAttachment(new WorldAttachment(*this, *worldNode, *mTerrainGenerator));
	} else {
		throw Ember::Exception("Could not create world node.");
	}
//	sceneManager->getRootSceneNode()->addChild(getSceneNode());
}

WorldEmberEntity::~WorldEmberEntity()
{
	delete mFoliage;
	delete mEnvironment;
}



void WorldEmberEntity::init(const Atlas::Objects::Entity::RootEntity &ge, bool fromCreateOp)
{
	///create the foliage
	mFoliage = new Environment::Foliage();
	EventFoliageCreated.emit();

	EmberEntity::init(ge, fromCreateOp);

	mEnvironment = new Environment::Environment(new Environment::CaelumEnvironment( EmberOgre::getSingleton().getSceneManager(), EmberOgre::getSingleton().getRenderWindow(), *EmberOgre::getSingleton().getMainOgreCamera()), new Environment::SimpleEnvironment(EmberOgre::getSingleton().getSceneManager(), EmberOgre::getSingleton().getRenderWindow(), *EmberOgre::getSingleton().getMainOgreCamera()));
	EventEnvironmentCreated.emit();

	///we will wait with creating the terrain and initialing the environment until we've got a onVisibilityChanged call, since the Eris::Calendar functionality depends on the world entity object to be fully constructed and initialized to work. By waiting until onVisibilityChanged is called we guarantee that the Calendar will get the correct server time


}

void WorldEmberEntity::onAttrChanged(const std::string& str, const Atlas::Message::Element& v)
{
	///check for terrain updates
	if (str == "terrain")
	{
		if (mTerrainParser.get()) {
			mTerrainParser->updateTerrain(v);
		}
	}
	Entity::onAttrChanged(str, v);
}

void WorldEmberEntity::onMoved(){
	Eris::Entity::onMoved();
}

void WorldEmberEntity::onVisibilityChanged(bool vis)
{
	///we do our initialization of the terrain and environment here instead of at onInit since that way we can guarantee that Eris::Calendar will work as it should (which is used to get the correct server time)
	if (!mHasBeenInitialized) {
		mEnvironment->initialize();
		if (mTerrainGenerator) {
			mTerrainParser = std::auto_ptr<Terrain::TerrainParser>(new Terrain::TerrainParser(*mTerrainGenerator));
			bool hasValidShaders = false;
			if (hasAttr("terrain")) {
				const Atlas::Message::Element& terrainElement = valueOfAttr("terrain");
				if (terrainElement.isMap()) {
					const Atlas::Message::MapType& terrainMap(terrainElement.asMap());
					if (terrainMap.count("surfaces")) {
						const Atlas::Message::Element& surfaceElement(terrainMap.find("surfaces")->second);
						mTerrainParser->createShaders(surfaceElement);
						hasValidShaders = true;
					}
				}
				if (!hasValidShaders) {
					mTerrainParser->createDefaultShaders();
					hasValidShaders = true;
				}
				mTerrainParser->updateTerrain(terrainElement);
			}
			if (!hasValidShaders) {
				mTerrainParser->createDefaultShaders();
				hasValidShaders = true;
			}

			///prepare all the segments in advance
			mTerrainGenerator->prepareAllSegments();
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

void WorldEmberEntity::addArea(Terrain::TerrainArea* area)
{
	mTerrainGenerator->addArea(area);
}

void WorldEmberEntity::addTerrainMod(Terrain::TerrainMod* mod)
{
    mTerrainGenerator->addTerrainMod(mod);
}

Environment::Environment* WorldEmberEntity::getEnvironment() const
{
	return mEnvironment;
}

Environment::Foliage* WorldEmberEntity::getFoliage() const
{
	return mFoliage;
}

DelayedFoliageInitializer::DelayedFoliageInitializer(Environment::Foliage& foliage, Eris::View& view, unsigned int intervalMs, unsigned int maxTimeMs)
: mFoliage(foliage)
, mView(view)
, mIntervalMs(intervalMs)
, mMaxTimeMs(maxTimeMs)
, mTimeout(new Eris::Timeout(intervalMs))
, mTotalElapsedTime(0)
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
