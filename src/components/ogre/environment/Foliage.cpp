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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "services/EmberServices.h"
#include "services/logging/LoggingService.h"
#include "services/config/ConfigService.h"

#include "Foliage.h"
#include "FoliageArea.h"
#include "FoliageLayer.h"
#include "FoliageBase.h"
#include "GrassFoliage.h"
#include "ShrubberyFoliage.h"
// #include "FoliageImpl.h"

//#include "GroundCover.h"
#include "components/ogre/EmberOgre.h"
#include "components/ogre/terrain/TerrainGenerator.h"


#include "../AvatarCamera.h"
#include "../EmberOgre.h"
#include "../terrain/TerrainGenerator.h"
#include "../terrain/TerrainLayerDefinition.h"
#include "../terrain/TerrainLayerDefinitionManager.h"
#include "../terrain/ISceneManagerAdapter.h"

#include "pagedgeometry/include/PagedGeometry.h"
#include "pagedgeometry/include/BatchPage.h"
#include "pagedgeometry/include/GrassLoader.h"


template<> EmberOgre::Environment::Foliage* Ember::Singleton<EmberOgre::Environment::Foliage>::ms_Singleton = 0;
using namespace EmberOgre::Terrain;

namespace EmberOgre {

namespace Environment {

#define GRASS_HEIGHT 0.5
#define GRASS_WIDTH 0.5
#define GRASS_MESH_NAME "grassblades"
#define GRASS_MATERIAL "Examples/GrassBlades"
#define OFFSET_PARAM 999



Foliage::Foliage( Ogre::SceneManager* sceneMgr)
:  mSceneMgr(sceneMgr)
, mGrass(0)
, mGrassLoader(0)
{
	S_LOG_INFO("Setting up foliage system.");
	
// 	mImpl = new FoliageImpl();GrassFoliage
	
// 	createGrassMesh();
// 
// 	Ogre::Entity* e = mSceneMgr->createEntity("1", GRASS_MESH_NAME);
// 	mEntities["grass"] = e;
// 
// 	Ogre::Entity* entity;
// 	 
// 	try {
// 		entity = mSceneMgr->createEntity("environment/field/placeholder", "placeholder.mesh");
// 		mEntities["placeholder"] = entity;
// 	} 
// 	catch (const Ogre::Exception& e) {}
// 	try {
// 		entity = mSceneMgr->createEntity("environment/field/heartblood", "environment/field/small_plant/heartblood/normal.mesh");
// 		mEntities["heartblood"] = entity;
// 	} 
// 	catch (const Ogre::Exception& e) {}
// 	try {
// 		entity = mSceneMgr->createEntity("environment/field/teardrops", "environment/field/small_plant/teardrops/normal.mesh");
// 		mEntities["teardrops"] = entity;
// 	} 
// 	catch (const Ogre::Exception& e) {}
// 	try {
// 		entity = mSceneMgr->createEntity("environment/field/thingrass", "environment/field/small_plant/thingrass/normal.mesh");
// 		mEntities["thingrass"] = entity;
// 	} 
// 	catch (const Ogre::Exception& e) {}
// 	try {
// 		entity = mSceneMgr->createEntity("environment/field/bittergrass", "3d_objects/plants/tussock/models/small_plant/simplegrass.mesh");
// 		mEntities["bittergrass"] = entity;
// 	} 
// 	catch (const Ogre::Exception& e) {}
// 	
// 	
// 	Ember::ConfigService* configSrv = Ember::EmberServices::getSingletonPtr()->getConfigService();
// 	//mSubmeshSize = (int)configSrv->getValue("foliage", "submeshsize");
// 	mGrassSpacing = (double)configSrv->getValue("foliage", "spacing_grass");
// // 	double bushSpacing = (double)configSrv->getValue("foliage", "spacing_bushes");
// // 	double cullDistance = (double)configSrv->getValue("foliage", "cullingdistance");
// 
	Ogre::Root::getSingleton().addFrameListener(this);

}


Foliage::~Foliage()
{
	S_LOG_INFO("Shutting down foliage system.");
	
	for (FoliageStore::iterator I = mFoliages.begin(); I != mFoliages.end(); ++I) {
		delete *I;
	}
	delete mGrassLoader;
	delete mGrass;
	
// 	delete mImpl;
	
// 	for (FoliageAreaStore::iterator I = mFoliageAreas.begin(); I != mFoliageAreas.end(); ++I) {
// 		delete *I;
// 	}
// 	
	Ogre::Root::getSingleton().removeFrameListener(this);
// 	for (EntityStore::iterator I = mEntities.begin(); I != mEntities.end(); ++I) {
// 		mSceneMgr->destroyMovableObject(I->second);
// 	}
}

void Foliage::initialize()
{

	for (TerrainLayerDefinitionManager::DefinitionStore::const_iterator I = TerrainLayerDefinitionManager::getSingleton().getDefinitions().begin(); I != TerrainLayerDefinitionManager::getSingleton().getDefinitions().end(); ++I) {
		const TerrainLayerDefinition* layerDef = *I;
		for (TerrainLayerDefinition::TerrainFoliageDefinitionStore::const_iterator J = layerDef->getFoliages().begin(); J != layerDef->getFoliages().end(); ++J) {
			try {
				if (J->getRenderTechnique() == "grass") {
					GrassFoliage* foliage = new GrassFoliage(*layerDef, *J);
					foliage->initialize();
					mFoliages.push_back(foliage);
				} else if (J->getRenderTechnique() == "shrubbery") {
					ShrubberyFoliage* foliage = new ShrubberyFoliage(*layerDef, *J);
					foliage->initialize();
					mFoliages.push_back(foliage);
				}
			} catch (const std::exception& ex) 
			{
				S_LOG_FAILURE("Error when creating foliage. Message: " << ex.what());
			}
		}
	}
}


// void Foliage::createGrass()
// {
// // 	return;
// 	Ogre::Camera* camera = EmberOgre::getSingleton().getMainCamera()->getCamera();
// 	mGrass = new ::PagedGeometry::PagedGeometry(camera, 32);
// 	const WFMath::AxisBox<2>& worldSize = EmberOgre::getSingleton().getTerrainGenerator()->getTerrainInfo().getWorldSizeInIndices();	mGrass->setBounds(Atlas2Ogre(worldSize));
// 	mGrass->addDetailLevel<PagedGeometry::GrassPage>(100);
// 	
// 	//Create a GrassLoader object
// 	mGrassLoader = new ::PagedGeometry::GrassLoader<FoliageLayer>(mGrass);
//  	mGrass->setPageLoader(mGrassLoader);	//Assign the "treeLoader" to be used to load 
// 	mGrassLoader->setHeightFunction(&getTerrainHeight);
// 
// 	//Add some grass to the scene with GrassLoader::addLayer()
// 	FoliageLayer *l = mGrassLoader->addLayer("/global/plants/grass/bittergrass/single");
// 	
// 	//Configure the grass layer properties (size, density, animation properties, fade settings, etc.)
// 	l->setMinimumSize(1.0f, 1.0f);
// 	l->setMaximumSize(1.5f, 1.5f);
// 	l->setAnimationEnabled(true);		//Enable animations
// 	l->setSwayDistribution(10.0f);		//Sway fairly unsynchronized
// 	l->setSwayLength(0.5f);				//Sway back and forth 0.5 units in length
// 	l->setSwaySpeed(0.5f);				//Sway 1/2 a cycle every second
// // 	l->setDensity(1.5f);				//Relatively dense grass
// 	l->setFadeTechnique(::PagedGeometry::FADETECH_GROW);	//Distant grass should slowly raise out of the ground when coming in range
// 	l->setRenderTechnique(::PagedGeometry::GRASSTECH_CROSSQUADS);	//Draw grass as scattered quads
// 
// // 	l->setHeightRange(0.001f);
// 	l->setMapBounds(Atlas2Ogre(worldSize));	//(0,0)-(1500,1500) is the full boundaries of the terrain
// 
// 
// // 	mImpl->createGrass();
// }

Ogre::Entity* Foliage::getEntity(const std::string& name)
{
// 	EntityStore::iterator I = mEntities.find(name);
// 	if (I != mEntities.end()) {
// 		return I->second;
// 	}
	return 0;
}

FoliageArea* Foliage::createArea(const WFMath::AxisBox<2>& extent)
{
	return 0;
	std::stringstream ss;
	ss << mFoliageAreas.size();
	FoliageArea* area = new FoliageArea(*this, *mSceneMgr, std::string("foliage_") + ss.str());
	mFoliageAreas.push_back(area);
	return area;
}

void Foliage::destroyArea(FoliageArea* area)
{
	for (FoliageAreaStore::iterator I = mFoliageAreas.begin(); I != mFoliageAreas.end(); ++I) {
		if (*I == area) {
			mFoliageAreas.erase(I);
			break;
		}
	}
 	delete area;
}

void Foliage::createGrassMesh()
{
// 	// Each grass section is 3 planes at 60 degrees to each other
// 	// Normals point straight up to simulate correct lighting
// 	Ogre::MeshPtr msh = Ogre::MeshManager::getSingleton().createManual(GRASS_MESH_NAME, 
// 		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
// 	Ogre::SubMesh* sm = msh->createSubMesh();
// 	sm->useSharedVertices = false;
// 	sm->vertexData = new Ogre::VertexData();
// 	sm->vertexData->vertexStart = 0;
// 	sm->vertexData->vertexCount = 12;
// 	Ogre::VertexDeclaration* dcl = sm->vertexData->vertexDeclaration;
// 	size_t offset = 0;
// 	dcl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
// 	offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
// 	dcl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_NORMAL);
// 	offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
// 	dcl->addElement(0, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES);
// 	offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT2);
// 
// 	Ogre::HardwareVertexBufferSharedPtr vbuf = Ogre::HardwareBufferManager::getSingleton()
// 		.createVertexBuffer(
// 			offset, 12, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);
// 	float* pReal = static_cast<float*>(vbuf->lock(Ogre::HardwareBuffer::HBL_DISCARD));
// 	Ogre::Vector3 baseVec(GRASS_WIDTH/2, 0, 0);
// 	Ogre::Vector3 vec = baseVec;
// 	Ogre::Quaternion rot;
// 	rot.FromAngleAxis(Ogre::Degree(60), Ogre::Vector3::UNIT_Y);
// 	int i;
// 	for (i = 0; i < 3; ++i)
// 	{
// 		// position
// 		*pReal++ = -vec.x;
// 		*pReal++ = GRASS_HEIGHT;
// 		*pReal++ = -vec.z;
// 		// normal
// 		*pReal++ = 0;
// 		*pReal++ = 1;
// 		*pReal++ = 0;
// 		// uv
// 		*pReal++ = 0;
// 		*pReal++ = 0;
// 
// 		// position
// 		*pReal++ = vec.x;
// 		*pReal++ = GRASS_HEIGHT;
// 		*pReal++ = vec.z;
// 		// normal
// 		*pReal++ = 0;
// 		*pReal++ = 1;
// 		*pReal++ = 0;
// 		// uv
// 		*pReal++ = 1;
// 		*pReal++ = 0;
// 
// 		// position
// 		*pReal++ = -vec.x;
// 		*pReal++ = 0;
// 		*pReal++ = -vec.z;
// 		// normal
// 		*pReal++ = 0;
// 		*pReal++ = 1;
// 		*pReal++ = 0;
// 		// uv
// 		*pReal++ = 0;
// 		*pReal++ = 1;
// 
// 		// position
// 		*pReal++ = vec.x;
// 		*pReal++ = 0;
// 		*pReal++ = vec.z;
// 		// normal
// 		*pReal++ = 0;
// 		*pReal++ = 1;
// 		*pReal++ = 0;
// 		// uv
// 		*pReal++ = 1;
// 		*pReal++ = 1;
// 
// 		vec = rot * vec;
// 	}
// 	vbuf->unlock();
// 	sm->vertexData->vertexBufferBinding->setBinding(0, vbuf);
// 	sm->indexData->indexCount = 6*3;
// 	sm->indexData->indexBuffer = Ogre::HardwareBufferManager::getSingleton()
// 		.createIndexBuffer(Ogre::HardwareIndexBuffer::IT_16BIT, 6*3,
// 			Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);
// 	Ogre::uint16* pI = static_cast<Ogre::uint16*>(
// 		sm->indexData->indexBuffer->lock(Ogre::HardwareBuffer::HBL_DISCARD));
// 	for (i = 0; i < 3; ++i)
// 	{
// 		int off = i*4;
// 		*pI++ = 0 + off;
// 		*pI++ = 3 + off;TerrainPageSurface
// 		*pI++ = 1 + off;
// 
// 		*pI++ = 0 + off;
// 		*pI++ = 2 + off;
// 		*pI++ = 3 + off;
// 	}
// 
// 	sm->indexData->indexBuffer->unlock();
// 
// 	sm->setMaterialName(GRASS_MATERIAL);
// 	msh->load();

}


bool Foliage::frameStarted(const Ogre::FrameEvent & evt)
{	
// 	mImpl->update();
// 	for (FoliageAreaStore::iterator I = mFoliageAreas.begin(); I != mFoliageAreas.end(); ++I) {
// 		(*I)->frameStarted(evt.timeSinceLastFrame);
// 	}
	for (FoliageStore::iterator I = mFoliages.begin(); I != mFoliages.end(); ++I) {
		(*I)->frameStarted(evt);
	}	
	
	if (mGrass) {
		try {
			mGrass->update();
		} catch (const Ogre::Exception& ex)
		{
			S_LOG_FAILURE("Error when updating grass. Will disable grass.\n"<< ex.what());
			delete mGrassLoader;
			delete mGrass;
		}
	}	
	return true;

}
}
}

