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


#include "services/EmberServices.h"
#include "services/config/ConfigService.h"


#include <Eris/Timeout.h>
#include <Eris/View.h>


#include "EmberEntity.h"
#include "model/Model.h"
#include "terrain/TerrainGenerator.h"
#include "terrain/TerrainShader.h"
#include "terrain/TerrainLayerDefinition.h"
#include "terrain/TerrainLayerDefinitionManager.h"
#include "WorldEmberEntity.h"
#include "environment/Foliage.h"
#include "environment/Environment.h"
#include "environment/CaelumEnvironment.h"
#include "environment/SimpleEnvironment.h"
#include "EmberOgre.h"
#include "Avatar.h"
#include "AvatarCamera.h"
#include "Convert.h"

#include <Mercator/Shader.h>
#include <Mercator/FillShader.h>
#include <Mercator/ThresholdShader.h>
#include <Mercator/DepthShader.h>
#include <Mercator/GrassShader.h>
#include <Mercator/ShaderFactory.h>

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
	sceneManager->getRootSceneNode()->addChild(getSceneNode());
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

	///set the position to always 0, 0, 0
	mOgreNode->setPosition(Ogre::Vector3(0, 0, 0));

	mEnvironment = new Environment::Environment(new Environment::CaelumEnvironment( EmberOgre::getSingleton().getSceneManager(), EmberOgre::getSingleton().getRenderWindow(), EmberOgre::getSingleton().getMainCamera()->getCamera()), new Environment::SimpleEnvironment(EmberOgre::getSingleton().getSceneManager(), EmberOgre::getSingleton().getRenderWindow(), EmberOgre::getSingleton().getMainCamera()->getCamera()));
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

TerrainParser::TerrainParser(Terrain::TerrainGenerator& terrainGenerator)
: mTerrainGenerator(terrainGenerator)
{
}


void TerrainParser::updateTerrain(const Atlas::Message::Element& terrain)
{
	//_fpreset();
	if (!terrain.isMap())
	{
		S_LOG_FAILURE("Terrain is not a map");
	}
	const Atlas::Message::MapType & tmap = terrain.asMap();
	Atlas::Message::MapType::const_iterator I = tmap.find("points");
	if (I == tmap.end())
	{
		S_LOG_FAILURE("No terrain points");
	}

	Terrain::TerrainGenerator::TerrainDefPointStore pointStore;
	if (I->second.isList())
	{
		// Legacy support for old list format.
		const Atlas::Message::ListType& plist = I->second.asList();
		Atlas::Message::ListType::const_iterator J = plist.begin();
		for (; J != plist.end(); ++J) {
			if (!J->isList()) {
				S_LOG_INFO("Non list in points");
				continue;
			}
			const Atlas::Message::ListType & point = J->asList();
			if (point.size() != 3) {
				S_LOG_INFO("Point without 3 nums.");
				continue;
			}

			Terrain::TerrainDefPoint defPoint(static_cast<int>(point[0].asNum()), static_cast<int>(point[1].asNum()), static_cast<float>(point[3].asNum()));
			pointStore.push_back(defPoint);
		}
	} else if (I->second.isMap()) {
		const Atlas::Message::MapType& plist = I->second.asMap();
		Atlas::Message::MapType::const_iterator J = plist.begin();
		for (; J != plist.end(); ++J) {
			if (!J->second.isList()) {
				S_LOG_INFO("Non list in points.");
				continue;
			}
			const Atlas::Message::ListType & point = J->second.asList();
			if (point.size() != 3) {
				S_LOG_INFO("Point without 3 nums.");
				continue;
			}
			int x = static_cast<int>(point[0].asNum());
			int y = static_cast<int>(point[1].asNum());
			float z = point[2].asNum();
			Terrain::TerrainDefPoint defPoint(x, y, z);
			pointStore.push_back(defPoint);
		}
	} else {
		S_LOG_FAILURE("Terrain is the wrong type");
		return;
	}
	mTerrainGenerator.updateTerrain(pointStore);

}

float extractFloat(const Atlas::Message::ListType& params, size_t position) {
	if (params.size() > position) {
		const Atlas::Message::Element& elem(params[position]);
		if (elem.isNum()) {
			return elem.asNum();
		}
	}
	return 0;
}

