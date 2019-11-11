/*
 Copyright (C) 2018 Erik Ogenvik

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

#include "BulletWorld.h"

#include <OgreMesh.h>
#include <OgreSubMesh.h>

#include <BulletCollision/CollisionShapes/btTriangleIndexVertexArray.h>
#include <BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h>
#include <BulletCollision/BroadphaseCollision/btAxisSweep3.h>

namespace Ember {
namespace OgreView {

BulletWorld::BulletWorld() {

	auto config = std::make_shared<btDefaultCollisionConfiguration>();
	auto dispatcher = std::make_shared<btCollisionDispatcher>(config.get());
	auto broadPhase = std::make_shared<btAxisSweep3>(btVector3(-1024, -128, -1024), btVector3(1024, 128, 1024));
	mCollisionWorld = std::shared_ptr<btCollisionWorld>(new btCollisionWorld(dispatcher.get(), broadPhase.get(), config.get()),
														[config, dispatcher, broadPhase](btCollisionWorld* p) { delete p; });

}

std::shared_ptr<btScaledBvhTriangleMeshShape> BulletWorld::createMeshShape(const Ogre::MeshPtr& meshPtr) {

	std::shared_ptr<btBvhTriangleMeshShape> shape;

	auto I = mTriangleMeshShapes.find(meshPtr->getHandle());
	if (I != mTriangleMeshShapes.end()) {
		shape = I->second;
	} else {
		shape = buildTriangleMeshShape(meshPtr);
	}

	return std::shared_ptr<btScaledBvhTriangleMeshShape>(new btScaledBvhTriangleMeshShape(shape.get(), btVector3(1, 1, 1)), [shape](btScaledBvhTriangleMeshShape* p) {
		delete p;
	});


}

std::shared_ptr<btBvhTriangleMeshShape> BulletWorld::buildTriangleMeshShape(const Ogre::MeshPtr& meshPtr) {
	static int vertStride = sizeof(float) * 3;
	static int indexStride = sizeof(unsigned int) * 3;

	// mesh data to retrieve
	auto vertices = std::make_shared<std::vector<float>>();
	auto indices = std::make_shared<std::vector<unsigned int>>();

	assert(meshPtr->isLoaded());

	// get the mesh information
	getMeshInformation(meshPtr, *vertices, *indices);

	std::shared_ptr<btTriangleIndexVertexArray> triangleVertexArray(new btTriangleIndexVertexArray(),
																	[vertices, indices](btTriangleIndexVertexArray* p) {
																		delete p;
																	});

	btIndexedMesh mesh;
	mesh.m_numTriangles = static_cast<int>(indices->size() / 3);
	mesh.m_triangleIndexBase = reinterpret_cast<const unsigned char*>(indices->data());
	mesh.m_triangleIndexStride = indexStride;
	mesh.m_numVertices = static_cast<int>(vertices->size() / 3);
	mesh.m_vertexBase = reinterpret_cast<const unsigned char*>(vertices->data());
	mesh.m_vertexStride = vertStride;

	triangleVertexArray->addIndexedMesh(mesh, PHY_INTEGER);

	btVector3 aabbMin, aabbMax;
	triangleVertexArray->calculateAabbBruteForce(aabbMin, aabbMax);
	triangleVertexArray->setPremadeAabb(aabbMin, aabbMax);

	std::shared_ptr<btBvhTriangleMeshShape> meshShape(new btBvhTriangleMeshShape(triangleVertexArray.get(), true, true),
													  [triangleVertexArray](btBvhTriangleMeshShape* p) {
														  delete p;
													  });

	mTriangleMeshShapes.insert(std::make_pair(meshPtr->getHandle(), meshShape));
	return meshShape;
}

btCollisionWorld& BulletWorld::getCollisionWorld() const {
	return *mCollisionWorld;
}

// Get the mesh information for the given mesh.
// Code found on this forum link: http://www.ogre3d.org/wiki/index.php/RetrieveVertexData
void BulletWorld::getMeshInformation(const Ogre::MeshPtr& mesh,
									 std::vector<float>& vertices,
									 std::vector<unsigned int>& indices) {
	bool added_shared = false;
	size_t current_offset = 0;
	size_t shared_offset = 0;
	size_t next_offset = 0;
	size_t index_offset = 0;

	size_t vertex_count = 0;
	size_t index_count = 0;

	// Calculate how many vertices and indices we're going to need
	for (size_t i = 0; i < mesh->getNumSubMeshes(); ++i) {
		Ogre::SubMesh* submesh = mesh->getSubMesh(i);

		// We only need to add the shared vertices once
		if (submesh->useSharedVertices) {
			if (!added_shared) {
				vertex_count += mesh->sharedVertexData->vertexCount;
				added_shared = true;
			}
		} else {
			vertex_count += submesh->vertexData->vertexCount;
		}

		// Add the indices
		index_count += submesh->indexData->indexCount;
	}


	// Allocate space for the vertices and indices
	vertices.resize(vertex_count * 3);
	indices.resize(index_count);


	added_shared = false;

	// Run through the submeshes again, adding the data into the arrays
	for (size_t i = 0; i < mesh->getNumSubMeshes(); ++i) {
		Ogre::SubMesh* submesh = mesh->getSubMesh(i);

		Ogre::VertexData* vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;

		if ((!submesh->useSharedVertices) || (!added_shared)) {
			if (submesh->useSharedVertices) {
				added_shared = true;
				shared_offset = current_offset;
			}

			const Ogre::VertexElement* posElem =
					vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);

			Ogre::HardwareVertexBufferSharedPtr vbuf =
					vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());

			auto* vertex =
					static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

			// There is _no_ baseVertexPointerToElement() which takes an Ogre::Real or a double
			//  as second argument. So make it float, to avoid trouble when Ogre::Real will
			//  be comiled/typedefed as double:
			//      Ogre::Real* pReal;
			float* pReal;

			for (size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize()) {
				posElem->baseVertexPointerToElement(vertex, &pReal);

				vertices[(current_offset * 3) + (j * 3)] = pReal[0];
				vertices[(current_offset * 3) + (j * 3) + 1] = pReal[1];
				vertices[(current_offset * 3) + (j * 3) + 2] = pReal[2];
			}

			vbuf->unlock();
			next_offset += vertex_data->vertexCount;
		}


		Ogre::IndexData* index_data = submesh->indexData;
		size_t numTris = index_data->indexCount / 3;
		Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;

		bool use32bitindexes = (ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);

		auto pLong = static_cast<Ogre::uint32*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
		auto pShort = reinterpret_cast<Ogre::uint16*>(pLong);


		size_t offset = (submesh->useSharedVertices) ? shared_offset : current_offset;

		if (use32bitindexes) {
			for (size_t k = 0; k < numTris * 3; ++k) {
				indices[index_offset++] = static_cast<unsigned int>(pLong[k] + offset);
			}
		} else {
			for (size_t k = 0; k < numTris * 3; ++k) {
				indices[index_offset++] = static_cast<unsigned int>(pShort[k] + offset);
			}
		}

		ibuf->unlock();
		current_offset = next_offset;
	}
}

}
}
