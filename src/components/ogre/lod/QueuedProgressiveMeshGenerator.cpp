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

#include "QueuedProgressiveMeshGenerator.h"

#include <OgreSubMesh.h>
#include <OgreHardwareBufferManager.h>

template<>
Ember::OgreView::Lod::QueuedProgressiveMeshGenerator *
Ember::Singleton<Ember::OgreView::Lod::QueuedProgressiveMeshGenerator>::ms_Singleton = 0;

namespace Ember
{
namespace OgreView
{
namespace Lod
{

PMGenRequest::~PMGenRequest()
{
	std::vector<SubmeshInfo>::iterator it = submesh.begin();
	std::vector<SubmeshInfo>::iterator itEnd = submesh.end();
	for (; it != itEnd; it++) {
		delete [] it->indexBuffer.indexBuffer;
		delete [] it->vertexBuffer.vertexBuffer;
		std::vector<IndexBuffer>::iterator it2 = it->genIndexBuffers.begin();
		std::vector<IndexBuffer>::iterator it2End = it->genIndexBuffers.end();
		for (; it2 != it2End; it2++) {
			delete [] it2->indexBuffer;
		}
	}
}


Ember::OgreView::Lod::PMWorker::PMWorker()
{
	Ogre::WorkQueue* wq = Ogre::Root::getSingleton().getWorkQueue();
	unsigned short workQueueChannel = wq->getChannel("PMGen");
	wq->addRequestHandler(workQueueChannel, this);
}


PMWorker::~PMWorker()
{
	Ogre::Root* root = Ogre::Root::getSingletonPtr();
	if (root) {
		Ogre::WorkQueue* wq = root->getWorkQueue();
		if (wq) {
			unsigned short workQueueChannel = wq->getChannel("PMGen");
			wq->removeRequestHandler(workQueueChannel, this);
		}
	}
}

Ogre::WorkQueue::Response* PMWorker::handleRequest(const Ogre::WorkQueue::Request* req, const Ogre::WorkQueue* srcQ)
{
	// Called on worker thread by Ogre::WorkQueue.
	OGRE_LOCK_MUTEX(this->OGRE_AUTO_MUTEX_NAME);
	mRequest = Ogre::any_cast<PMGenRequest*>(req->getData());
	buildRequest(mRequest->config);
	return OGRE_NEW Ogre::WorkQueue::Response(req, true, req->getData());
}


void PMWorker::buildRequest(LodConfig& lodConfigs)
{
	mMeshBoundingSphereRadius = lodConfigs.mesh->getBoundingSphereRadius();
	cleanupMemory();
	tuneContainerSize();
	initialize(); // Load vertices and triangles.
	computeCosts(); // Calculate all collapse costs.
	assertValidMesh();

	computeLods(lodConfigs);
}

void PMWorker::tuneContainerSize()
{
	// Get Vertex count for container tuning.
	bool sharedVerticesAdded = false;
	size_t vertexCount = 0;
	size_t vertexLookupSize = 0;
	size_t sharedVertexLookupSize = 0;
	unsigned short submeshCount = mRequest->submesh.size();
	for (unsigned short i = 0; i < submeshCount; i++) {
		const PMGenRequest::SubmeshInfo& submesh = mRequest->submesh[i];
		if (!submesh.useSharedVertexBuffer) {
			size_t count = submesh.vertexBuffer.vertexCount;
			vertexLookupSize = std::max(vertexLookupSize, count);
			vertexCount += count;
		} else if (!sharedVerticesAdded) {
			sharedVerticesAdded = true;
			sharedVertexLookupSize = mRequest->sharedVertexBuffer.vertexCount;
			vertexCount += sharedVertexLookupSize;
		}
	}

	// Tune containers:
	mUniqueVertexSet.rehash(2 * vertexCount); // less then 0.5 item/bucket for low collision rate

	// There are less triangles then 2 * vertexCount. Except if there are bunch of triangles,
	// where all vertices have the same position, but that would not make much sense.
	mTriangleList.reserve(2 * vertexCount);

	mVertexList.reserve(vertexCount);
	mSharedVertexLookup.reserve(sharedVertexLookupSize);
	mVertexLookup.reserve(vertexLookupSize);
	mIndexBufferInfoList.resize(submeshCount);
}

void PMWorker::initialize()
{
	unsigned short submeshCount = mRequest->submesh.size();
	for (unsigned short i = 0; i < submeshCount; ++i) {
		PMGenRequest::SubmeshInfo& submesh = mRequest->submesh[i];
		PMGenRequest::VertexBuffer& vertexBuffer =
		    (submesh.useSharedVertexBuffer ? mRequest->sharedVertexBuffer : submesh.vertexBuffer);
		addVertexBuffer(vertexBuffer, submesh.useSharedVertexBuffer);
		addIndexBuffer(submesh.indexBuffer, submesh.useSharedVertexBuffer, i);
	}

	// These were only needed for addIndexData() and addVertexData().
	mSharedVertexLookup.clear();
	mVertexLookup.clear();
	mUniqueVertexSet.clear();
}

void PMWorker::addVertexBuffer(const PMGenRequest::VertexBuffer& vertexBuffer, bool useSharedVertexLookup)
{
	if (useSharedVertexLookup && !mSharedVertexLookup.empty()) {
		return; // We already loaded the shared vertex buffer.
	}

	VertexLookupList& lookup = useSharedVertexLookup ? mSharedVertexLookup : mVertexLookup;
	lookup.clear();

	// Loop through all vertices and insert them to the Unordered Map.
	Ogre::Vector3* pOut = vertexBuffer.vertexBuffer;
	Ogre::Vector3* pEnd = vertexBuffer.vertexBuffer + vertexBuffer.vertexCount;
	for (; pOut < pEnd; pOut++) {
		mVertexList.push_back(PMVertex());
		PMVertex* v = &mVertexList.back();
		v->position = *pOut;
		std::pair<UniqueVertexSet::iterator, bool> ret;
		ret = mUniqueVertexSet.insert(v);
		if (!ret.second) {
			// Vertex position already exists.
			mVertexList.pop_back();
			v = *ret.first; // Point to the existing vertex.
			v->seam = true;
		} else {
#ifndef NDEBUG
			v->costSetPosition = mCollapseCostSet.end();
#endif
			v->seam = false;
		}
		lookup.push_back(v);
	}
}

void PMWorker::addIndexBuffer(PMGenRequest::IndexBuffer& indexBuffer, bool useSharedVertexLookup, unsigned short submeshID)
{
	size_t isize = indexBuffer.indexSize;
	mIndexBufferInfoList[submeshID].indexSize = isize;
	mIndexBufferInfoList[submeshID].indexCount = indexBuffer.indexCount;
	VertexLookupList& lookup = useSharedVertexLookup ? mSharedVertexLookup : mVertexLookup;

	// Lock the buffer for reading.
	unsigned char* iStart = indexBuffer.indexBuffer;
	unsigned char* iEnd = iStart + indexBuffer.indexCount * isize;
	if (isize == sizeof(unsigned short)) {
		addIndexDataImpl<unsigned short>((unsigned short*) iStart, (unsigned short*) iEnd, lookup, submeshID);
	} else {
		// Unsupported index size.
		assert(isize == sizeof(unsigned int));
		addIndexDataImpl<unsigned int>((unsigned int*) iStart, (unsigned int*) iEnd, lookup, submeshID);
	}
}

void PMWorker::bakeLods(const LodLevel& lodConfigs)
{

	unsigned short submeshCount = mRequest->submesh.size();
	std::auto_ptr<IndexBufferPointer> indexBuffer(new IndexBufferPointer[submeshCount]);

	// Create buffers.
	for (unsigned short i = 0; i < submeshCount; i++) {
		std::vector<PMGenRequest::IndexBuffer>& lods = mRequest->submesh[i].genIndexBuffers;
		int indexCount = mIndexBufferInfoList[i].indexCount;
		assert(indexCount >= 0);

		lods.push_back(PMGenRequest::IndexBuffer());
		lods.back().indexCount = indexCount;
		lods.back().indexSize = mRequest->submesh[i].indexBuffer.indexSize;
		lods.back().indexBuffer = new unsigned char[lods.back().indexCount * lods.back().indexSize];
		indexBuffer.get()[i].pshort = (unsigned short*) lods.back().indexBuffer;
	}

	// Fill buffers.
	size_t triangleCount = mTriangleList.size();
	for (size_t i = 0; i < triangleCount; i++) {
		if (!mTriangleList[i].isRemoved) {
			if (mIndexBufferInfoList[mTriangleList[i].submeshID].indexSize == 2) {
				for (int m = 0; m < 3; m++) {
					*(indexBuffer.get()[mTriangleList[i].submeshID].pshort++) =
					    static_cast<unsigned short>(mTriangleList[i].vertexID[m]);
				}
			} else {
				for (int m = 0; m < 3; m++) {
					*(indexBuffer.get()[mTriangleList[i].submeshID].pint++) =
					    static_cast<unsigned short>(mTriangleList[i].vertexID[m]);
				}
			}
		}
	}
}


PMInjector::PMInjector()
{
	Ogre::WorkQueue* wq = Ogre::Root::getSingleton().getWorkQueue();
	unsigned short workQueueChannel = wq->getChannel("PMGen");
	wq->addResponseHandler(workQueueChannel, this);
	Ogre::Root::getSingleton().addFrameListener(this);
}

PMInjector::~PMInjector()
{
	Ogre::Root* root = Ogre::Root::getSingletonPtr();
	if (root) {
		Ogre::WorkQueue* wq = root->getWorkQueue();
		if (wq) {
			unsigned short workQueueChannel = wq->getChannel("PMGen");
			wq->removeResponseHandler(workQueueChannel, this);
			root->removeFrameListener(this);
		}
	}
}

void PMInjector::handleResponse(const Ogre::WorkQueue::Response* res, const Ogre::WorkQueue* srcQ)
{
	PMGenRequest* request = Ogre::any_cast<PMGenRequest*>(res->getData());
	OGRE_LOCK_MUTEX(this->OGRE_AUTO_MUTEX_NAME);
	readyLods.push(request);
}

bool PMInjector::frameStarted(const Ogre::FrameEvent& evt)
{
	OGRE_LOCK_MUTEX(this->OGRE_AUTO_MUTEX_NAME);
	while (!readyLods.empty()) {
		PMGenRequest* request = readyLods.top();
		inject(request);
		delete request;
		readyLods.pop();
	}
	return true;
}

void PMInjector::inject(PMGenRequest* request)
{
	const Ogre::MeshPtr& mesh = request->config.mesh;
	unsigned short submeshCount = request->submesh.size();
	assert(mesh->getNumSubMeshes() == submeshCount);
	mesh->removeLodLevels();
	for (unsigned short i = 0; i < submeshCount; i++) {
		Ogre::SubMesh::LODFaceList& lods = mesh->getSubMesh(i)->mLodFaceList;
		typedef std::vector<PMGenRequest::IndexBuffer> GenBuffers;
		GenBuffers& buffers = request->submesh[i].genIndexBuffers;
		GenBuffers::iterator it = buffers.begin();
		GenBuffers::iterator itEnd = buffers.end();
		for (; it != itEnd; it++) {
			PMGenRequest::IndexBuffer& buff = *it;
			int indexCount = buff.indexCount;
			assert(indexCount >= 0);
			if(indexCount != 0) {
				lods.push_back(OGRE_NEW Ogre::IndexData());
				lods.back()->indexStart = 0;
				lods.back()->indexCount = indexCount;
				lods.back()->indexBuffer = Ogre::HardwareBufferManager::getSingleton().createIndexBuffer(
					buff.indexSize == 2 ?
					Ogre::HardwareIndexBuffer::IT_16BIT : Ogre::HardwareIndexBuffer::IT_32BIT,
					indexCount, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY, false);
				int sizeInBytes = lods.back()->indexBuffer->getSizeInBytes();
				void* pOutBuff = lods.back()->indexBuffer->lock(0, sizeInBytes, Ogre::HardwareBuffer::HBL_DISCARD);
				memcpy(pOutBuff, buff.indexBuffer, sizeInBytes);
				lods.back()->indexBuffer->unlock();
			}
		}
	}
	static_cast<EmberOgreMesh*>(mesh.get())->_configureMeshLodUsage(request->config);
}


void QueuedProgressiveMeshGenerator::build(LodConfig& lodConfig)
{
#ifndef NDEBUG
	// Do not call this with empty Lod.
	assert(!lodConfig.levels.empty());

	// Too many lod levels.
	assert(lodConfig.levels.size() <= 0xffff);

	// Lod distances needs to be sorted.
	Ogre::Mesh::LodValueList values;
	for (int i = 0; i < lodConfig.levels.size(); i++) {
		values.push_back(lodConfig.levels[i].distance);
	}
	lodConfig.mesh->getLodStrategy()->assertSorted(values);
#endif // if ifndef NDEBUG

	PMGenRequest* req = new PMGenRequest();
	req->config = lodConfig;
	copyBuffers(lodConfig.mesh.get(), req);
	Ogre::WorkQueue* wq = Ogre::Root::getSingleton().getWorkQueue();
	unsigned short workQueueChannel = wq->getChannel("PMGen");
	wq->addRequest(workQueueChannel, 0, Ogre::Any(req));
}

void QueuedProgressiveMeshGenerator::copyVertexBuffer(Ogre::VertexData* data, PMGenRequest::VertexBuffer& out)
{
	// Locate position element and the buffer to go with it.
	const Ogre::VertexElement* elem = data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);