void TerrainParser::createShaders(const Atlas::Message::Element& surfaces)
{
	Terrain::TerrainLayerDefinitionManager& terrainManager = Terrain::TerrainLayerDefinitionManager::getSingleton();
	bool isValid = false;
	if (surfaces.isList())
	{
		const Atlas::Message::ListType & slist(surfaces.asList());
		for (Atlas::Message::ListType::const_iterator I = slist.begin(); I != slist.end(); ++I) {
			if (I->isMap()) {
				std::string name;
				std::string pattern;
				const Atlas::Message::MapType& surfaceMap(I->asMap());

				Mercator::Shader::Parameters params;
				if (surfaceMap.count("params")) {
					const Atlas::Message::Element& paramsElem(surfaceMap.find("params")->second);
					if (paramsElem.isMap()) {
						for (Atlas::Message::MapType::const_iterator J = paramsElem.asMap().begin(); J != paramsElem.asMap().end(); ++J) {
							if (J->second.isNum()) {
								params[J->first] = J->second.asNum();
							}
						}
					}
				}

				if (surfaceMap.count("name")) {
					const Atlas::Message::Element& nameElem(surfaceMap.find("name")->second);
					if (nameElem.isString()) {
						const std::string& name = nameElem.asString();
						Terrain::TerrainLayerDefinition* def(terrainManager.getDefinitionForShader(name));
						if (def) {
							if (surfaceMap.count("pattern")) {
								const Atlas::Message::Element& patternElem(surfaceMap.find("pattern")->second);
								if (patternElem.isString()) {
									const std::string& pattern = patternElem.asString();
									Mercator::Shader* shader = Mercator::ShaderFactories::instance().newShader(pattern, params);
									if (shader) {
										isValid = true;
										mTerrainGenerator.createShader(def, shader);
									}
								}
							}
						}
					}
				}
			}
		}
	}
	if (!isValid)
	{
		createDefaultShaders();
	}
}

void TerrainParser::createDefaultShaders()
{
	Terrain::TerrainLayerDefinitionManager& terrainManager = Terrain::TerrainLayerDefinitionManager::getSingleton();
	Terrain::TerrainLayerDefinition* def(0);
	if ((def = terrainManager.getDefinitionForShader("rock")))
	{
		mTerrainGenerator.createShader(def, new Mercator::FillShader());
	}
	if ((def = terrainManager.getDefinitionForShader("sand")))
	{
		mTerrainGenerator.createShader(def,  new Mercator::BandShader(-2.f, 1.5f));
	}

	if ((def = terrainManager.getDefinitionForShader("grass")))
	{
		mTerrainGenerator.createShader(def,   new Mercator::GrassShader(1.f, 80.f, .5f, 1.f));
	}
}


void WorldEmberEntity::adjustPositionForContainedNode(const EmberEntity& entity, const Ogre::Vector3& position)
{
	Ogre::SceneNode* sceneNode = entity.getSceneNode();

	if (entity.getPositioningMode() == EmberEntity::PM_FLOATING) {
		sceneNode->setPosition(position.x, 0,position.z);
	} else if (entity.getMovementMode() == EmberEntity::MM_SWIMMING) {
		///if it's swimming, make sure that it's between the sea bottom and the surface
		const TerrainPosition pos = Convert::toWF<TerrainPosition>(position);
		float height = 0;
		if (mTerrainGenerator->getHeight(pos, height)) {
			if (position.y < height) {
				sceneNode->setPosition(position.x, height,position.z);
			} else if (position.y > 0) {
				sceneNode->setPosition(position.x, 0,position.z);
			}
		}

	} else {
		///get the height from Mercator through the TerrainGenerator
/*		assert(mTerrainGenerator);
//		TerrainPosition pos(entity->getPredictedPos().x(), entity->getPredictedPos().y());
		TerrainPosition pos = Convert::toWF_TerrainPosition(position);
		float height = mTerrainGenerator->getHeight(pos);*/
		EmberEntity::adjustPositionForContainedNode(entity, position);
		//sceneNode->setPosition(getOffsetForContainedNode(position, entity));
	}

}

const Ogre::Vector3& WorldEmberEntity::getOffsetForContainedNode(const Ogre::Vector3& localPosition, const EmberEntity& entity)
{
	assert(mTerrainGenerator);
	///NOTE: won't work with threading!
	static Ogre::Vector3 offset = Ogre::Vector3::ZERO;
	float height = 0;
	if (mTerrainGenerator->getHeight(Convert::toWF<TerrainPosition>(localPosition), height)) {
		offset.y = height - localPosition.y;
	}
	return offset;

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
			mTerrainParser = std::auto_ptr<TerrainParser>(new TerrainParser(*mTerrainGenerator));
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
