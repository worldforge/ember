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

#include <Mercator/Area.h>
#include <Mercator/FillShader.h>
#include <Mercator/ThresholdShader.h>
#include <Mercator/DepthShader.h>
#include <Mercator/GrassShader.h>
#include <Mercator/ShaderFactory.h>

namespace EmberOgre {
WorldEmberEntity::WorldEmberEntity(const std::string& id, Eris::TypeInfo* ty, Eris::View* vw, Ogre::SceneManager* sceneManager, Terrain::TerrainGenerator* terrainGenerator) : 
EmberEntity(id, ty, vw, sceneManager)
, mTerrainGenerator(terrainGenerator)
, mFoliage(0)
, mEnvironment(0)
, mTerrainParser(0)
, mFoliageInitializer(0)
, mHasBeenInitialized(false)
{
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
	// Need to detect D3D or GL for best depth shadowmapping
	bool isOpenGL;
	if (Ogre::Root::getSingleton().getRenderSystem()->getName().find("GL") != Ogre::String::npos)
	{
		isOpenGL = true;
	}
	else
	{
		isOpenGL = false;
	}

	Ogre::SceneManager* sceneMgr = EmberOgre::getSingleton().getSceneManager();
	sceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_ADDITIVE_INTEGRATED);

	sceneMgr->setShadowTextureCount(5);
	//sceneMgr->setShadowColour(Ogre::ColourValue(0.1, 0.1, 0.1));
	sceneMgr->setShadowTextureSelfShadow(true);

	sceneMgr->setShadowTextureCountPerLightType(Ogre::Light::LT_DIRECTIONAL, 3);
	sceneMgr->setShadowTextureCountPerLightType(Ogre::Light::LT_POINT, 1);
	sceneMgr->setShadowTextureCountPerLightType(Ogre::Light::LT_SPOTLIGHT, 1);

	// Set shadow far distance before creating any lights
	//sceneMgr->setShadowFarDistance(100);

	if (isOpenGL)
	{
		// GL performs much better if you pick half-float format
		sceneMgr->setShadowTexturePixelFormat(Ogre::PF_FLOAT16_R);
	}
	else
	{
		// D3D is the opposite - if you ask for PF_FLOAT16_R you
		// get an integer format instead! You can ask for PF_FLOAT16_GR
		// but the precision doesn't work well
		sceneMgr->setShadowTexturePixelFormat(Ogre::PF_FLOAT32_R);
	}
	sceneMgr->setShadowTextureSize(1024);

	sceneMgr->setShadowTextureCasterMaterial("Ogre/DepthShadowmap/Caster/Float");
	/*
	sceneMgr->setShadowTexturePixelFormat(PF_L8);
    sceneMgr->setShadowTextureCasterMaterial(StringUtil::BLANK);
    sceneMgr->setShadowTextureReceiverMaterial(StringUtil::BLANK);
	*/

	// Shadow camera setup
	//Ogre::LiSPSMShadowCameraSetup* liSPSMSetup = new Ogre::LiSPSMShadowCameraSetup();
	////liSPSMSetup->setUseAggressiveFocusRegion(false);

    Ogre::PSSMShadowCameraSetup* pssmSetup = new Ogre::PSSMShadowCameraSetup();

	Ogre::PSSMShadowCameraSetup::SplitPointList splitPointList = pssmSetup->getSplitPoints();;
	splitPointList[0] = 0.1;
	splitPointList[1] = 32.0;
	splitPointList[2] = 64.0;
	splitPointList[3] = 256.0;
	pssmSetup->setSplitPoints(splitPointList);
	//Ogre::Camera& camera = EmberOgre::getSingleton().getMainCamera()->getCamera();
	//pssmSetup->calculateSplitPoints(3, camera.getNearClipDistance(), camera.getFarClipDistance());

	pssmSetup->setUseAggressiveFocusRegion(false);

	//pssmSetup->setSplitPadding(5.0);

	// Set the LISPM adjustment factor (see API documentation for these)
	pssmSetup->setOptimalAdjustFactor(0, 2.0);
	pssmSetup->setOptimalAdjustFactor(1, 1.0);
	pssmSetup->setOptimalAdjustFactor(2, 0.5);

	sceneMgr->setShadowCameraSetup(Ogre::ShadowCameraSetupPtr(pssmSetup));

