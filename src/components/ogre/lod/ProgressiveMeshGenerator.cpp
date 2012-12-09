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

// Optimize for best output instead of performance. Use OGRE_DOUBLE_PRECISION for even better results.
// #define PM_BEST_QUALITY
// #define NDEBUG


#include "ProgressiveMeshGenerator.h"
#include "EmberOgreMesh.h"

#include "framework/LoggingInstance.h"

#include <OgreHardwareIndexBuffer.h>
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

#define NEVER_COLLAPSE_COST std::numeric_limits<Ogre::Real>::max()
#define UNINITIALIZED_COLLAPSE_COST (std::numeric_limits<Ogre::Real>::infinity())

ProgressiveMeshGenerator::ProgressiveMeshGenerator() :
	mMesh(NULL)
{
	assert(NEVER_COLLAPSE_COST < UNINITIALIZED_COLLAPSE_COST && NEVER_COLLAPSE_COST != UNINITIALIZED_COLLAPSE_COST);

}

ProgressiveMeshGenerator::~ProgressiveMeshGenerator()
{
}

void ProgressiveMeshGenerator::tuneContainerSize()
{
	// Get Vertex count for container tuning.
	bool sharedVerticesAdded = false;
	size_t vertexCount = 0;
	size_t vertexLookupSize = 0;
	size_t sharedVertexLookupSize = 0;
	unsigned short submeshCount = mMesh->getNumSubMeshes();
	for (unsigned short i = 0; i < submeshCount; i++) {
		const Ogre::SubMesh* submesh = mMesh->getSubMesh(i);
		if (!submesh->useSharedVertices) {
			size_t count = submesh->vertexData->vertexCount;
			vertexLookupSize = std::max(vertexLookupSize, count);
			vertexCount += count;
		} else if (!sharedVerticesAdded) {
			sharedVerticesAdded = true;
			sharedVertexLookupSize = mMesh->sharedVertexData->vertexCount;
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

void ProgressiveMeshGenerator::initialize()
{
	unsigned short submeshCount = mMesh->getNumSubMeshes();
	for (unsigned short i = 0; i < submeshCount; ++i) {
		const Ogre::SubMesh* submesh = mMesh->getSubMesh(i);
		Ogre::VertexData* vertexData = (submesh->useSharedVertices ? mMesh->sharedVertexData : submesh->vertexData);
		addVertexData(vertexData, submesh->useSharedVertices);
		addIndexData(submesh->indexData, submesh->useSharedVertices, i);
	}

	// These were only needed for addIndexData() and addVertexData().
	mSharedVertexLookup.clear();
	mVertexLookup.clear();
	mUniqueVertexSet.clear();
}

void ProgressiveMeshGenerator::PMTriangle::computeNormal()
{
	// Cross-product 2 edges
	Ogre::Vector3 e1 = vertex[1]->position - vertex[0]->position;
	Ogre::Vector3 e2 = vertex[2]->position - vertex[1]->position;

	normal = e1.crossProduct(e2);
	normal.normalise();
}
void ProgressiveMeshGenerator::addVertexData(Ogre::VertexData* vertexData, bool useSharedVertexLookup)
{
	if ((useSharedVertexLookup && !mSharedVertexLookup.empty()) // We already loaded the shared vertex buffer.
	    || vertexData->vertexCount == 0) { // Locking a zero length buffer on linux with nvidia cards fails.
		return;
	}

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

	VertexLookupList& lookup = useSharedVertexLookup ? mSharedVertexLookup : mVertexLookup;
	lookup.clear();

	// Loop through all vertices and insert them to the Unordered Map.
	for (; vertex < vEnd; vertex += vSize) {
		float* pFloat;
		elem->baseVertexPointerToElement(vertex, &pFloat);
		mVertexList.push_back(PMVertex());
		PMVertex* v = &mVertexList.back();
		v->position.x = pFloat[0];
		v->position.y = pFloat[1];
		v->position.z = pFloat[2];
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
	vbuf->unlock();
}
template<typename IndexType>
void ProgressiveMeshGenerator::addIndexDataImpl(IndexType* iPos, const IndexType* iEnd,
                                                VertexLookupList& lookup,
                                                unsigned short submeshID)
{

	// Loop through all triangles and connect them to the vertices.
	for (; iPos < iEnd; iPos += 3) {
		// It should never reallocate or every pointer will be invalid.
		assert(mTriangleList.capacity() > mTriangleList.size());
		mTriangleList.push_back(PMTriangle());
		PMTriangle* tri = &mTriangleList.back();
		tri->isRemoved = false;
		tri->submeshID = submeshID;
		for (int i = 0; i < 3; i++) {
			// Invalid index: Index is bigger then vertex buffer size.
			assert(iPos[i] < lookup.size());
			tri->vertexID[i] = iPos[i];
			tri->vertex[i] = lookup[iPos[i]];
		}
		if (tri->isMalformed()) {
			std::stringstream str;
			str << "In " << /*mMesh->getName() <<*/ " malformed triangle found with ID: " << getTriangleID(tri) << ". " <<
			std::endl;
			printTriangle(tri, str);
			str << "It will be excluded from Lod level calculations.";
			S_LOG_WARNING(str.str());
			tri->isRemoved = true;
			mIndexBufferInfoList[tri->submeshID].indexCount -= 3;
			continue;
		}
		tri->computeNormal();
		addTriangleToEdges(tri);
	}
}

void ProgressiveMeshGenerator::addIndexData(Ogre::IndexData* indexData, bool useSharedVertexLookup, unsigned short submeshID)
{
	const Ogre::HardwareIndexBufferSharedPtr& ibuf = indexData->indexBuffer;
	size_t isize = ibuf->getIndexSize();
	mIndexBufferInfoList[submeshID].indexSize = isize;
	mIndexBufferInfoList[submeshID].indexCount = indexData->indexCount;
	if (indexData->indexCount == 0) {
		// Locking a zero length buffer on linux with nvidia cards fails.
		return;
	}
	VertexLookupList& lookup = useSharedVertexLookup ? mSharedVertexLookup : mVertexLookup;

	// Lock the buffer for reading.
	char* iStart = static_cast<char*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
	char* iEnd = iStart + ibuf->getSizeInBytes();
	if (isize == sizeof(unsigned short)) {
		addIndexDataImpl<unsigned short>((unsigned short*) iStart, (unsigned short*) iEnd, lookup, submeshID);
	} else {
		// Unsupported index size.
		assert(isize == sizeof(unsigned int));
		addIndexDataImpl<unsigned int>((unsigned int*) iStart, (unsigned int*) iEnd, lookup, submeshID);
	}
	ibuf->unlock();
}


bool ProgressiveMeshGenerator::PMTriangle::hasVertex(const PMVertex* v) const
{
	return (v == vertex[0] || v == vertex[1] || v == vertex[2]);
}

void ProgressiveMeshGenerator::replaceVertexID(PMTriangle* triangle, unsigned int oldID, unsigned int newID, PMVertex* dst)
{
	dst->triangles.addNotExists(triangle);
	// NOTE: triangle is not removed from src. This is implementation specific optimization.

	// Its up to the compiler to unroll everything.
	for (int i = 0; i < 3; i++) {
		if (triangle->vertexID[i] == oldID) {
			for (int n = 0; n < 3; n++) {
				if (i != n) {
					// This is implementation specific optimization to remove following line.
					// removeEdge(triangle->vertex[i], PMEdge(triangle->vertex[n]));

					removeEdge(triangle->vertex[n], PMEdge(triangle->vertex[i]));
					addEdge(triangle->vertex[n], PMEdge(dst));
					addEdge(dst, PMEdge(triangle->vertex[n]));
				}
			}
			triangle->vertex[i] = dst;
			triangle->vertexID[i] = newID;
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

void ProgressiveMeshGenerator::removeTriangleFromEdges(PMTriangle* triangle, PMVertex* skip)
{
	// skip is needed if we are iterating on the vertex's edges or triangles.
	for (int i = 0; i < 3; i++) {
		if (triangle->vertex[i] != skip) {
			triangle->vertex[i]->triangles.removeExists(triangle);
		}
	}
	for (int i = 0; i < 3; i++) {
		for (int n = 0; n < 3; n++) {
			if (i != n && triangle->vertex[i] != skip) {
				removeEdge(triangle->vertex[i], PMEdge(triangle->vertex[n]));
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

bool ProgressiveMeshGenerator::PMTriangle::isMalformed()
{
	return vertex[0] == vertex[1] || vertex[0] == vertex[2] || vertex[1] == vertex[2];
}

bool ProgressiveMeshGenerator::isDuplicateTriangle(PMTriangle* triangle, PMTriangle* triangle2)
{
	for (int i = 0; i < 3; i++) {
		if (triangle->vertex[i] != triangle2->vertex[0] ||
		    triangle->vertex[i] != triangle2->vertex[1] ||
		    triangle->vertex[i] != triangle2->vertex[2]) {
			return false;
		}
	}
	return true;
}

ProgressiveMeshGenerator::PMTriangle* ProgressiveMeshGenerator::isDuplicateTriangle(PMTriangle* triangle)
{
	// duplicate triangle detection (where all vertices has the same position)
	VTriangles::iterator itEnd = triangle->vertex[0]->triangles.end();
	VTriangles::iterator it = triangle->vertex[0]->triangles.begin();
	for (; it != itEnd; it++) {
		PMTriangle* t = *it;
		if (isDuplicateTriangle(triangle, t)) {
			return *it;
		}
	}
	return NULL;
}
int ProgressiveMeshGenerator::getTriangleID(PMTriangle* triangle)
{
	return (triangle - &mTriangleList[0]) / sizeof(PMTriangle);
}
void ProgressiveMeshGenerator::addTriangleToEdges(PMTriangle* triangle)
{
#ifdef PM_BEST_QUALITY
	PMTriangle* duplicate = isDuplicateTriangle(triangle);
	if (duplicate != NULL) {
		std::stringstream str;
		str << "In " << /*mMesh->getName() <<*/ " duplicate triangle found." << std::endl;
		str << "Triangle " << getTriangleID(triangle) << " positions:" << std::endl;
		printTriangle(triangle, str);
		str << "Triangle " << getTriangleID(duplicate) << " positions:" << std::endl;
		printTriangle(duplicate, str);
		str << "Triangle " << getTriangleID(triangle) << " will be excluded from Lod level calculations.";
		S_LOG_WARNING(str.str());
		triangle->isRemoved = true;
		mIndexBufferInfoList[triangle->submeshID].indexCount -= 3;
		return;
	}
#endif // ifdef PM_BEST_QUALITY
	for (int i = 0; i < 3; i++) {
		triangle->vertex[i]->triangles.addNotExists(triangle);
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
	VEdges::const_iterator it = vertex->edges.begin();
	VEdges::const_iterator itEnd = vertex->edges.end();
	for (; it != itEnd; it++) {
		if (it->refCount == 1) {
			return true;
		}
	}
	return false;
}

ProgressiveMeshGenerator::PMTriangle* ProgressiveMeshGenerator::findSideTriangle(const PMVertex* v1, const PMVertex* v2)
{
	VTriangles::const_iterator it = v1->triangles.begin();
	VTriangles::const_iterator itEnd = v1->triangles.end();
	for (; it != itEnd; it++) {
		PMTriangle* triangle = *it;
		if (triangle->hasVertex(v2)) {
			return triangle;
		}
	}
	assert(0);
	return NULL;
}

ProgressiveMeshGenerator::PMEdge* ProgressiveMeshGenerator::getPointer(VEdges::iterator it)
{
	return &*it;
}

void ProgressiveMeshGenerator::computeCosts()
{
	mCollapseCostSet.clear();
	VertexList::iterator it = mVertexList.begin();
	VertexList::iterator itEnd = mVertexList.end();
	for (; it != itEnd; it++) {
		if (!it->edges.empty()) {
			computeVertexCollapseCost(&*it);

		} else {
			std::stringstream str;
			str << "In " << /*mMesh->getName() <<*/ " never used vertex found with ID: " << mCollapseCostSet.size() << "."
			    << std::endl
			    << "Vertex position: ("
			    << it->position.x << ", "
			    << it->position.y << ", "
			    << it->position.z << ")" << std::endl
			    << "It will be excluded from Lod level calculations.";
			S_LOG_WARNING(str.str());
		}
	}
}

void ProgressiveMeshGenerator::computeVertexCollapseCost(PMVertex* vertex)
{
	vertex->collapseCost = UNINITIALIZED_COLLAPSE_COST;
	assert(!vertex->edges.empty());
	VEdges::iterator it = vertex->edges.begin();
	for (; it != vertex->edges.end(); it++) {
		it->collapseCost = computeEdgeCollapseCost(vertex, getPointer(it));
		if (vertex->collapseCost > it->collapseCost) {
			vertex->collapseCost = it->collapseCost;
			vertex->collapseTo = it->dst;
		}
	}
	assert(vertex->collapseCost != UNINITIALIZED_COLLAPSE_COST);
	vertex->costSetPosition = mCollapseCostSet.insert(vertex);

}

Ogre::Real ProgressiveMeshGenerator::computeEdgeCollapseCost(PMVertex* src, PMEdge* dstEdge)
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
		VTriangles::iterator it = src->triangles.begin();
		VTriangles::iterator itEnd = src->triangles.end();
		for (; it != itEnd; it++) {
			PMTriangle* triangle = *it;
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

	Ogre::Real cost;

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
			// PMTriangle* triangle = findSideTriangle(src, dst);

			cost = 0.0f;
			Ogre::Vector3 collapseEdge = src->position - dst->position;
			collapseEdge.normalise();
			VEdges::iterator it = src->edges.begin();
			VEdges::iterator itEnd = src->edges.end();
			for (; it != itEnd; it++) {
				PMVertex* neighbor = it->dst;
				if (neighbor != dst && it->refCount == 1) {
					Ogre::Vector3 otherBorderEdge = src->position - neighbor->position;
					otherBorderEdge.normalise();
					// This time, the nearer the dot is to -1, the better, because that means
					// the edges are opposite each other, therefore less kinkiness
					// Scale into [0..1]
					Ogre::Real kinkiness = (otherBorderEdge.dotProduct(collapseEdge) + 1.002f) * 0.5f;
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
		VTriangles::iterator it = src->triangles.begin();
		VTriangles::iterator itEnd = src->triangles.end();
		for (; it != itEnd; it++) {
			Ogre::Real mincurv = 1.0f; // curve for face i and closer side to it
			PMTriangle* triangle = *it;
			VTriangles::iterator it2 = src->triangles.begin();
			for (; it2 != itEnd; it2++) {
				PMTriangle* triangle2 = *it2;
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
			cost += mMeshBoundingSphereRadius;
		} else {
			cost += mMeshBoundingSphereRadius * 0.5;
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
	VEdges::iterator it = vertex->edges.begin();
	VEdges::iterator itEnd = vertex->edges.end();
	for (; it != itEnd; it++) {
		assert(it->collapseCost == computeEdgeCollapseCost(vertex, getPointer(it)));
		PMVertex* neighbor = it->dst;
		VEdges::iterator it2 = neighbor->edges.begin();
		VEdges::iterator it2End = neighbor->edges.end();
		for (; it2 != it2End; it2++) {
			assert(it2->collapseCost == computeEdgeCollapseCost(neighbor, getPointer(it2)));
		}
	}
#endif // ifndef NDEBUG
}

void ProgressiveMeshGenerator::updateVertexCollapseCost(PMVertex* vertex)
{
	Ogre::Real collapseCost = UNINITIALIZED_COLLAPSE_COST;
	PMVertex* collapseTo;
	VEdges::iterator it = vertex->edges.begin();
	VEdges::iterator itEnd = vertex->edges.end();
	for (; it != itEnd; it++) {
		it->collapseCost = computeEdgeCollapseCost(vertex, getPointer(it));
		if (collapseCost > it->collapseCost) {
			collapseCost = it->collapseCost;
			collapseTo = it->dst;
		}
	}
	if (collapseCost != vertex->collapseCost || vertex->collapseTo != collapseTo) {
		assert(vertex->collapseTo != NULL);
		assert(vertex->costSetPosition != mCollapseCostSet.end());
		mCollapseCostSet.erase(vertex->costSetPosition);
		if (collapseCost != UNINITIALIZED_COLLAPSE_COST) {
			vertex->collapseCost = collapseCost;
			vertex->collapseTo = collapseTo;
			vertex->costSetPosition = mCollapseCostSet.insert(vertex);
		} else {
#ifndef NDEBUG
			vertex->collapseTo == NULL;
			vertex->costSetPosition = mCollapseCostSet.end();
#endif
		}
	}
}

void ProgressiveMeshGenerator::build(LodConfig& lodConfig)
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
	mMesh->getLodStrategy()->assertSorted(values);
#endif // if ifndef NDEBUG
	mMesh = lodConfig.mesh;
	mMeshBoundingSphereRadius = mMesh->getBoundingSphereRadius();
	mMesh->removeLodLevels();
	tuneContainerSize();
	initialize(); // Load vertices and triangles
	computeCosts(); // Calculate all collapse costs
	assertValidMesh();

	computeLods(lodConfig);

	static_cast<EmberOgreMesh*>(mMesh.get())->_configureMeshLodUsage(lodConfig);
}

void ProgressiveMeshGenerator::computeLods(LodConfig& lodConfigs)
{
	size_t vertexCount = mVertexList.size();
	size_t lastBakeVertexCount = vertexCount;
	size_t lodCount = lodConfigs.levels.size();
	for (mCurLod = 0; mCurLod < lodCount; mCurLod++) {
		size_t neededVertexCount = calcLodVertexCount(lodConfigs.levels[mCurLod]);
		for (; neededVertexCount < vertexCount; vertexCount--) {
			CollapseCostSet::iterator nextIndex = mCollapseCostSet.begin();
			if (nextIndex != mCollapseCostSet.end() && (*nextIndex)->collapseCost < mCollapseCostLimit) {
				collapse(*nextIndex);
			} else {
				break;
			}
		}
		lodConfigs.levels[mCurLod].outUniqueVertexCount = vertexCount;
		lodConfigs.levels[mCurLod].outSkipped = (lastBakeVertexCount == vertexCount);
		if (!lodConfigs.levels[mCurLod].outSkipped) {
			lastBakeVertexCount = vertexCount;
			assertValidMesh();
			bakeLods(lodConfigs.levels[mCurLod]);
		}
	}
}

size_t ProgressiveMeshGenerator::findDstID(unsigned int srcID, unsigned short submeshID)
{
	// Tries to find a compatible edge.

	// Exact match search.
	for (size_t i = 0; i < tmpCollapsedEdges.size(); i++) {
		if (tmpCollapsedEdges[i].srcID == srcID && tmpCollapsedEdges[i].submeshID == submeshID) {
			return i;
		}
	}

	// Usable match search.
	for (size_t i = 0; i < tmpCollapsedEdges.size(); i++) {
		if (tmpCollapsedEdges[i].submeshID == submeshID) {
			return i;
		}
	}
	return -1; // Not found
}

bool ProgressiveMeshGenerator::hasSrcID(unsigned int srcID, unsigned short submeshID)
{
	// This will only return exact matches.
	for (size_t i = 0; i < tmpCollapsedEdges.size(); i++) {
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
	VTriangles::iterator it = v->triangles.begin();
	VTriangles::iterator itEnd = v->triangles.end();
	for (; it != itEnd; it++) {
		PMTriangle* t = *it;
		for (int i = 0; i < 3; i++) {
			assert(t->vertex[i]->costSetPosition != mCollapseCostSet.end());
			t->vertex[i]->edges.findExists(PMEdge(t->vertex[i]->collapseTo));
			for (int n = 0; n < 3; n++) {
				if (i != n) {
					VEdges::iterator it = t->vertex[i]->edges.findExists(PMEdge(t->vertex[n]));
					assert(it->collapseCost != UNINITIALIZED_COLLAPSE_COST);
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
	assert(src->collapseCost != UNINITIALIZED_COLLAPSE_COST);
	assert(!src->edges.empty());
	assert(!src->triangles.empty());
	assert(src->edges.find(PMEdge(dst)) != src->edges.end());

	// It may have vertexIDs and triangles from different submeshes(different vertex buffers),
	// so we need to connect them correctly based on deleted triangle's edge.
	// mCollapsedEdgeIDs will be used, when looking up the connections for replacement.
	tmpCollapsedEdges.clear();
	VTriangles::iterator it = src->triangles.begin();
	VTriangles::iterator itEnd = src->triangles.end();
	for (; it != itEnd; it++) {
		PMTriangle* triangle = *it;
		if (triangle->hasVertex(dst)) {
			// Remove a triangle
			// Tasks:
			// 1. Add it to the collapsed edges list.
			// 2. Reduce index count for the Lods, which will not have this triangle.
			// 3. Mark as removed, so it will not be added in upcoming Lod levels.
			// 4. Remove references/pointers to this triangle.

			// 1. task
			unsigned int srcID = triangle->getVertexID(src);
			if (!hasSrcID(srcID, triangle->submeshID)) {
				tmpCollapsedEdges.push_back(PMCollapsedEdge());
				tmpCollapsedEdges.back().srcID = srcID;
				tmpCollapsedEdges.back().dstID = triangle->getVertexID(dst);
				tmpCollapsedEdges.back().submeshID = triangle->submeshID;
			}

			// 2. task
			mIndexBufferInfoList[triangle->submeshID].indexCount -= 3;

			// 3. task
			triangle->isRemoved = true;

			// 4. task
			removeTriangleFromEdges(triangle, src);
		}
	}
	assert(tmpCollapsedEdges.size());
	assert(dst->edges.find(PMEdge(src)) == dst->edges.end());

	CollapsedEdges::iterator it2End = tmpCollapsedEdges.begin();
	it = src->triangles.begin();
	for (; it != itEnd; it++) {
		PMTriangle* triangle = *it;
		if (!triangle->hasVertex(dst)) {
			// Replace a triangle
			// Tasks:
			// 1. Determine the edge which we will move along. (we need to modify single vertex only)
			// 2. Move along the selected edge.

			// 1. task
			unsigned int srcID = triangle->getVertexID(src);
			size_t id = findDstID(srcID, triangle->submeshID);
			if (id == -1) {
				// Not found any edge to move along.
				// Destroy the triangle.
				triangle->isRemoved = true;
				mIndexBufferInfoList[triangle->submeshID].indexCount -= 3;
				removeTriangleFromEdges(triangle, src);
				continue;
			}
			unsigned int dstID = tmpCollapsedEdges[id].dstID;

			// 2. task
			replaceVertexID(triangle, srcID, dstID, dst);

#ifdef PM_BEST_QUALITY
			triangle->computeNormal();
#endif
		}
	}
#ifndef PM_BEST_QUALITY
	VEdges::iterator it3 = src->edges.begin();
	VEdges::iterator it3End = src->edges.end();
	for (; it3 != it3End; it3++) {
		updateVertexCollapseCost(it3->dst);
	}
#else
	// TODO: Find out why is this needed. assertOutdatedCollapseCost() fails on some
	// rare situations without this. For example goblin.mesh fails.
	typedef Ogre::SmallVector<PMVertex*, 64> UpdatableList;
	UpdatableList updatable;
	VEdges::iterator it3 = src->edges.begin();
	VEdges::iterator it3End = src->edges.end();
	for (; it3 != it3End; it3++) {
		updatable.push_back(it3->dst);
		VEdges::iterator it4End = it3->dst->edges.end();
		VEdges::iterator it4 = it3->dst->edges.begin();
		for (; it4 != it4End; it4++) {
			updatable.push_back(it4->dst);
		}
	}

	// Remove duplicates.
	UpdatableList::iterator it5 = updatable.begin();
	UpdatableList::iterator it5End = updatable.end();
	std::sort(it5, it5End);
	it5End = std::unique(it5, it5End);

	for (; it5 != it5End; it5++) {
		updateVertexCollapseCost(*it5);
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
#endif // ifndef PM_BEST_QUALITY
	mCollapseCostSet.erase(src->costSetPosition); // Remove src from collapse costs.
	src->edges.clear(); // Free memory
	src->triangles.clear(); // Free memory
#ifndef NDEBUG
	src->costSetPosition = mCollapseCostSet.end();
#endif
	assertValidVertex(dst);
}
size_t ProgressiveMeshGenerator::calcLodVertexCount(const LodLevel& lodConfig)
{
	size_t uniqueVertices = mVertexList.size();
	switch (lodConfig.reductionMethod) {
	case LodLevel::VRM_PROPORTIONAL:
		mCollapseCostLimit = NEVER_COLLAPSE_COST;
		return uniqueVertices - (uniqueVertices * lodConfig.reductionValue);

	case LodLevel::VRM_CONSTANT:
	{
		mCollapseCostLimit = NEVER_COLLAPSE_COST;
		size_t reduction = lodConfig.reductionValue;
		if (reduction < uniqueVertices) {
			return uniqueVertices - reduction;
		} else {
			return 0;
		}
	}

	case LodLevel::VRM_COLLAPSE_COST:
		mCollapseCostLimit = lodConfig.reductionValue;
		return 0;

	default:
		assert(0);
		return uniqueVertices;
	}
}

void ProgressiveMeshGenerator::bakeLods(const LodLevel& lodConfigs)
{

	unsigned short submeshCount = mMesh->getNumSubMeshes();
	std::auto_ptr<IndexBufferPointer> indexBuffer(new IndexBufferPointer[submeshCount]);

	// Create buffers.
	for (unsigned short i = 0; i < submeshCount; i++) {
		Ogre::SubMesh::LODFaceList& lods = mMesh->getSubMesh(i)->mLodFaceList;
		int indexCount = mIndexBufferInfoList[i].indexCount;
		assert(indexCount > 0);
		lods.push_back(OGRE_NEW Ogre::IndexData());
		lods.back()->indexStart = 0;
		lods.back()->indexCount = indexCount;

		bool isEmpty = false;
		if (indexCount == 0) {
			//If the index is empty we need to create a "dummy" triangle, just to keep the index from beíng empty.
			//The main reason for this is that the OpenGL render system will crash with a segfault unless the index has some values.
			//This should hopefully be removed with future versions of Ogre. The most preferred solution would be to add the
			//ability for a submesh to be excluded from rendering for a given LOD (which isn't possible currently 2012-12-09).
			indexCount = 3;
			isEmpty = true;
		}
		lods.back()->indexCount = indexCount;

		if (indexCount != 0) {
			lods.back()->indexBuffer = Ogre::HardwareBufferManager::getSingleton().createIndexBuffer(
				mIndexBufferInfoList[i].indexSize == 2 ?
				Ogre::HardwareIndexBuffer::IT_16BIT : Ogre::HardwareIndexBuffer::IT_32BIT,
				indexCount, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY, false);
			// Locking a zero length buffer on linux with nvidia cards fails, so we need to wrap it.
			indexBuffer.get()[i].pshort =
			    static_cast<unsigned short*>(lods.back()->indexBuffer->lock(0, lods.back()->indexBuffer->getSizeInBytes(),
			                                                                Ogre::HardwareBuffer::HBL_DISCARD));
		}

		if (isEmpty) {
			if (mIndexBufferInfoList[i].indexSize == 2) {
				for (int m = 0; m < 3; m++) {
					*(indexBuffer.get()[i].pshort++) =
					    static_cast<unsigned short>(0);
				}
			} else {
				for (int m = 0; m < 3; m++) {
					*(indexBuffer.get()[i].pint++) =
					    static_cast<unsigned short>(0);
				}
			}
		}

	}

	// Fill buffers.
	size_t triangleCount = mTriangleList.size();
	for (size_t i = 0; i < triangleCount; i++) {
		if (!mTriangleList[i].isRemoved) {
			assert(mIndexBufferInfoList[mTriangleList[i].submeshID].indexCount != 0);
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

	// Close buffers.
	for (unsigned short i = 0; i < submeshCount; i++) {
		if (mIndexBufferInfoList[mTriangleList[i].submeshID].indexCount) {
			Ogre::SubMesh::LODFaceList& lods = mMesh->getSubMesh(i)->mLodFaceList;
			lods.back()->indexBuffer->unlock();
		}
	}
}

ProgressiveMeshGenerator::PMEdge::PMEdge(PMVertex* destination) :
	dst(destination)
#ifndef NDEBUG
	, collapseCost(UNINITIALIZED_COLLAPSE_COST)
#endif
{

}

ProgressiveMeshGenerator::PMEdge::PMEdge(const PMEdge& b)
{
	*this = b;
}

bool ProgressiveMeshGenerator::PMEdge::operator< (const PMEdge& other) const
{
	return (size_t) dst < (size_t) other.dst;   // Comparing pointers for uniqueness.
}

ProgressiveMeshGenerator::PMEdge& ProgressiveMeshGenerator::PMEdge::operator= (const PMEdge& b)
{
	dst = b.dst;
	collapseCost = b.collapseCost;
	refCount = b.refCount;
	return *this;
}

bool ProgressiveMeshGenerator::PMEdge::operator== (const PMEdge& other) const
{
	return dst == other.dst;
}

void ProgressiveMeshGenerator::addEdge(PMVertex* v, const PMEdge& edge)
{
	assert(edge.dst != v);
	VEdges::iterator it;
	it = v->edges.add(edge);
	if (it == v->edges.end()) {
		v->edges.back().refCount = 1;
	} else {
		it->refCount++;
	}
}

void ProgressiveMeshGenerator::removeEdge(PMVertex* v, const PMEdge& edge)
{
	VEdges::iterator it = v->edges.findExists(edge);
	if (it->refCount == 1) {
		v->edges.remove(it);
	} else {
		it->refCount--;
	}
}

void ProgressiveMeshGenerator::cleanupMemory()
{
	this->mCollapseCostSet.clear();
	this->mIndexBufferInfoList.clear();
	this->mSharedVertexLookup.clear();
	this->mVertexLookup.clear();
	this->mUniqueVertexSet.clear();
	this->mVertexList.clear();
	this->mTriangleList.clear();
}

bool ProgressiveMeshGenerator::PMVertexEqual::operator() (const PMVertex* lhs, const PMVertex* rhs) const
{
	return lhs->position == rhs->position;
}

bool ProgressiveMeshGenerator::PMCollapseCostLess::operator() (const PMVertex* lhs, const PMVertex* rhs) const
{
	return lhs->collapseCost < rhs->collapseCost;
}

size_t ProgressiveMeshGenerator::PMVertexHash::operator() (const PMVertex* v) const
{
	boost::hash<Ogre::Real> hasher;
	return hasher(v->position.x)
	       ^ hasher(v->position.y)
	       ^ hasher(v->position.z);
}

template<typename T, unsigned S>
void ProgressiveMeshGenerator::VectorSet<T, S>::addNotExists(const T& item)
{
	assert(find(item) == baseClass::end());
	baseClass::push_back(item);
}

template<typename T, unsigned S>
void ProgressiveMeshGenerator::VectorSet<T, S>::remove(iterator it)
{
	// Thats my trick to remove an item from the vector very fast!
	// It works similar to the heap_pop().
	// It swaps the removable item to the back, then pops it.
	*it = baseClass::back();
	baseClass::pop_back();
}

template<typename T, unsigned S>
typename ProgressiveMeshGenerator::VectorSet<T, S>::iterator ProgressiveMeshGenerator::VectorSet<T, S>::add(const T& item)
{
	iterator it = find(item);
	if (it == baseClass::end()) {
		baseClass::push_back(item);
		return baseClass::end();
	}
	return it;
}

template<typename T, unsigned S>
void ProgressiveMeshGenerator::VectorSet<T, S>::removeExists(const T& item)
{
	iterator it = find(item);
	assert(it != end());
	remove(it);
}

template<typename T, unsigned S>
bool ProgressiveMeshGenerator::VectorSet<T, S>::remove(const T& item)
{
	iterator it = find(item);
	if (it != baseClass::end()) {
		remove(it);
		return true;
	} else {
		return false;
	}
}

template<typename T, unsigned S>
void ProgressiveMeshGenerator::VectorSet<T, S>::replaceExists(const T& oldItem, const T& newItem)
{
	iterator it = find(oldItem);
	assert(it != end());
	*it = newItem;
}

template<typename T, unsigned S>
bool ProgressiveMeshGenerator::VectorSet<T, S>::has(const T& item)
{
	return find(item) != baseClass::end();
}

template<typename T, unsigned S>
typename ProgressiveMeshGenerator::VectorSet<T, S>::iterator ProgressiveMeshGenerator::VectorSet<T, S>::find(const T& item)
{
	return std::find(baseClass::begin(), baseClass::end(), item);
}

template<typename T, unsigned S>
typename ProgressiveMeshGenerator::VectorSet<T, S>::iterator ProgressiveMeshGenerator::VectorSet<T, S>::findExists(
    const T& item)
{
	iterator it = find(item);
	assert(it != baseClass::end());
	return it;
}

}
}
}
