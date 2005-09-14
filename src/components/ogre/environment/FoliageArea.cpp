//
// C++ Implementation: FoliageArea
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2005
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
#include "FoliageArea.h"

#include "services/EmberServices.h"
#include "services/logging/LoggingService.h"
#include "services/config/ConfigService.h"
#include "components/ogre/EmberOgre.h"
#include "components/ogre/TerrainGenerator.h"
#include "Foliage.h"

#define OFFSET_PARAM 999

namespace EmberOgre {

	Ogre::Real FoliageArea::mXinc = Ogre::Math::PI * 0.004;
	Ogre::Real FoliageArea::mZinc = Ogre::Math::PI * 0.0055;

FoliageArea::FoliageArea()
{
	Ogre::Real mXpos = Ogre::Math::RangeRandom(-Ogre::Math::PI, Ogre::Math::PI);
	Ogre::Real mZpos = Ogre::Math::RangeRandom(-Ogre::Math::PI, Ogre::Math::PI);
}


FoliageArea::~FoliageArea()
{
}

void FoliageArea::setVisible(bool visible)
{
	mVisible = visible;
	//mGround->getSceneNode()->setVisible(mVisible);
}

	void FoliageArea::waveGrass(Ogre::Real timeElapsed)
	{

		mXpos += mXinc * timeElapsed;
		mZpos += mZinc * timeElapsed;

		// Update vertex program parameters by binding a value to each renderable
		static Ogre::Vector4 offset(0,0,0,0);

		Ogre::StaticGeometry::RegionIterator rit =  mStaticGeom->getRegionIterator();
		while (rit.hasMoreElements())
		{
			Ogre::StaticGeometry::Region* reg = rit.getNext();

			// a little randomness
			mXpos += reg->getCentre().x * 0.000001;
			mZpos += reg->getCentre().z * 0.000001;
			offset.x = Ogre::Math::Sin(mXpos);
			offset.z = Ogre::Math::Sin(mZpos);

			Ogre::StaticGeometry::Region::LODIterator lodit = reg->getLODIterator();
			while (lodit.hasMoreElements())
			{
				Ogre::StaticGeometry::LODBucket* lod = lodit.getNext();
				Ogre::StaticGeometry::LODBucket::MaterialIterator matit = 
					lod->getMaterialIterator();
				while (matit.hasMoreElements())
				{
					Ogre::StaticGeometry::MaterialBucket* mat = matit.getNext();
					Ogre::StaticGeometry::MaterialBucket::GeometryIterator geomit = 
						mat->getGeometryIterator();
					while (geomit.hasMoreElements())
					{
						Ogre::StaticGeometry::GeometryBucket* geom = geomit.getNext();
						geom->setCustomParameter(OFFSET_PARAM, offset);

					}
				}
			}
		}

	}

	
	

void FoliageArea::init(Foliage* foliage, Ogre::SceneManager* sceneManager, const std::string& name)
{
	mFoliage = foliage;
	mSceneMgr = sceneManager;

	mStaticGeom = mSceneMgr->createStaticGeometry(name);
	mStaticGeom->setRegionDimensions(Ogre::Vector3(32,32, 32));
	// Set the region origin so the centre is at 0 world
	//s->setOrigin((endPosition - startPosition) / 2);
	mStaticGeom->setRenderingDistance(34);

	mStaticGeom->setRenderQueueGroup(Ogre::RENDER_QUEUE_7);

	
}


void FoliageArea::placeGrass(const std::string& type, const TerrainPosition& position)
{
	const Ogre::Vector3 scale(
		1, Ogre::Math::RangeRandom(0.85, 1.15), 1);
	placeGrass(type, position, scale);
}


void FoliageArea::placeGrass(const std::string& type, const TerrainPosition& position, const Ogre::Vector3& scale)
{
	TerrainGenerator* terrain = EmberOgre::getSingleton().getTerrainGenerator();
	Ogre::Entity* currentEnt;
	currentEnt = mFoliage->getEntity(type);
	if (currentEnt) {
		Ogre::Vector3 ogrePosition = Atlas2Ogre(position);
		ogrePosition.y = terrain->getHeight(position);
		
		Ogre::Quaternion orientation;
		orientation.FromAngleAxis(
			Ogre::Degree(Ogre::Math::RangeRandom(0, 359)),
			Ogre::Vector3::UNIT_Y);
		mStaticGeom->addEntity(currentEnt, ogrePosition, orientation, scale);
	}


}

void FoliageArea::build()
{
	assert(mStaticGeom);
	try {
		mStaticGeom->build();
	} catch (Ogre::Exception& e) {
		S_LOG_FAILURE("Got error when building static geometry for foliage. Expection: " << e.getFullDescription());
	}
	Ogre::Root::getSingleton().addFrameListener(this);
}

//This is all very quick and messy, to be replaced by something better in the future
void FoliageArea::generateUnderVegetation(TerrainPosition minExtent, TerrainPosition maxExtent)
{
/*	mExtentMin = minExtent;
	mExtentMax = maxExtent;

	Ogre::Vector3 startPosition = Atlas2Ogre(minExtent);
	Ogre::Vector3 endPosition = Atlas2Ogre(maxExtent);





	Ember::ConfigService* configSrv = Ember::EmberServices::getInstance()->getConfigService();
	int submeshSize = (int)configSrv->getValue("foliage", "submeshsize");
	double grassSpacing = (double)configSrv->getValue("foliage", "spacing_grass");
	double bushSpacing = (double)configSrv->getValue("foliage", "spacing_bushes");
	double cullDistance = (double)configSrv->getValue("foliage", "cullingdistance");
	
	

	TerrainGenerator* terrain = EmberOgre::getSingleton().getTerrainGenerator();

	//we need to calculate the center of the foliage patch
	//Ogre::Vector3 center((endPosition.x + startPosition.x) * 0.5,0,(startPosition.z + endPosition.z) * 0.5);
	
	double spaceBetween = grassSpacing;
	
	for (double i = startPosition.x; i < endPosition.x; i = i + spaceBetween) {
		for (double j = endPosition.z; j < startPosition.z; j = j + spaceBetween) {
			Ogre::Real xPos = i + Ogre::Math::RangeRandom(-spaceBetween, spaceBetween);
			Ogre::Real zPos = j + Ogre::Math::RangeRandom(-spaceBetween, spaceBetween);
			Ogre::Real random = Ogre::Math::UnitRandom();
			Ogre::String typeOfGrass;
			Ogre::Entity* currentEnt;
			if (random > 0.95) {
				currentEnt = mFoliage->getEntity(1);
			} else if (random > 0.9) {
				currentEnt = mFoliage->getEntity(2);
			} else if (random > 0.85) {
				currentEnt = mFoliage->getEntity(3);
			} else {
				currentEnt = mFoliage->getEntity(4);
			}
			currentEnt = mFoliage->getEntity(0);
			
			//currentEnt = e;

			TerrainPosition atlas_pos = Ogre2Atlas_TerrainPosition(Ogre::Vector3(xPos,0, zPos));
			Ogre::Vector3 pos(xPos, terrain->getHeight(atlas_pos), zPos);

			Ogre::Quaternion orientation;
			orientation.FromAngleAxis(
				Ogre::Degree(Ogre::Math::RangeRandom(0, 359)),
				Ogre::Vector3::UNIT_Y);
			Ogre::Vector3 scale(
				1, Ogre::Math::RangeRandom(0.85, 1.15), 1);
			mStaticGeom->addEntity(currentEnt, pos, orientation, scale);
			//std::cout << "Added entity at: " << pos;
			
		}
	}*/
	
// 	spaceBetween = bushSpacing;
// 	for (double i = startPosition.x; i < endPosition.x; i = i + spaceBetween) {
// 		for (double j = endPosition.z; j < startPosition.z; j = j + spaceBetween) {
// 			Ogre::Real xPos = i + Ogre::Math::RangeRandom(-spaceBetween, spaceBetween);
// 			Ogre::Real zPos = j + Ogre::Math::RangeRandom(-spaceBetween, spaceBetween);
// 			Ogre::Real random = Ogre::Math::UnitRandom();
// 			Ogre::String typeOfGrass;
// 			if (random > 0.9) {
// 				typeOfGrass = "heartblood";
// 			} else if (random > 0.8) {
// 				typeOfGrass = "teardrops";
// 			} else if (random > 0.5) {
// 				typeOfGrass = "thingrass";
// 			} else {
// 				typeOfGrass = "bittergrass";
// 			}
// 			GroundCover::InstanceData* instance = mGround->add(std::string("environment/field/patch_01/") + typeOfGrass + "/normal.mesh" , std::string("environment/field/patch_01/") + typeOfGrass + "/low.mesh");
// 			TerrainPosition pos = Ogre2Atlas_TerrainPosition(Ogre::Vector3(xPos,0, zPos));
// 			instance->vPos = Ogre::Vector3(xPos, terrain->getHeight(pos), zPos);
// 			Ogre::Vector3 scale = Ogre::Vector3::UNIT_SCALE * Ogre::Math::RangeRandom(0.8f, 1.0f);
// 			//make the patches a bit smaller
// 			instance->vScale = scale * 0.5;
// 			Ogre::Quaternion rotation;
// 			rotation.FromAngleAxis((Ogre::Degree)Ogre::Math::RangeRandom(0.0f, 360.0f), Ogre::Vector3::UNIT_Y);
// 			instance->qOrient = rotation;
// 		}
// 	}

	mStaticGeom->build();
/*	mGround->setCullParameters(cullDistance, cullDistance, 120);
	mGround->compile();
	mGround->update(mCamera);*/
	
	
/*	Ogre::SceneNode* groundNode = mGround->getSceneNode();
	
	EmberOgre::getSingleton().getWorldSceneNode()->addChild(groundNode);
	
	
*/
	Ogre::Root::getSingleton().addFrameListener(this);
		
}

bool FoliageArea::frameStarted(const Ogre::FrameEvent & evt)
{	
	waveGrass(evt.timeSinceLastFrame);

	return true;
/*	if (mVisible) {
	}*/
	return true;


}






// void FoliageArea::waveGrass(Ogre::Real timeElapsed)
// {
// 	static Ogre::Real xinc = Ogre::Math::PI * 0.4;
// 	static Ogre::Real zinc = Ogre::Math::PI * 0.55;
// 	static Ogre::Real xpos = Ogre::Math::RangeRandom(-Ogre::Math::PI, Ogre::Math::PI);
// 	static Ogre::Real zpos = Ogre::Math::RangeRandom(-Ogre::Math::PI, Ogre::Math::PI);
// 
// 	xpos += xinc * timeElapsed;
// 	zpos += zinc * timeElapsed;
// 
// 	// Update vertex program parameters by binding a value to each renderable
// 	static Ogre::Vector4 offset(0,0,0,0);
// 
// 	Ogre::StaticGeometry::RegionIterator rit =  mStaticGeom->getRegionIterator();
// 	while (rit.hasMoreElements())
// 	{
// 		Ogre::StaticGeometry::Region* reg = rit.getNext();
// 
// 		// a little randomness
// 		xpos += reg->getCentre().x * 0.001;
// 		zpos += reg->getCentre().z * 0.001;
// 		offset.x = Ogre::Math::Sin(xpos) * 0.05;
// 		offset.z = Ogre::Math::Sin(zpos) * 0.05;
// 
// 		Ogre::StaticGeometry::Region::LODIterator lodit = reg->getLODIterator();
// 		while (lodit.hasMoreElements())
// 		{
// 			Ogre::StaticGeometry::LODBucket* lod = lodit.getNext();
// 			Ogre::StaticGeometry::LODBucket::MaterialIterator matit = 
// 				lod->getMaterialIterator();
// 			while (matit.hasMoreElements())
// 			{
// 				Ogre::StaticGeometry::MaterialBucket* mat = matit.getNext();
// 				Ogre::StaticGeometry::MaterialBucket::GeometryIterator geomit = 
// 					mat->getGeometryIterator();
// 				while (geomit.hasMoreElements())
// 				{
// 					Ogre::StaticGeometry::GeometryBucket* geom = geomit.getNext();
// 					geom->setCustomParameter(OFFSET_PARAM, offset);
// 
// 				}
// 			}
// 		}
// 	}
// 
// }


}
