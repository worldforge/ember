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

// The collapse cost calculation in this file is based heavily on Ogre's implementation:
/*
 * -----------------------------------------------------------------------------
 * This source file is part of OGRE
 * (Object-oriented Graphics Rendering Engine)
 * For the latest info, see http://www.ogre3d.org/
 *
 * Copyright (c) 2000-2011 Torus Knot Software Ltd
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 * -----------------------------------------------------------------------------
 */

// The algorithm in this file is based heavily on:
/*
 * Progressive Mesh type Polygon Reduction Algorithm
 * by Stan Melax (c) 1998
 */

#include "ProgressiveMeshGenerator.h"


#include "framework/LoggingInstance.h"

#include <OgreHardwareBufferManager.h>
#include <OgreSubMesh.h>
#include <OgreMesh.h>

#include <limits>
#include <sstream>

namespace Ember
{
namespace OgreView
{
namespace Lod
{

#define NEVER_COLLAPSE_COST std::numeric_limits<float>::infinity()
#define NEEDS_UPDATE_COLLAPSE_COST (-std::numeric_limits<float>::infinity())
#define UNINITIALIZED_COLLAPSE_COST (std::numeric_limits<float>::min())

ProgressiveMeshGenerator::ProgressiveMeshGenerator(Ogre::Mesh& mesh) :
	mMesh(mesh),
	mIndexBufferInfoList(new IndexBufferInfo[mesh.getNumSubMeshes()])
{
	assert(UNINITIALIZED_COLLAPSE_COST != NEEDS_UPDATE_COLLAPSE_COST);

	tuneContainerSize();
	initialize(); // Load vertices and triangles
	computeCosts(); // Calculate all collapse costs
	assertValidMesh();
}

ProgressiveMeshGenerator::~ProgressiveMeshGenerator()
{
	delete [] mIndexBufferInfoList;
}

void ProgressiveMeshGenerator::tuneContainerSize()
{
	// Get Vertex count for container tuning.
	bool sharedVerticesAdded = false;
	size_t vertexCount = 0;

	for (unsigned short i = 0; i < mMesh.getNumSubMeshes(); ++i) {
		const Ogre::SubMesh* submesh = mMesh.getSubMesh(i);
		if (submesh->useSharedVertices /* && !sharedVerticesAdded*/) {
			// sharedVerticesAdded = true;
			vertexCount += mMesh.sharedVertexData->vertexCount;
		} else { /*if(!sharedVerticesAdded)*/
			vertexCount += submesh->vertexData->vertexCount;
		}
	}

	// Tune containers:
	mUniqueVertexSet.rehash(2 * vertexCount); // less then 0.5 item/bucket for low collision rate

	// At the beginning there are less triangles then 2 * vertexCount,
	// but after collapsing, new triangles are created, so we approximate to 4 * vertexCount.
	mTriangleList.reserve(4 * vertexCount);
}

void ProgressiveMeshGenerator::initialize()
{
	bool sharedVerticesAdded = false;
	bool verticesAdded;
	for (unsigned short i = 0; i < mMesh.getNumSubMeshes(); ++i) {
		const Ogre::SubMesh* submesh = mMesh.getSubMesh(i);

		// Add shared vertices once.
		if (submesh->useSharedVertices && !sharedVerticesAdded) {
			sharedVerticesAdded = true;
			verticesAdded = true;
		} else {
			verticesAdded = false;
		}
		Ogre::VertexData* vertexData = (submesh->useSharedVertices ? mMesh.sharedVertexData : submesh->vertexData);
		addIndexData(vertexData, submesh->indexData, submesh->useSharedVertices, i);
	}

	// This was only needed for triangle creation.
	// mVertexLookup.clear();
}

void ProgressiveMeshGenerator::PMTriangle::computeNormal()
{
	// Cross-product 2 edges
	Ogre::Vector3 e1 = vertex[1]->position - vertex[0]->position;
	Ogre::Vector3 e2 = vertex[2]->position - vertex[1]->position;

	normal = e1.crossProduct(e2);
	normal.normalise();
}

template<typename indexType>
void ProgressiveMeshGenerator::addIndexDataImpl(const Ogre::HardwareIndexBufferSharedPtr& ibuf,
                                                const std::vector<PMVertex*>& vertices,
                                                Ogre::VertexData* vertexData,
                                                Ogre::IndexData* indexData,
                                                unsigned short submeshID)
{

	// Lock the buffer for reading.
	indexType* iStart = static_cast<indexType*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
	indexType* pIndex = iStart;
	indexType* iEnd = iStart + ibuf->getNumIndexes();

	// Loop through all triangles and connect them to the vertices.
	for (; pIndex < iEnd; pIndex += 3) {
		mTriangleList.push_back(PMTriangle());
		PMTriangle& tri = mTriangleList.back();
		tri.minLod = 0;
		tri.maxLod = 0xFFFF;
		tri.submeshID = submeshID;
		for (int i = 0; i < 3; i++) {
			tri.vertexID[i] = pIndex[i];
			tri.vertex[i] = vertices[pIndex[i]];
			tri.vertex[i]->costSetPosition = mCollapseCostSet.end();
		}
		tri.computeNormal();
		addTriangleToEdges(mTriangleList.size() - 1);
	}

	ibuf->unlock();
}

void ProgressiveMeshGenerator::addIndexData(Ogre::VertexData* vertexData,
                                            Ogre::IndexData* indexData,
                                            bool verticesAdded,
                                            unsigned short submeshID)
{
	// TODO: USe verticesAdded parameter to prevent duplicate work.
	// Locate position element and the buffer to go with it.
	const Ogre::VertexElement* elem = vertexData->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);

