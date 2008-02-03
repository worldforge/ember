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
#include "Foliage.h"
#include "pagedgeometry/include/PagedGeometry.h"
#include "pagedgeometry/include/GrassLoader.h"
#include "pagedgeometry/include/BatchPage.h"
#include "pagedgeometry/include/BatchedGeometry.h"

#include "../AvatarCamera.h"
#include "../EmberOgre.h"
#include "../terrain/TerrainGenerator.h"
#include "../terrain/ISceneManagerAdapter.h"

using namespace PagedGeometry;
// #define OFFSET_PARAM 999

namespace EmberOgre {

namespace Environment {

// 	Ogre::Real FoliageArea::mXinc = Ogre::Math::PI * 0.004;
// 	Ogre::Real FoliageArea::mZinc = Ogre::Math::PI * 0.0055;

FoliageArea::FoliageArea(Foliage& foliage, Ogre::SceneManager& sceneManager, const std::string name) :
mName(name),
mSceneMgr(sceneManager),
mFoliage(foliage),
mVisible(false),
mGrass(0),
mGrass2(0),
mGrassLoader(0),
mGrassLoader2(0)
// mStaticGeom(0)
{
/*	mXpos = Ogre::Math::RangeRandom(-Ogre::Math::PI, Ogre::Math::PI);
	mZpos = Ogre::Math::RangeRandom(-Ogre::Math::PI, Ogre::Math::PI);*/
}


FoliageArea::~FoliageArea()
{
	delete mGrass;
	delete mGrass2;
	delete mGrassLoader;
	delete mGrassLoader2;

// 	if (mStaticGeom) {
// 		mSceneMgr.destroyStaticGeometry(mStaticGeom);
// 	}
}

void FoliageArea::setVisible(bool visible)
{
// 	mVisible = visible;
// 	if (mStaticGeom) {
// 		mStaticGeom->setVisible(visible);
// 	}
	//mGround->getSceneNode()->setVisible(mVisible);
}

// void FoliageArea::waveGrass(Ogre::Real timeElapsed)
// {
// 
// 	mXpos += mXinc * timeElapsed;
// 	mZpos += mZinc * timeElapsed;
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
// 		mXpos += reg->getCentre().x * 0.000001;
// 		mZpos += reg->getCentre().z * 0.000001;
// 		offset.x = Ogre::Math::Sin(mXpos);
// 		offset.z = Ogre::Math::Sin(mZpos);
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

	
	

void FoliageArea::init(const WFMath::AxisBox<2>& extent, Ogre::TexturePtr densityMap, Ogre::TexturePtr shadowMap)
{
	mExtent = extent;
	createGrass(densityMap, shadowMap);

/*	mStaticGeom = mSceneMgr.createStaticGeometry(name);
	mStaticGeom->setRegionDimensions(Ogre::Vector3(32,32, 32));
	// Set the region origin so the centre is at 0 world
	//s->setOrigin((endPosition - startPosition) / 2);
 	mStaticGeom->setRenderingDistance(34);

	mStaticGeom->setRenderQueueGroup(Ogre::RENDER_QUEUE_7);*/
}

void FoliageArea::createGrass(Ogre::TexturePtr densityMap, Ogre::TexturePtr shadowMap)
{
	TBounds bounds = Atlas2Ogre(mExtent);
// 	TBounds bounds(mExtent.lowCorner().x(),  mExtent.lowCorner().y(), mExtent.highCorner().x(),mExtent.highCorner().y());
	S_LOG_INFO("Creating grass for area: left: "<< bounds.left <<" right: " << bounds.right << " top: " << bounds.top << " bottom: " << bounds.bottom << ".");
	Ogre::Camera* camera = EmberOgre::getSingleton().getMainCamera()->getCamera();
// 	const WFMath::AxisBox<2>& worldSize = EmberOgre::getSingleton().getTerrainGenerator()->getTerrainInfo().getWorldSizeInIndices();
	
	//-------------------------------------- LOAD GRASS --------------------------------------
	//Create and configure a new PagedGeometry instance for grass
	
	mGrass = new ::PagedGeometry::PagedGeometry(camera, 32);
	mGrass->setBounds(bounds);
	mGrass->addDetailLevel<GrassPage>(100);
	
	//Create a GrassLoader object
	mGrassLoader = new GrassLoader(mGrass);
	mGrass->setPageLoader(mGrassLoader);	//Assign the "treeLoader" to be used to load geometry for the PagedGeometry instance

	mGrassLoader->setHeightFunction(&getTerrainHeight);

	//Add some grass to the scene with GrassLoader::addLayer()
	GrassLayer *l = mGrassLoader->addLayer("grass");

	//Configure the grass layer properties (size, density, animation properties, fade settings, etc.)
	l->setMinimumSize(1.0f, 1.0f);
	l->setMaximumSize(1.5f, 1.5f);
	l->setAnimationEnabled(true);		//Enable animations
	l->setSwayDistribution(10.0f);		//Sway fairly unsynchronized
	l->setSwayLength(0.5f);				//Sway back and forth 0.5 units in length
	l->setSwaySpeed(0.5f);				//Sway 1/2 a cycle every second
	l->setDensity(1.5f);				//Relatively dense grass
	l->setFadeTechnique(FADETECH_GROW);	//Distant grass should slowly raise out of the ground when coming in range
	l->setRenderTechnique(GRASSTECH_SPRITE);	//Draw grass as scattered quads

	l->setHeightRange(0.001f);
	
	//This sets a color map to be used when determining thfalsee color of each grass quad. setMapBounds()
	//is used to set the area which is affected by the color map. Here, "terrain_texture.jpg" is used
	//to color the grass to match the terrain under it.
	l->setColorMap(shadowMap.get());
	l->setDensityMap(densityMap.get());
	l->setMapBounds(bounds);	//(0,0)-(1500,1500) is the full boundaries of the terrain

// 	//-------------------------------------- LOAD GRASS --------------------------------------
// 	//Create and configure a new PagedGeometry instance for grass
// 	mGrass2 = new PagedGeometry(camera, 32);
// 	mGrass2->addDetailLevel<GrassPage>(100);
// 
// 	//Create a GrassLoader object
// 	mGrassLoader2 = new GrassLoader(mGrass2);
// 	mGrass2->setPageLoader(mGrassLoader2);	//Assign the "treeLoader" to be used to load geometry for the PagedGeometry instance
// 
// 	//Supply a height function to GrassLoader so it can calculate grass Y values
// 	//HeightFunction::initialize(sceneMgr);
// 	mGrassLoader2->setHeightFunction(&getTerrainHeight);
// 
// 	//Add some grass to the scene with GrassLoader::addLayer()
// 	GrassLayer *l2 = mGrassLoader2->addLayer("grass");
// 
// 	//Configure the grass layer properties (size, density, animation properties, fade settings, etc.)
// 	l2->setMinimumSize(1.0f, 1.0f);
// 	l2->setMaximumSize(1.5f, 1.5f);
// 	l2->setAnimationEnabled(true);		//Enable animations
// 	l2->setSwayDistribution(10.0f);		//Sway fairly unsynchronized
// 	l2->setSwayLength(0.5f);				//Sway back and forth 0.5 units in length
// 	l2->setSwaySpeed(0.5f);				//Sway 1/2 a cycle every second
// 	l2->setDensity(1.5f);				//Relatively dense grass
// 	l2->setFadeTechnique(FADETECH_ALPHA);	//Distant grass should slowly raise out of the ground when coming in range
// 	l2->setRenderTechnique(GRASSTECH_SPRITE);	//Draw grass as scattered quads
// 
// 	//This sets a color map to be used when determining the color of each grass quad. setMapBounds()
// 	//is used to set the area which is affected by the color map. Here, "terrain_texture.jpg" is used
// 	//to color the grass to match the terrain under it.
// 	//l2->setColorMap("terrain_texture.jpg");
// 	l2->setMapBounds(TBounds(mExtent.lowCorner().x(), mExtent.lowCorner().y(), mExtent.highCorner().x(), mExtent.highCorner().y()));	
}


// void FoliageArea::placeGrass(const std::string& type, const TerrainPosition& position)
// {
// 	float scaleFactor = Ogre::Math::RangeRandom(1.85, 2.15);
// 	const Ogre::Vector3 scale(
// 		scaleFactor, scaleFactor, scaleFactor);
// 	placeGrass(type, position, scale);
// }
// 
// 
// void FoliageArea::placeGrass(const std::string& type, const TerrainPosition& position, const Ogre::Vector3& scale)
// {
// 	Terrain::TerrainGenerator* terrain = EmberOgre::getSingleton().getTerrainGenerator();
// 	Ogre::Vector3 ogrePosition = Atlas2Ogre(position);
// 	ogrePosition.y = terrain->getHeight(position);
// 	placeGrass(type, ogrePosition, scale);	
// }
// 
// void FoliageArea::placeGrass(const std::string& type, const Ogre::Vector3& position, const Ogre::Vector3& scale)
// {
// 	Ogre::Quaternion orientation;
// 	orientation.FromAngleAxis(
// 		Ogre::Degree(Ogre::Math::RangeRandom(0, 359)),
// 		Ogre::Vector3::UNIT_Y);
// 	placeGrass(type, position, scale, orientation);
// }
// 
// void FoliageArea::placeGrass(const std::string& type, const Ogre::Vector3& position, const Ogre::Vector3& scale, const Ogre::Quaternion& orientation)
// {
// 	Ogre::Entity* currentEnt;
// 	currentEnt = mFoliage.getEntity(type);
// 	if (currentEnt) {
// // 		std::stringstream ss;
// // 		ss << "Adding foliage of type '" << type << "' at x:" << position.x << " y:" << position.y << " z:" << position.z;
// // 		S_LOG_VERBOSE(ss.str());
// 
// 		mStaticGeom->addEntity(currentEnt, position, orientation, scale);
// 	}
// }

// void FoliageArea::build()
// {
// 	assert(mStaticGeom);
// 	try {
// 		mStaticGeom->build();
// 		setVisible(true);
// 		
// 	} catch (const Ogre::Exception& e) {
// 		S_LOG_FAILURE("Got error when building static geometry for foliage. Expection: " << e.getFullDescription());
// 	}
// // 	mStaticGeom->dump("/home/erik/skit/staticgeom.txt");
// }


void FoliageArea::frameStarted(const Ogre::Real & timeElapsed)
{	

	mGrass->update();
// 	mGrass2->update();
	
// 	//don't wave if we're not visible, not entirely sure that this won't look strange, I don't think so
// 	if (mVisible) {
// 		waveGrass(timeElapsed);
// 	}

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

//Gets the height of the terrain at the specified x/z coordinate
//The userData parameter isn't used in this implementation of a height function, since
//there's no need for extra data other than the x/z coordinates.
float getTerrainHeight(const float x, const float z, void *userData){
//	return 1;
	return EmberOgre::getSingleton().getTerrainGenerator()->getAdapter()->getHeightAt(x, z);
}

}

}