	// Only float supported.
	assert(elem->getSize() == 12);

	Ogre::HardwareVertexBufferSharedPtr vbuf = data->vertexBufferBinding->getBuffer(elem->getSource());

	out.vertexCount = data->vertexCount;
	out.vertexBuffer = new Ogre::Vector3[out.vertexCount];

	if (out.vertexCount > 0) {
		// Lock the buffer for reading.
		unsigned char* vStart = static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
		unsigned char* vertex = vStart;
		int vSize = vbuf->getVertexSize();

		// Loop through all vertices and insert them to the Unordered Map.
		Ogre::Vector3* pOut = out.vertexBuffer;
		Ogre::Vector3* pEnd = out.vertexBuffer + out.vertexCount;
		for (; pOut < pEnd; pOut++) {
			float* pFloat;
			elem->baseVertexPointerToElement(vertex, &pFloat);
			pOut->x = *pFloat;
			pOut->y = *(++pFloat);
			pOut->z = *(++pFloat);
			vertex += vSize;
		}
		vbuf->unlock();
	}
}

void QueuedProgressiveMeshGenerator::copyIndexBuffer(Ogre::IndexData* data, PMGenRequest::IndexBuffer& out)
{
	const Ogre::HardwareIndexBufferSharedPtr& indexBuffer = data->indexBuffer;
	out.indexSize = indexBuffer->getIndexSize();
	out.indexCount = data->indexCount;
	if (out.indexCount > 0) {
		unsigned char* pBuffer = (unsigned char*) indexBuffer->lock(Ogre::HardwareBuffer::HBL_READ_ONLY);
		size_t offset = data->indexStart * out.indexSize;
		out.indexBuffer = new unsigned char[out.indexCount * out.indexSize];
		memcpy(out.indexBuffer, pBuffer + offset, out.indexCount * out.indexSize);
		indexBuffer->unlock();
	}
}

void QueuedProgressiveMeshGenerator::copyBuffers(Ogre::Mesh* mesh, PMGenRequest* req)
{
	// Get Vertex count for container tuning.
	bool sharedVerticesAdded = false;
	unsigned short submeshCount = mesh->getNumSubMeshes();
	req->submesh.resize(submeshCount);
	for (unsigned short i = 0; i < submeshCount; i++) {
		const Ogre::SubMesh* submesh = mesh->getSubMesh(i);
		PMGenRequest::SubmeshInfo& outsubmesh = req->submesh[i];
		copyIndexBuffer(submesh->indexData, outsubmesh.indexBuffer);
		outsubmesh.useSharedVertexBuffer = submesh->useSharedVertices;
		if (!outsubmesh.useSharedVertexBuffer) {
			copyVertexBuffer(submesh->vertexData, outsubmesh.vertexBuffer);

		} else if (!sharedVerticesAdded) {
			sharedVerticesAdded = true;
			copyVertexBuffer(mesh->sharedVertexData, req->sharedVertexBuffer);
		}
	}
}

QueuedProgressiveMeshGenerator::~QueuedProgressiveMeshGenerator()
{
}

}
}
}
