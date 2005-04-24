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
#include "FoliageArea.h"

//#include "GroundCover.h"
#include "components/ogre/EmberOgre.h"
#include "components/ogre/TerrainGenerator.h"


namespace EmberOgre {

#define GRASS_HEIGHT 0.5
#define GRASS_WIDTH 0.5
#define GRASS_MESH_NAME "grassblades"
#define GRASS_MATERIAL "Examples/GrassBlades"
#define OFFSET_PARAM 999
Foliage::Foliage(Ogre::Camera* camera, Ogre::SceneManager* sceneMgr)
: mCamera(camera), mSceneMgr(sceneMgr)
{
	createGrassMesh();

	Ogre::Entity* e = mSceneMgr->createEntity("1", GRASS_MESH_NAME);
	mEntities.push_back(e);

	Ogre::Entity* e0 = mSceneMgr->createEntity("environment/field/heartblood", "environment/field/small_plant/heartblood/normal.mesh");
	Ogre::Entity* e1 = mSceneMgr->createEntity("environment/field/teardrops", "environment/field/small_plant/teardrops/normal.mesh");
	Ogre::Entity* e2 = mSceneMgr->createEntity("environment/field/thingrass", "environment/field/small_plant/thingrass/normal.mesh");
	Ogre::Entity* e3 = mSceneMgr->createEntity("environment/field/bittergrass", "environment/field/small_plant/bittergrass/normal.mesh");

	mEntities.push_back(e0);
	mEntities.push_back(e1);
	mEntities.push_back(e2);
	mEntities.push_back(e3);

}


Foliage::~Foliage()
{
}

Ogre::Entity* Foliage::getEntity(int index)
{
	return mEntities[index];
}

FoliageArea* Foliage::createArea()
{
	
	FoliageArea* area = new FoliageArea();
	mFoliageAreas.push_back(area);
	std::stringstream ss;
	ss << mFoliageAreas.size();
	area->init(this, mSceneMgr, std::string("foliage_") + ss.str());
	return area;
}


void Foliage::createGrassMesh()
{
	// Each grass section is 3 planes at 60 degrees to each other
	// Normals point straight up to simulate correct lighting
	Ogre::MeshPtr msh = Ogre::MeshManager::getSingleton().createManual(GRASS_MESH_NAME, 
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	Ogre::SubMesh* sm = msh->createSubMesh();
	sm->useSharedVertices = false;
	sm->vertexData = new Ogre::VertexData();
	sm->vertexData->vertexStart = 0;
	sm->vertexData->vertexCount = 12;
	Ogre::VertexDeclaration* dcl = sm->vertexData->vertexDeclaration;
	size_t offset = 0;
	dcl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
	offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
	dcl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_NORMAL);
	offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
	dcl->addElement(0, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES);
	offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT2);

	Ogre::HardwareVertexBufferSharedPtr vbuf = Ogre::HardwareBufferManager::getSingleton()
		.createVertexBuffer(
			offset, 12, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);
	float* pReal = static_cast<float*>(vbuf->lock(Ogre::HardwareBuffer::HBL_DISCARD));
	Ogre::Vector3 baseVec(GRASS_WIDTH/2, 0, 0);
	Ogre::Vector3 vec = baseVec;
	Ogre::Quaternion rot;
	rot.FromAngleAxis(Ogre::Degree(60), Ogre::Vector3::UNIT_Y);
	int i;
	for (i = 0; i < 3; ++i)
	{
		// position
		*pReal++ = -vec.x;
		*pReal++ = GRASS_HEIGHT;
		*pReal++ = -vec.z;
		// normal
		*pReal++ = 0;
		*pReal++ = 1;
		*pReal++ = 0;
		// uv
		*pReal++ = 0;
		*pReal++ = 0;

		// position
		*pReal++ = vec.x;
		*pReal++ = GRASS_HEIGHT;
		*pReal++ = vec.z;
		// normal
		*pReal++ = 0;
		*pReal++ = 1;
		*pReal++ = 0;
		// uv
		*pReal++ = 1;
		*pReal++ = 0;

		// position
		*pReal++ = -vec.x;
		*pReal++ = 0;
		*pReal++ = -vec.z;
		// normal
		*pReal++ = 0;
		*pReal++ = 1;
		*pReal++ = 0;
		// uv
		*pReal++ = 0;
		*pReal++ = 1;

		// position
		*pReal++ = vec.x;
		*pReal++ = 0;
		*pReal++ = vec.z;
		// normal
		*pReal++ = 0;
		*pReal++ = 1;
		*pReal++ = 0;
		// uv
		*pReal++ = 1;
		*pReal++ = 1;

		vec = rot * vec;
	}
	vbuf->unlock();
	sm->vertexData->vertexBufferBinding->setBinding(0, vbuf);
	sm->indexData->indexCount = 6*3;
	sm->indexData->indexBuffer = Ogre::HardwareBufferManager::getSingleton()
		.createIndexBuffer(Ogre::HardwareIndexBuffer::IT_16BIT, 6*3,
			Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);
	Ogre::uint16* pI = static_cast<Ogre::uint16*>(
		sm->indexData->indexBuffer->lock(Ogre::HardwareBuffer::HBL_DISCARD));
	for (i = 0; i < 3; ++i)
	{
		int off = i*4;
		*pI++ = 0 + off;
		*pI++ = 3 + off;
		*pI++ = 1 + off;

		*pI++ = 0 + off;
		*pI++ = 2 + off;
		*pI++ = 3 + off;
	}

	sm->indexData->indexBuffer->unlock();

	sm->setMaterialName(GRASS_MATERIAL);
	msh->load();

}








};
