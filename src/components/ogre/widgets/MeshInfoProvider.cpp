/*
 * Copyright (C) 2012 Peter Szucs <peter.szucs.dev@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "MeshInfoProvider.h"
#include "components/ogre/lod/LodManager.h"
#include "components/ogre/lod/LodDefinition.h"
#include "components/ogre/lod/LodDefinitionManager.h"
#include "components/ogre/lod/EmberOgreMesh.h"
#include <OgrePrerequisites.h>
#include <OgreMesh.h>
#include <OgreSubMesh.h>
#include <OgreProgressiveMesh.h>

#include <sstream>


namespace Ember
{
namespace OgreView
{
namespace Gui
{

void MeshInfoProvider::calcUniqueVertexCount(VertexSet& vertices, const Ogre::VertexData& data)
{
	// Locate position element and the buffer to go with it.
	const Ogre::VertexElement* posElem = data.vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
	Ogre::HardwareVertexBufferSharedPtr vbuf = data.vertexBufferBinding->getBuffer(posElem->getSource());

	// Lock the buffer for reading.
	unsigned char* pVertex = static_cast<unsigned char*>(
	    vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
	int vSize = vbuf->getVertexSize();
	unsigned char* pEnd = pVertex + data.vertexCount * vSize;

	// Loop through all vertices and insert them to the HashMap.
	for (; pVertex <= pEnd; pVertex += vSize) {
		float* pFloat;
		posElem->baseVertexPointerToElement(pVertex, &pFloat);

		// TODO: use C++11 emplace for speedup.
		vertices.insert(Ogre::Vector3(pFloat[0], pFloat[1], pFloat[2]));
	}
	vbuf->unlock();
}

size_t MeshInfoProvider::calcUniqueVertexCount(const Ogre::MeshPtr& mesh)
{
	bool addedShared = false;

	// TODO: use C++11 unordered_set and remove VectorLessComparator struct.
	VertexSet vertices;

	for (unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i) {
		const Ogre::SubMesh& submesh = *mesh->getSubMesh(i);
		// We only need to add the shared vertices once.
		if (submesh.useSharedVertices) {
			if (!addedShared) {
				addedShared = true;
				calcUniqueVertexCount(vertices, *mesh->sharedVertexData);
			}
		} else {
			calcUniqueVertexCount(vertices, *submesh.vertexData);
		}
	}

	return vertices.size();
}

size_t MeshInfoProvider::calcUniqueVertexCount(const Ogre::VertexData& data)
{
	bool addedShared = false;

	// TODO: use C++11 unordered_set and remove VectorLessComparator struct.
	VertexSet vertices;
	calcUniqueVertexCount(vertices, data);

	return vertices.size();
}
int MeshInfoProvider::getVertexSize(const Ogre::VertexData* data)
{
	int size = 0;
	for (size_t i = 0; i < data->vertexDeclaration->getMaxSource() + 1; i++) {
		size += data->vertexDeclaration->getVertexSize(i);
	}
	return size;
}
std::string MeshInfoProvider::getInfo(int submeshIndex)
{
	const Ogre::MeshPtr& mesh = mEntityRenderer->getEntity()->getMesh();
	size_t count = calcUniqueVertexCount(mesh);
	std::stringstream str;
	const Ogre::SubMesh& submesh = *mesh->getSubMesh(submeshIndex);
	// We only need to add the shared vertices once
	Ogre::VertexData* vertexData = submesh.useSharedVertices ? mesh->sharedVertexData : submesh.vertexData;
	Ogre::HardwareIndexBufferSharedPtr ibuf = submesh.indexData->indexBuffer;
	str << "Unique vertex count: " << count << std::endl;
	str << "Vertex count: " << vertexData->vertexCount << std::endl;
	str << "Vertex size: " << getVertexSize(vertexData) << std::endl;
	str << "Index count: " << submesh.indexData->indexCount << std::endl;
	str << "Index size: " << (ibuf->getType() == Ogre::HardwareIndexBuffer::IT_16BIT ? 2 : 4) << std::endl;
	str << "Shared vertices: " << (submesh.useSharedVertices ? "[colour='FF00AA00']yes" : "[colour='FFFF0000']no") <<
	"[colour='FF000000']" << std::endl;
	str << "Edge data: " << (submesh.isBuildEdgesEnabled() ? "[colour='FFFF0000']yes" : "[colour='FF00AA00']no") <<
	"[colour='FF000000']" << std::endl;
	return str.str();
}

std::string MeshInfoProvider::getPreviewInfo()
{
	std::stringstream str;
	Ogre::Entity* entity = mEntityRenderer->getEntity();
	if (!entity) {
		return "";
	}
	const Ogre::MeshPtr& mesh = entity->getMesh();
	mLodIndex = std::min(mesh->getNumLodLevels() - 1, mLodIndex);
	const Ogre::MeshLodUsage& lod = mesh->getLodLevel(mLodIndex);
	if (mesh->isLodManual()) {
		str << lod.manualName << std::endl;
	} else {
		bool addedShared;
		size_t indexCount = 0;

		int count = mesh->getNumSubMeshes();
		for (unsigned short i = 0; i < count; ++i) {
			const Ogre::SubMesh& submesh = *mesh->getSubMesh(i);

			if (mLodIndex > 0) {
				indexCount += submesh.mLodFaceList[mLodIndex - 1]->indexCount;
			} else {
				indexCount += submesh.indexData->indexCount;
			}

		}

		str << indexCount << " indices" << std::endl;
		str << (indexCount / 3) << " triangles" << std::endl;
	}

	return str.str();
}
bool MeshInfoProvider::prequeueEntityMeshLodChanged(Ogre::EntityMeshLodChangedEvent& evt)
{
	if (evt.entity == mEntityRenderer->getEntity() && mLodIndex != evt.newLodIndex) {
		mLodIndex = evt.newLodIndex;
		EventLodChanged.emit();
	}
	return false;
}

int MeshInfoProvider::getLodIndex()
{
	return mLodIndex;
}

MeshInfoProvider::MeshInfoProvider(OgreEntityRenderer* entityRenderer) :
	mEntityRenderer(entityRenderer),
	mLodIndex(0)
{
	mEntityRenderer->getSceneManager()->addLodListener(this);
}

MeshInfoProvider::~MeshInfoProvider()
{
	mEntityRenderer->getSceneManager()->removeLodListener(this);
}

}
}
}
