//
// C++ Implementation: Foliage
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2004
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
#include "services/EmberServices.h"
#include "services/logging/LoggingService.h"
#include "services/config/ConfigService.h"

#include "Foliage.h"

#include "GroundCover.h"
#include "components/ogre/EmberOgre.h"
#include "components/ogre/TerrainGenerator.h"


namespace EmberOgre {

Foliage::Foliage(Ogre::Camera* camera, Ogre::SceneManager* sceneMgr)
: mCamera(camera), mSceneMgr(sceneMgr)
{
}


Foliage::~Foliage()
{
}

void Foliage::setVisible(bool visible)
{
	mVisible = visible;
	mGround->getSceneNode()->setVisible(mVisible);
}



//This is all very quick and messy, to be replaced by something better in the future
void Foliage::generateUnderVegetation(long segmentXStart, long segmentZStart, long numberOfSegments)
{
	Ember::ConfigService* configSrv = Ember::EmberServices::getInstance()->getConfigService();
	int submeshSize = (int)configSrv->getValue("foliage", "submeshsize");
	double grassSpacing = (double)configSrv->getValue("foliage", "spacing_grass");
	double bushSpacing = (double)configSrv->getValue("foliage", "spacing_bushes");
	double cullDistance = (double)configSrv->getValue("foliage", "cullingdistance");

	Ogre::Root::getSingleton().addFrameListener(this);

	TerrainGenerator* terrain = EmberOgre::getSingleton().getTerrainGenerator();
	long xStart = segmentXStart * 64;
	long zStart = segmentZStart * 64;
	
	mGround = new GroundCover(mSceneMgr, Ogre::Vector3(numberOfSegments * 64,0,  numberOfSegments * 64), submeshSize, Ogre::Vector3(0,0,0));
	
	double spaceBetween = grassSpacing;
	
	long i_end = xStart + numberOfSegments * 64;
	long j_end = zStart + numberOfSegments * 64;
	for (double i = xStart; i < i_end; i = i + spaceBetween) {
		for (double j = zStart; j < j_end; j = j + spaceBetween) {
			Ogre::Real xPos = i + Ogre::Math::RangeRandom(-spaceBetween, spaceBetween);
			Ogre::Real zPos = j + Ogre::Math::RangeRandom(-spaceBetween, spaceBetween);
			Ogre::Real random = Ogre::Math::UnitRandom();
			Ogre::String typeOfGrass;
			if (random > 0.9) {
				typeOfGrass = "heartblood";
			} else if (random > 0.8) {
				typeOfGrass = "teardrops";
			} else if (random > 0.7) {
				typeOfGrass = "thingrass";
			} else {
				typeOfGrass = "bittergrass";
			}
			GroundCover::InstanceData* instance = mGround->add(std::string("environment/field/small_plant/") + typeOfGrass + "/normal.mesh" , std::string("environment/field/small_plant/") + typeOfGrass + "/low.mesh");
			instance->vPos = Ogre::Vector3(xPos, terrain->getHeight(xPos,zPos), zPos);
			Ogre::Vector3 scale = Ogre::Vector3::UNIT_SCALE * Ogre::Math::RangeRandom(0.8f, 1.0f);
			instance->vScale = scale;
			Ogre::Quaternion rotation;
			rotation.FromAngleAxis((Ogre::Degree)Ogre::Math::RangeRandom(0.0f, 360.0f), Ogre::Vector3::UNIT_Y);
			instance->qOrient = rotation;
			
		}
	}
	
	spaceBetween = bushSpacing;
	i_end = xStart + numberOfSegments * 64;
	j_end = zStart + numberOfSegments * 64;
	for (double i = xStart; i < i_end; i = i + spaceBetween) {
		for (double j = zStart; j < j_end; j = j + spaceBetween) {
			Ogre::Real xPos = i + Ogre::Math::RangeRandom(-spaceBetween, spaceBetween);
			Ogre::Real zPos = j + Ogre::Math::RangeRandom(-spaceBetween, spaceBetween);
			Ogre::Real random = Ogre::Math::UnitRandom();
			Ogre::String typeOfGrass;
			if (random > 0.9) {
				typeOfGrass = "heartblood";
			} else if (random > 0.8) {
				typeOfGrass = "teardrops";
			} else if (random > 0.5) {
				typeOfGrass = "thingrass";
			} else {
				typeOfGrass = "bittergrass";
			}
			GroundCover::InstanceData* instance = mGround->add(std::string("environment/field/patch_01/") + typeOfGrass + "/normal.mesh" , std::string("environment/field/patch_01/") + typeOfGrass + "/low.mesh");
			instance->vPos = Ogre::Vector3(xPos, terrain->getHeight(xPos,zPos), zPos);
			Ogre::Vector3 scale = Ogre::Vector3::UNIT_SCALE * Ogre::Math::RangeRandom(0.8f, 1.0f);
			//make the patches a bit smaller
			instance->vScale = scale * 0.5;
			Ogre::Quaternion rotation;
			rotation.FromAngleAxis((Ogre::Degree)Ogre::Math::RangeRandom(0.0f, 360.0f), Ogre::Vector3::UNIT_Y);
			instance->qOrient = rotation;
		}
	}

	mGround->setCullParameters(cullDistance, cullDistance, 120);
	mGround->compile();
	mGround->update(mCamera);
		
}

bool Foliage::frameStarted(const Ogre::FrameEvent & evt)
{	
	if (mVisible) {
		mGround->update(mCamera);
	}
	return true;


}




};