	///create the foliage
	mFoliage = new Environment::Foliage(EmberOgre::getSingleton().getSceneManager());
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

TerrainParser::TerrainParser(Terrain::TerrainGenerator* terrainGenerator)
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
	mTerrainGenerator->updateTerrain(pointStore);

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
										mTerrainGenerator->createShader(def, shader);
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
		mTerrainGenerator->createShader(def, new Mercator::FillShader());
	}
	if ((def = terrainManager.getDefinitionForShader("sand")))
	{
		mTerrainGenerator->createShader(def,  new Mercator::BandShader(-2.f, 1.5f));
	}
	
	if ((def = terrainManager.getDefinitionForShader("grass")))
	{
		mTerrainGenerator->createShader(def,   new Mercator::GrassShader(1.f, 80.f, .5f, 1.f));
	}
	
	//      createShader(std::string(configSrv->getValue("shadertextures", "snow")), new Mercator::HighShader(110.f)); // Snow
	//      createShader(std::string(configSrv->getValue("shadertextures", "seabottom")), new Mercator::DepthShader(0.f, -10.f)); // Underwater
	
	
	//    this->addShader(new TerrainShader(std::string(configSrv->getVariable("Shadertextures", "grass")), new Mercator::GrassShader(1.f, 80.f, .5f, 1.f))); // Grass
	

}


void WorldEmberEntity::adjustPositionForContainedNode(EmberEntity* const entity, const Ogre::Vector3& position)
{
	Ogre::SceneNode* sceneNode = entity->getSceneNode();
	
	if (entity->getMovementMode() == EmberEntity::MM_FLOATING) {
		sceneNode->setPosition(position.x, 0,position.z);
	} else if (entity->getMovementMode() == EmberEntity::MM_SWIMMING) {
		///if it's swimming, make sure that it's between the sea bottom and the surface
		const TerrainPosition pos = Ogre2Atlas_TerrainPosition(position);
		float height = mTerrainGenerator->getHeight(pos);
		if (position.y < height) {
			sceneNode->setPosition(position.x, height,position.z);
		} else if (position.y > 0) {
			sceneNode->setPosition(position.x, 0,position.z);
		}
		
	} else {
		///get the height from Mercator through the TerrainGenerator
/*		assert(mTerrainGenerator);
//		TerrainPosition pos(entity->getPredictedPos().x(), entity->getPredictedPos().y());
		TerrainPosition pos = Ogre2Atlas_TerrainPosition(position);
		float height = mTerrainGenerator->getHeight(pos);*/
		EmberEntity::adjustPositionForContainedNode(entity, position);
		//sceneNode->setPosition(getOffsetForContainedNode(position, entity));
	}

}

const Ogre::Vector3& WorldEmberEntity::getOffsetForContainedNode(const Ogre::Vector3& localPosition, EmberEntity* const entity)
{
	assert(mTerrainGenerator);
	///NOTE: won't work with threading!
	static Ogre::Vector3 offset = Ogre::Vector3::ZERO;
	float height = mTerrainGenerator->getHeight(Ogre2Atlas_TerrainPosition(localPosition));
	offset.y = height - localPosition.y;
	return offset;
	//return mTerrainGenerator->getHeight(position);

}


void WorldEmberEntity::onMoved(){
	Eris::Entity::onMoved();
}
//  void WorldEmberEntity::onTalk(const Atlas::Objects::Operation::RootOperation& talk)
//  {
//  	Eris::Entity::onTalk(talk);
//  }
//	virtual void setContainer(Entity *pr);
void WorldEmberEntity::onVisibilityChanged(bool vis)
{
	///we do our initialization of the terrain and environment here instead of at onInit since that way we can guarantee that Eris::Calendar will work as it should (which is used to get the correct server time)
	if (!mHasBeenInitialized) {
		mEnvironment->initialize();
		mTerrainParser = std::auto_ptr<TerrainParser>(new TerrainParser(mTerrainGenerator));
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
		
		//mTerrainGenerator->prepareSegments(0,0,1,true);
		
		///TODO: Parse world location data when it's available
		mEnvironment->setWorldPosition(mWorldPosition.LongitudeDegrees, mWorldPosition.LatitudeDegrees);
		
		///wait a little with initializing the foliage
		mFoliageInitializer = std::auto_ptr<DelayedFoliageInitializer>(new DelayedFoliageInitializer(mFoliage, getView(), 1000, 15000));
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

DelayedFoliageInitializer::DelayedFoliageInitializer(Environment::Foliage* foliage, Eris::View* view, unsigned int intervalMs, unsigned int maxTimeMs)
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
	if (mView->lookQueueSize() == 0 || mTotalElapsedTime > mMaxTimeMs) {
		if (mFoliage) {
			mFoliage->initialize();
		}
	} else {
		mTotalElapsedTime += mIntervalMs;
		mTimeout->reset(mIntervalMs);
	}
}



}