	// Only float supported.
	assert(elem->getSize() == 12);

	Ogre::HardwareVertexBufferSharedPtr vbuf = vertexData->vertexBufferBinding->getBuffer(elem->getSource());

	// Lock the buffer for reading.
	unsigned char* vStart = static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
	unsigned char* vertex = vStart;
	int vSize = vbuf->getVertexSize();
	unsigned char* vEnd = vertex + vertexData->vertexCount * vSize;

	mVertexLookup.push_back(SubmeshVertexLookupList());
	SubmeshVertexLookupList& vertices = mVertexLookup.back();

	vertices.reserve(vertexData->vertexCount);
	// Loop through all vertices and insert them to the Unordered Map.
	for (; vertex < vEnd; vertex += vSize) {
		float* pFloat;
		elem->baseVertexPointerToElement(vertex, &pFloat);
		PMVertex v(pFloat[0], pFloat[1], pFloat[2]);
		std::pair<UniqueVertexSet::iterator, bool> ret;
		ret = mUniqueVertexSet.insert(v);
		PMVertex* pv = getPointer(ret.first);
		pv->seam = !ret.second;
		vertices.push_back(pv);
	}

	vbuf->unlock();

	const Ogre::HardwareIndexBufferSharedPtr& ibuf = indexData->indexBuffer;
	size_t isize = ibuf->getIndexSize();
	mIndexBufferInfoList[submeshID].indexSize = isize;
	mIndexBufferInfoList[submeshID].indexCount = indexData->indexCount;
	if (isize == sizeof(unsigned short)) {
		addIndexDataImpl<unsigned short>(ibuf, vertices, vertexData, indexData, submeshID);
	} else {
		// Unsupported index size.
		assert(isize == sizeof(unsigned int));
		addIndexDataImpl<unsigned int>(ibuf, vertices, vertexData, indexData, submeshID);
	}
}


bool ProgressiveMeshGenerator::PMTriangle::hasVertex(const PMVertex* v) const
{
	return (v == vertex[0] || v == vertex[1] || v == vertex[2]);
}

void ProgressiveMeshGenerator::PMTriangle::replaceVertexID(unsigned int oldID, unsigned int newID, PMVertex* dst)
{
	for (int i = 0; i < 3; i++) {
		if (vertexID[i] == oldID) {
			vertexID[i] = newID;
			vertex[i] = dst;
			return;
		}
	}
	assert(0);
}

unsigned int ProgressiveMeshGenerator::PMTriangle::getVertexID(const PMVertex* v) const
{
	for (int i = 0; i < 3; i++) {
		if (vertex[i] == v) {
			return vertexID[i];
		}
	}
	assert(0);
	return 0;
}

void ProgressiveMeshGenerator::removeTriangleFromEdges(size_t triangleID, PMVertex* skip)
{
	// skip is needed if we are iterating on the vertex's edges or triangles.
	PMTriangle* triangle = &mTriangleList[triangleID];
	for (int i = 0; i < 3; i++) {
		if (triangle->vertex[i] != skip) {
			size_t success = triangle->vertex[i]->triangles.erase(triangleID);
			assert(success);
		}
	}
	for (int i = 0; i < 3; i++) {
		for (int n = 0; n < 3; n++) {
			if (i != n && mTriangleList[triangleID].vertex[i] != skip) {
				removeEdge(mTriangleList[triangleID].vertex[i], PMEdge(mTriangleList[triangleID].vertex[n]));
			}
		}
	}
}
void ProgressiveMeshGenerator::printTriangle(PMTriangle* triangle, std::stringstream& str)
{
	for (int i = 0; i < 3; i++) {
		str << (i + 1) << ". vertex position: ("
		    << triangle->vertex[i]->position.x << ", "
		    << triangle->vertex[i]->position.y << ", "
		    << triangle->vertex[i]->position.z << ") "
		    << "vertex ID: " << triangle->vertexID[i] << std::endl;
	}
}
void ProgressiveMeshGenerator::addTriangleToEdges(size_t triangleID)
{
	PMTriangle* triangle = &mTriangleList[triangleID];
#if 0
	// Don't use this, because collapsing will fail.
	// Conflicts with invalidateEdgeCosts().
	// You need to call invalidateEdgeCosts after edges
	// are added, but this will remove the edges.
	{
		// duplicate triangle detection (where all vertices has the same position)
		VertexTriangles::iterator itEnd = triangle->vertex[0]->triangles.end();
		VertexTriangles::iterator it = triangle->vertex[0]->triangles.begin();
		for (; it != itEnd; it++) {
			PMTriangle* t = &mTriangleList[*it];
			for (int i = 0; i < 3; i++) {
				if (t->vertex[i] != triangle->vertex[0] ||
				    t->vertex[i] != triangle->vertex[1] ||
				    t->vertex[i] != triangle->vertex[2]) {
					continue;
				}
			}
			std::stringstream str;
			str << "In " << mMesh.getName() << " duplicate triangle found." << std::endl;
			str << "Triangle " << triangleID << " positions:" << std::endl;
			printTriangle(triangle, str);
			str << "Triangle " << *it << " positions:" << std::endl;
			printTriangle(t, str);
			str << "Triangle " << *it << " will be excluded from Lod level calculations.";
			S_LOG_WARNING(str.str());
			t->maxLod = 0;
			unsigned short maxLod = mLodIndexCountMatrix.size2();
			for (unsigned short i = 0; i < maxLod; i++) {
				mLodIndexCountMatrix(t->submeshID, i) -= 3;
			}
			return;

		}
	}
#endif // if 0
	for (int i = 0; i < 3; i++) {
		std::pair<VertexTriangles::iterator, bool> ret;
		ret = triangle->vertex[i]->triangles.insert(triangleID);
		if (!ret.second) {
			std::stringstream str;
			str << "In " << mMesh.getName() << " malformed triangle found with ID: " << triangleID << ". " << std::endl;
			printTriangle(triangle, str);
			str << "It will be excluded from Lod level calculations.";
			S_LOG_WARNING(str.str());
			for (int i = 0; i < 3; i++) {
				triangle->vertex[i]->triangles.erase(triangleID);
			}
			triangle->maxLod = 0;
			unsigned short maxLod = mLodIndexCountMatrix.size2();
			for (unsigned short i = 0; i < maxLod; i++) {
				mLodIndexCountMatrix(triangle->submeshID, i) -= 3;
			}
			return;
		}
	}
	for (int i = 0; i < 3; i++) {
		for (int n = 0; n < 3; n++) {
			if (i != n) {
				addEdge(triangle->vertex[i], PMEdge(triangle->vertex[n]));
			}
		}
	}
}

bool ProgressiveMeshGenerator::isBorderVertex(const PMVertex* vertex) const
{
	VertexEdges::const_iterator it = vertex->edges.begin();
	VertexEdges::const_iterator itEnd = vertex->edges.end();
	for (; it != itEnd; it++) {
		if (it->refCount == 1) {
			return true;
		}
	}
	return false;
}

ProgressiveMeshGenerator::PMTriangle* ProgressiveMeshGenerator::findSideTriangle(const PMVertex* v1, const PMVertex* v2)
{
	VertexTriangles::const_iterator it = v1->triangles.begin();
	VertexTriangles::const_iterator itEnd = v1->triangles.end();
	for (; it != itEnd; it++) {
		PMTriangle* triangle = &mTriangleList[*it];
		if (triangle->hasVertex(v2)) {
			return triangle;
		}
	}
	assert(0);
	return NULL;
}

ProgressiveMeshGenerator::PMVertex* ProgressiveMeshGenerator::getPointer(UniqueVertexSet::iterator it)
{
	// This is needed to overcome a C++ limitation.
	// Since I'm using a Set container, the template object is a key and you can't modify the key.
	// However in the comparator, I have only used the position, so I can modify the rest of the object.
	// If I would use Map container, then "first" and "second" separation would look ugly and makes code unreadable.
	return const_cast<PMVertex*>(&*it);
}

ProgressiveMeshGenerator::PMEdge* ProgressiveMeshGenerator::getPointer(VertexEdges::iterator it)
{
	return const_cast<PMEdge*>(&*it);
}

void ProgressiveMeshGenerator::computeCosts()
{
	mCollapseCostSet.clear();
	UniqueVertexSet::iterator it = mUniqueVertexSet.begin();
	UniqueVertexSet::iterator itEnd = mUniqueVertexSet.end();
	while (it != itEnd) {
		PMVertex* vertex = getPointer(it);
		if (!vertex->edges.empty()) {
			computeVertexCollapseCost(vertex);
			it++;
		} else {
			std::stringstream str;
			str << "In " << mMesh.getName() << " never used vertex found with ID: " << mCollapseCostSet.size() << "." <<
			std::endl
			    << "Vertex position: ("
			    << vertex->position.x << ", "
			    << vertex->position.y << ", "
			    << vertex->position.z << ")" << std::endl
			    << "It will be excluded from Lod level calculations.";
			S_LOG_WARNING(str.str());
			mUniqueVertexSet.erase(it++);
		}
	}
}

void ProgressiveMeshGenerator::computeVertexCollapseCost(PMVertex* vertex)
{
	vertex->collapseCost = NEEDS_UPDATE_COLLAPSE_COST;
	assert(!vertex->edges.empty());
	VertexEdges::iterator it = vertex->edges.begin();
	for (; it != vertex->edges.end(); it++) {
		it->collapseCost = computeEdgeCollapseCost(vertex, getPointer(it));
		if (vertex->collapseCost < it->collapseCost) {
			vertex->collapseCost = it->collapseCost;
			vertex->collapseTo = it->dst;
		}
	}
	assert(vertex->collapseCost != NEEDS_UPDATE_COLLAPSE_COST);
	vertex->costSetPosition = mCollapseCostSet.insert(vertex);

}
// TODO: convert float to Ogre::Real
float ProgressiveMeshGenerator::computeEdgeCollapseCost(PMVertex* src, PMEdge* dstEdge)
{
	// This is based on Ogre's collapse cost calculation algorithm.

	PMVertex* dst = dstEdge->dst;

	// Check for singular triangle destruction
	// If src and dest both only have 1 triangle (and it must be a shared one)
	// then this would destroy the shape, so don't do this
	if (src->triangles.size() == 1 && dst->triangles.size() == 1) {
		return NEVER_COLLAPSE_COST;
	}

	// Degenerate case check
	// Are we going to invert a face normal of one of the neighbouring faces?
	// Can occur when we have a very small remaining edge and collapse crosses it
	// Look for a face normal changing by > 90 degrees
	{
		VertexTriangles::iterator it = src->triangles.begin();
		VertexTriangles::iterator itEnd = src->triangles.end();
		for (; it != itEnd; it++) {
			PMTriangle* triangle = &mTriangleList[*it];
			// Ignore the deleted faces (those including src & dest)
			if (!triangle->hasVertex(dst)) {
				// Test the new face normal
				PMVertex* pv0, * pv1, * pv2;

				// Replace src with dest wherever it is
				pv0 = (triangle->vertex[0] == src) ? dst : triangle->vertex[0];
				pv1 = (triangle->vertex[1] == src) ? dst : triangle->vertex[1];
				pv2 = (triangle->vertex[2] == src) ? dst : triangle->vertex[2];

				// Cross-product 2 edges
				Ogre::Vector3 e1 = pv1->position - pv0->position;
				Ogre::Vector3 e2 = pv2->position - pv1->position;

				Ogre::Vector3 newNormal = e1.crossProduct(e2);
				newNormal.normalise();

				// Dot old and new face normal
				// If < 0 then more than 90 degree difference
				if (newNormal.dotProduct(triangle->normal) < 0.0f) {
					// Don't do it!
					return NEVER_COLLAPSE_COST;
				}
			}
		}
	}

	float cost;

	// Special cases
	// If we're looking at a border vertex
	if (isBorderVertex(src)) {
		if (dstEdge->refCount > 1) {
			// src is on a border, but the src-dest edge has more than one tri on it
			// So it must be collapsing inwards
			// Mark as very high-value cost
			// curvature = 1.0f;
			cost = 1.0f;
		} else {
			// Collapsing ALONG a border
			// We can't use curvature to measure the effect on the model
			// Instead, see what effect it has on 'pulling' the other border edges
			// The more colinear, the less effect it will have
			// So measure the 'kinkiness' (for want of a better term)

			// Find the only triangle using this edge.
			PMTriangle* triangle = findSideTriangle(src, dst);

			cost = 0.0f;
			Ogre::Vector3 collapseEdge = src->position - dst->position;
			collapseEdge.normalise();
			VertexEdges::iterator it = src->edges.begin();
			VertexEdges::iterator itEnd = src->edges.end();
			for (; it != itEnd; it++) {
				PMVertex* neighbor = it->dst;
				if (neighbor != dst && it->refCount == 1) {
					Ogre::Vector3 otherBorderEdge = src->position - neighbor->position;
					otherBorderEdge.normalise();
					// This time, the nearer the dot is to -1, the better, because that means
					// the edges are opposite each other, therefore less kinkiness
					// Scale into [0..1]
					float kinkiness = (otherBorderEdge.dotProduct(collapseEdge) + 1.002f) * 0.5f;
					cost = std::max(cost, kinkiness);

				}
			}
		}
	} else { // not a border

		// Standard inner vertex
		// Calculate curvature
		// use the triangle facing most away from the sides
		// to determine our curvature term
		// Iterate over src's faces again
		cost = 0.001f;
		VertexTriangles::iterator it = src->triangles.begin();
		VertexTriangles::iterator itEnd = src->triangles.end();
		for (; it != itEnd; it++) {
			Ogre::Real mincurv = 1.0f; // curve for face i and closer side to it
			PMTriangle* triangle = &mTriangleList[*it];
			VertexTriangles::iterator it2 = src->triangles.begin();
			for (; it2 != itEnd; it2++) {
				PMTriangle* triangle2 = &mTriangleList[*it2];
				if (triangle2->hasVertex(dst)) {

					// Dot product of face normal gives a good delta angle
					Ogre::Real dotprod = triangle->normal.dotProduct(triangle2->normal);
					// NB we do (1-..) to invert curvature where 1 is high curvature [0..1]
					// Whilst dot product is high when angle difference is low
					mincurv = std::min(mincurv, (1.002f - dotprod) * 0.5f);
				}
			}
			cost = std::max(cost, mincurv);
		}
	}

	// check for texture seam ripping and multiple submeshes
	if (src->seam) {
		if (!dst->seam) {
			cost += mMesh.getBoundingSphereRadius();
		} else {
			cost += mMesh.getBoundingSphereRadius() * 0.5;
		}
	}

	assert(cost >= 0);
	// TODO: use squared distance.
	return cost * src->position.distance(dst->position);
}

void ProgressiveMeshGenerator::assertOutdatedCollapseCost(PMVertex* vertex)
{
	// Validates that collapsing has updated all edges needed by computeEdgeCollapseCost.
	// This will assert if the dependencies inside computeEdgeCollapseCost changes.
#ifndef NDEBUG
	VertexEdges::iterator it = vertex->edges.begin();
	VertexEdges::iterator itEnd = vertex->edges.end();
	for (; it != itEnd; it++) {
		assert(it->collapseCost == computeEdgeCollapseCost(vertex, getPointer(it)));
		PMVertex* neighbor = it->dst;
		VertexEdges::iterator it2 = neighbor->edges.begin();
		VertexEdges::iterator it2End = neighbor->edges.end();
		for (; it2 != it2End; it2++) {
			assert(it2->collapseCost == computeEdgeCollapseCost(neighbor, getPointer(it2)));
		}
	}
#endif
}

void ProgressiveMeshGenerator::updateVertexCollapseCost(PMVertex* vertex)
{
	float collapseCost = UNINITIALIZED_COLLAPSE_COST;
	PMVertex* collapseTo = NULL;
	VertexEdges::iterator it = vertex->edges.begin();
	VertexEdges::iterator itEnd = vertex->edges.end();
	for (; it != itEnd; it++) {
		it->collapseCost = computeEdgeCollapseCost(vertex, getPointer(it));
		if (collapseCost < it->collapseCost) {
			collapseCost = it->collapseCost;
			collapseTo = it->dst;
		}
	}
	if (collapseCost != vertex->collapseCost || vertex->collapseTo != collapseTo) {
		if (vertex->costSetPosition != mCollapseCostSet.end()) {
			mCollapseCostSet.erase(vertex->costSetPosition);
		}
		if (collapseCost != UNINITIALIZED_COLLAPSE_COST) {
			assert(collapseCost != NEEDS_UPDATE_COLLAPSE_COST);
			assert(collapseTo != NULL);
			vertex->collapseCost = collapseCost;
			vertex->collapseTo = collapseTo;
			vertex->costSetPosition = mCollapseCostSet.insert(vertex);
		} else {
			assert(collapseTo == NULL);
			assert(vertex->edges.empty());
			vertex->costSetPosition = mCollapseCostSet.end();
		}
	}
}

void ProgressiveMeshGenerator::build(const LodConfigList& lodConfigs)
{
#if !defined(NDEBUG) && defined(_DEBUG)

	// Do not call this with empty Lod.
	assert(!lodConfigs.empty());

	// Too many lod levels.
	assert(lodConfigs.size() <= 0xffff);

	// Lod distances needs to be sorted.
	for (int i = 1; i < lodConfigs.size(); i++) {
		assert(lodConfigs[i - 1].distance < lodConfigs[i].distance);
	}
#endif // if !defined(NDEBUG) && defined(_DEBUG)

	mLodIndexCountMatrix.resize(mMesh.getNumSubMeshes(), lodConfigs.size(), false);

	// Initialize indexCount.
	for (unsigned short i = 0; i < mLodIndexCountMatrix.size1(); i++) {
		for (int n = 0; n < mLodIndexCountMatrix.size2(); n++) {
			mLodIndexCountMatrix(i, n) = mIndexBufferInfoList[i].indexCount;
		}
	}

	computeLods(lodConfigs);
	bakeLods(lodConfigs);

}
void ProgressiveMeshGenerator::computeLods(const LodConfigList& lodConfigs)
{
	int numVerts = mUniqueVertexSet.size();
	int lodCount = lodConfigs.size();
	for (mCurLod = 0; mCurLod < lodCount; mCurLod++) {
		size_t neededVerts = calcLodVertexCount(lodConfigs[mCurLod]);

		for (; neededVerts < numVerts; numVerts--) {
			// TODO: detect non changed Lod.
			CollapseCostSet::iterator nextIndex = mCollapseCostSet.begin();
			if (nextIndex != mCollapseCostSet.end() && (*nextIndex)->collapseCost != NEVER_COLLAPSE_COST) {
				collapse(*nextIndex);
			} else {
				return;
			}

		}
		assertValidMesh();
	}
}

size_t ProgressiveMeshGenerator::findDstID(unsigned int srcID, unsigned short submeshID)
{
	// Tries to find a compatible edge.

	// Exact match search.
	for (int i = 0; i < tmpCollapsedEdges.size(); i++) {
		if (tmpCollapsedEdges[i].srcID == srcID && tmpCollapsedEdges[i].submeshID == submeshID) {
			return i;
		}
	}

	// Usable match search.
	for (int i = 0; i < tmpCollapsedEdges.size(); i++) {
		if (tmpCollapsedEdges[i].submeshID == submeshID) {
			return i;
		}
	}
	return -1; // Not found
}

bool ProgressiveMeshGenerator::hasSrcID(unsigned int srcID, unsigned short submeshID)
{
	// This will only return exact matches.
	for (int i = 0; i < tmpCollapsedEdges.size(); i++) {
		if (tmpCollapsedEdges[i].srcID == srcID && tmpCollapsedEdges[i].submeshID == submeshID) {
			return true;
		}
	}
	return false; // Not found
}

void ProgressiveMeshGenerator::assertValidMesh()
{
	// Allows to find bugs in collapsing.
#ifndef NDEBUG
	size_t s1 = mUniqueVertexSet.size();
	size_t s2 = mCollapseCostSet.size();
	CollapseCostSet::iterator it = mCollapseCostSet.begin();
	CollapseCostSet::iterator itEnd = mCollapseCostSet.end();
	while (it != itEnd) {
		assertValidVertex(*it);
		it++;
	}
#endif
}
void ProgressiveMeshGenerator::assertValidVertex(PMVertex* v)
{
	// Allows to find bugs in collapsing.
#ifndef NDEBUG
	VertexTriangles::iterator it = v->triangles.begin();
	VertexTriangles::iterator itEnd = v->triangles.end();
	for (; it != itEnd; it++) {
		size_t tID = *it;
		PMTriangle* t = &mTriangleList[tID];
		for (int i = 0; i < 3; i++) {
			assert(t->vertex[i]->costSetPosition != mCollapseCostSet.end());
			assert(t->vertex[i]->edges.find(PMEdge(t->vertex[i]->collapseTo)) != t->vertex[i]->edges.end());
			for (int n = 0; n < 3; n++) {
				if (i != n) {
					VertexEdges::iterator it = t->vertex[i]->edges.find(PMEdge(t->vertex[n]));
					assert(it != t->vertex[i]->edges.end());
					assert(it->collapseCost != UNINITIALIZED_COLLAPSE_COST);
					assert(it->collapseCost != NEEDS_UPDATE_COLLAPSE_COST);
				} else {
					assert(t->vertex[i]->edges.find(PMEdge(t->vertex[n])) == t->vertex[i]->edges.end());
				}
			}
		}
	}
#endif // ifndef NDEBUG
}

void ProgressiveMeshGenerator::collapse(PMVertex* src)
{
	PMVertex* dst = src->collapseTo;
	assertValidVertex(dst);
	assertValidVertex(src);
	assert(src->collapseCost != NEVER_COLLAPSE_COST);
	assert(src->collapseCost != NEEDS_UPDATE_COLLAPSE_COST);
	assert(!src->edges.empty());
	assert(!src->triangles.empty());
	assert(src->edges.find(PMEdge(dst)) != src->edges.end());

	// It may have vertexIDs and triangles from different submeshes(different vertex buffers),
	// so we need to connect them correctly based on deleted triangle's edge.
	// mCollapsedEdgeIDs will be used, when looking up the connections for replacement.
	tmpCollapsedEdges.clear();
	VertexTriangles::iterator it = src->triangles.begin();
	VertexTriangles::iterator itEnd = src->triangles.end();
	for (; it != itEnd; it++) {
		size_t triangleID = *it;
		PMTriangle* triangle = &mTriangleList[triangleID];
		if (triangle->hasVertex(dst)) {
			// Remove a triangle
			// Tasks:
			// 1. Add it to the collapsed edges list.
			// 2. Reduce index count for the Lods, which will not have this triangle.
			// 3. Remove references/pointers to this triangle.

			// 1. task
			unsigned int srcID = triangle->getVertexID(src);
			if (!hasSrcID(srcID, triangle->submeshID)) {
				tmpCollapsedEdges.push_back(CollapsedEdge(srcID, triangle->getVertexID(dst), triangle->submeshID));
			}

			// 2. task
			triangle->maxLod = mCurLod;
			unsigned short maxLod = mLodIndexCountMatrix.size2();
			for (unsigned short i = mCurLod; i < maxLod; i++) {
				mLodIndexCountMatrix(triangle->submeshID, i) -= 3;
			}
			// 3. task
			removeTriangleFromEdges(triangleID, src);
		}
	}
	assert(tmpCollapsedEdges.size());
	assert(dst->edges.find(PMEdge(src)) == dst->edges.end());

	CollapsedEdges::iterator it2End = tmpCollapsedEdges.begin();
	it = src->triangles.begin();
	for (; it != itEnd; it++) {
		size_t triangleID = *it;
		PMTriangle* triangle = &mTriangleList[triangleID];
		if (!triangle->hasVertex(dst)) {
			// Replace a triangle
			// Since we are baking all Lod levels in a single loop, we can't really replace it, because some Lods may use it.
			// Instead we will create a copy of the triangle with the replaced vertices and leave the old unmodified.
			// The triangle contains only POD members, so I could use memcpy for copying, but I hope the compiler will optimize it automatically.
			// Tasks:
			// 1. Determine the edge which we will move along.(we need to modify single vertex only)
			// 2. Create a copy of the triangle and add it to mTriangleList.
			// 3. Set the minLod for the new triangle, so it will not be added to Lod levels which don't need anymore reductions.
			// 4. Move new triangle along the edge.
			// 5. Replace references/pointers from old triangle to new triangle.
			// 6. Set the maxLod for the old triangle, so it will not be added to Lod levels which need more reductions.

			// 1. task
			unsigned int srcID = triangle->getVertexID(src);
			size_t id = findDstID(srcID, triangle->submeshID);
			if (id == -1) {
				// Not found any edge to move along.
				// Destroy the triangle.
				triangle->maxLod = mCurLod;
				unsigned short maxLod = mLodIndexCountMatrix.size2();
				for (unsigned short i = mCurLod; i < maxLod; i++) {
					mLodIndexCountMatrix(triangle->submeshID, i) -= 3;
				}
				removeTriangleFromEdges(triangleID, src);
				continue;
			}
			unsigned int dstID = tmpCollapsedEdges[id].dstID;

			PMTriangle tmp(*triangle);

			// 6. task
			triangle->maxLod = mCurLod;

			// 2. task
			// The following code would crash if vector is resized, so we need a temporary copy on stack.
			// mTriangleList.push_back(*triangle);
			mTriangleList.push_back(tmp);
			PMTriangle* newtriangle = &mTriangleList.back();
#ifndef NDEBUG
			triangle = NULL; // The pointer may be invalidated.
#endif
			// 3. task
			newtriangle->minLod = mCurLod;

			// Should we update normals here??
			newtriangle->computeNormal();

			// 4. task
			newtriangle->replaceVertexID(srcID, dstID, dst);

			// 5. task
			addTriangleToEdges(mTriangleList.size() - 1);
			removeTriangleFromEdges(triangleID, src);

		}
	}

	VertexEdges::iterator it3 = src->edges.begin();
	VertexEdges::iterator it3End = src->edges.end();
	for (; it3 != it3End; it3++) {
		updateVertexCollapseCost(it3->dst);
	}

#ifndef NDEBUG
	it3 = src->edges.begin();
	it3End = src->edges.end();
	for (; it3 != it3End; it3++) {
		assertOutdatedCollapseCost(it3->dst);
	}
	it3 = dst->edges.begin();
	it3End = dst->edges.end();
	for (; it3 != it3End; it3++) {
		assertOutdatedCollapseCost(it3->dst);
	}
	assertOutdatedCollapseCost(dst);
#endif // ifndef NDEBUG
	mCollapseCostSet.erase(src->costSetPosition); // Remove src from collapse costs.
	src->edges.clear(); // Free memory
	src->triangles.clear(); // Free memory
	// TODO: Remove setting costSetPosition to end() for release build.
	src->costSetPosition = mCollapseCostSet.end();

	assertValidVertex(dst);
}
size_t ProgressiveMeshGenerator::calcLodVertexCount(const LodConfig& lodConfig)
{
	size_t uniqueVertices = mUniqueVertexSet.size();
	switch (lodConfig.reductionMethod) {
	case ProgressiveMeshGenerator::VRM_PROPORTIONAL:
		return uniqueVertices - (uniqueVertices * lodConfig.reductionValue);

	case ProgressiveMeshGenerator::VRM_CONSTANT:
		return uniqueVertices - lodConfig.reductionValue;

	default:
		assert(0);
		return uniqueVertices;
	}
}

void ProgressiveMeshGenerator::bakeLods(const LodConfigList& lodConfigs)
{
	union IndexBufferPointer {
		unsigned short* pshort;
		unsigned int* pint;
	};

	typedef boost::numeric::ublas::matrix<IndexBufferPointer> IndexBufferMatrix;
	IndexBufferMatrix buffers(mMesh.getNumSubMeshes(), lodConfigs.size());
	mMesh.removeLodLevels();

	// Create buffers.
	for (unsigned short i = 0; i < buffers.size1(); i++) {
		Ogre::ProgressiveMesh::LODFaceList& lods = mMesh.getSubMesh(i)->mLodFaceList;

		for (int n = 0; n < buffers.size2(); n++) {
			int indexCount = mLodIndexCountMatrix(i, n);
			assert(indexCount >= 0);

			lods.push_back(OGRE_NEW Ogre::IndexData());
			lods.back()->indexStart = 0;
			lods.back()->indexCount = indexCount;
			lods.back()->indexBuffer = Ogre::HardwareBufferManager::getSingleton().createIndexBuffer(
			    mIndexBufferInfoList[i].indexSize == 2 ?
			    Ogre::HardwareIndexBuffer::IT_16BIT : Ogre::HardwareIndexBuffer::IT_32BIT,
			    indexCount, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY, false);
			buffers(i, n).pshort = static_cast<unsigned short*>(lods.back()->indexBuffer->
			                                                    lock(0, lods.back()->indexBuffer->getSizeInBytes(),
			                                                         Ogre::HardwareBuffer::HBL_DISCARD));
		}
	}
	size_t count = 0;
	size_t indexCount = mLodIndexCountMatrix(0, 0);
	// Fill buffers.
	for (size_t i = 0; i < mTriangleList.size(); i++) {
		mTriangleList[i].maxLod = std::min<unsigned short>(mTriangleList[i].maxLod, lodConfigs.size());
		if (mIndexBufferInfoList[mTriangleList[i].submeshID].indexSize == 2) {
			for (int n = mTriangleList[i].minLod; n < mTriangleList[i].maxLod; n++) {
				count++;
				for (int m = 0; m < 3; m++) {
					*(buffers(mTriangleList[i].submeshID,
					          n).pshort++) = static_cast<unsigned short>(mTriangleList[i].vertexID[m]);
				}
			}
		} else {
			for (int n = mTriangleList[i].minLod; n < mTriangleList[i].maxLod; n++) {
				for (int m = 0; m < 3; m++) {
					*(buffers(i, n).pint++) = static_cast<unsigned int>(mTriangleList[i].vertexID[m]);
				}
			}
		}
	}

	// Close buffers.
	for (unsigned short i = 0; i < mMesh.getNumSubMeshes(); i++) {
		Ogre::ProgressiveMesh::LODFaceList& lods = mMesh.getSubMesh(i)->mLodFaceList;
		for (int n = 0; n < lodConfigs.size(); n++) {
			lods[n]->indexBuffer->unlock();
		}
	}
}

ProgressiveMeshGenerator::PMEdge::PMEdge(PMVertex* destination) :
	dst(destination),
	collapseCost(UNINITIALIZED_COLLAPSE_COST)
{

}

bool ProgressiveMeshGenerator::PMEdge::operator< (const PMEdge& other) const
{
	return (size_t) dst < (size_t) other.dst;   // Comparing pointers for uniqueness.
}


bool ProgressiveMeshGenerator::PMVertex::operator== (const PMVertex& other) const
{
	return position == other.position;
}

ProgressiveMeshGenerator::PMEdge* ProgressiveMeshGenerator::addEdge(PMVertex* v, const PMEdge& edge)
{
	assert(edge.dst != v);
	std::pair<VertexEdges::iterator, bool> ret;
	ret = v->edges.insert(edge);
	if (ret.second) {
		PMEdge* e = getPointer(ret.first);
		e->refCount = 1;
		return e;
	} else {
		PMEdge* e = getPointer(ret.first);
		e->refCount++;
		return e;
	}
}

ProgressiveMeshGenerator::PMEdge* ProgressiveMeshGenerator::removeEdge(PMVertex* v, const PMEdge& edge)
{
	VertexEdges::iterator it = v->edges.find(edge);
	assert(it != v->edges.end());
	PMEdge* e = getPointer(it);
	if (e->refCount == 1) {
		PMVertex* v2 = e->dst;
		v->edges.erase(it);
		return NULL;
	}
	e->refCount--;
	return e;
}

}
}
}
