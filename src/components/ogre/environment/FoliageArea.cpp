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

FoliageArea::FoliageArea() :
mFoliage(0),
mSceneMgr(0), 
mStaticGeom(0), 
mVisible(false)
{
	Ogre::Real mXpos = Ogre::Math::RangeRandom(-Ogre::Math::PI, Ogre::Math::PI);
	Ogre::Real mZpos = Ogre::Math::RangeRandom(-Ogre::Math::PI, Ogre::Math::PI);
}


FoliageArea::~FoliageArea()
{
	if (mStaticGeom) {
		mSceneMgr->destroyStaticGeometry(mStaticGeom);
	}
}

void FoliageArea::setVisible(bool visible)
{
	mVisible = visible;
	if (mStaticGeom) {
		mStaticGeom->setVisible(visible);
	}
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
		setVisible(true);
		
	} catch (const Ogre::Exception& e) {
		S_LOG_FAILURE("Got error when building static geometry for foliage. Expection: " << e.getFullDescription());
	}
}


void FoliageArea::frameStarted(const Ogre::Real & timeElapsed)
{	
	//don't wave if we're not visible, not entirely sure that this won't look strange, I don't think so
	if (mVisible) {
		waveGrass(timeElapsed);
	}

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
