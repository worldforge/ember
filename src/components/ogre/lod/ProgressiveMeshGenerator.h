/*
 * Copyright (c) 2013 Peter Szucs <peter.szucs.dev@gmail.com>
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
 */

#ifndef PROGRESSIVEMESHGENERATOR_H
#define PROGRESSIVEMESHGENERATOR_H

#include "LodConfig.h"

#include <OgrePrerequisites.h>
#include <OgreVector3.h>
#include "OgreSmallVector.h"
#include <OgreMesh.h>

#include <boost/unordered_set.hpp>

#include <vector>
#include <set>

namespace Ember
{
namespace OgreView
{
namespace Lod
{

struct LodConfig;

/**
 * @brief Improved version of Ogre::ProgressiveMesh.
 */
class ProgressiveMeshGenerator
{
public:

	/**
	 * @brief Ctor.
	 */
	ProgressiveMeshGenerator();
	virtual ~ProgressiveMeshGenerator();

	/**
	 * @brief Builds the Lod levels for a submesh based on a LodConfigList.
	 *
	 * @param lodConfigs Specification of the requested Lod levels.
	 */
	void build(LodConfig& lodConfigs);
protected:

	// VectorSet is basically a helper to use a vector as a small set container.
	// Also these functions keep the code clean and fast.
	// You can insert in O(1) time, if you know that it doesn't exists.
	// You can remove in O(1) time, if you know the position of the item.
	template<typename T, unsigned S>
	struct VectorSet :
		public Ogre::SmallVector<T, S> {
		typedef typename Ogre::SmallVector<T, S>::iterator iterator;

		void addNotExists(const T& item); // Complexity: O(1)!!
		void remove(iterator it); // Complexity: O(1)!!
		iterator add(const T& item); // Complexity: O(N)
		void removeExists(const T& item); // Complexity: O(N)
		bool remove(const T& item); // Complexity: O(N)
		void replaceExists(const T& oldItem, const T& newItem); // Complexity: O(N)
		bool has(const T& item); // Complexity: O(N)
		iterator find(const T& item); // Complexity: O(N)
		iterator findExists(const T& item); // Complexity: O(N)
	};

	struct PMEdge;
	struct PMVertex;
	struct PMTriangle;
	struct PMVertexHash;
	struct PMVertexEqual;
	struct PMCollapseCostLess;
	struct PMCollapsedEdge;
	struct PMIndexBufferInfo;

	typedef std::vector<PMVertex> VertexList;
	typedef std::vector<PMTriangle> TriangleList;
	typedef boost::unordered_set<PMVertex*, PMVertexHash, PMVertexEqual> UniqueVertexSet;
	typedef std::multiset<PMVertex*, PMCollapseCostLess> CollapseCostSet;
	typedef std::vector<PMVertex*> VertexLookupList;

	typedef VectorSet<PMEdge, 8> VEdges;
	typedef VectorSet<PMTriangle*, 7> VTriangles;

	typedef std::vector<PMCollapsedEdge> CollapsedEdges;
	typedef std::vector<PMIndexBufferInfo> IndexBufferInfoList;

	// Hash function for UniqueVertexSet.
	struct PMVertexHash {
		size_t operator() (const PMVertex* v) const;
	};

	// Equality function for UniqueVertexSet.
	struct PMVertexEqual {
		bool operator() (const PMVertex* lhs, const PMVertex* rhs) const;
	};

	// Comparator for CollapseCostSet.
	struct PMCollapseCostLess {
		bool operator() (const PMVertex* lhs, const PMVertex* rhs) const;
	};

	// Directed edge
	struct PMEdge {
		PMVertex* dst;
		Ogre::Real collapseCost;
		int refCount;

		explicit PMEdge(PMVertex* destination);
		bool operator== (const PMEdge& other) const;
		PMEdge& operator= (const PMEdge& b);
		PMEdge(const PMEdge& b);
		bool operator< (const PMEdge& other) const;
	};

	struct PMVertex {
		Ogre::Vector3 position;
		Ogre::Real collapseCost;
		VEdges edges;
		VTriangles triangles; // Triangle ID set, which are using this vertex.

		PMVertex* collapseTo;
		bool seam;
		CollapseCostSet::iterator costSetPosition; // Iterator pointing to the position in the mCollapseCostSet, which allows fast remove.
	};

	struct PMTriangle {
		PMVertex* vertex[3];
		Ogre::Vector3 normal;
		bool isRemoved;
		unsigned short submeshID; // ID of the submesh. Usable with mMesh.getSubMesh() function.
		unsigned int vertexID[3]; // Vertex ID in the buffer associated with the submeshID.

		void computeNormal();
		bool hasVertex(const PMVertex* v) const;
		unsigned int getVertexID(const PMVertex* v) const;
		bool isMalformed();
	};

	struct PMIndexBufferInfo {
		size_t indexSize;
		size_t indexCount;
	};

	union IndexBufferPointer {
		unsigned short* pshort;
		unsigned int* pint;
	};

	struct PMCollapsedEdge {
		unsigned int srcID;
		unsigned int dstID;
		unsigned short submeshID;
	};

	VertexLookupList mSharedVertexLookup;
	VertexLookupList mVertexLookup;
	VertexList mVertexList;
	TriangleList mTriangleList;
	UniqueVertexSet mUniqueVertexSet;
	CollapseCostSet mCollapseCostSet;
	CollapsedEdges tmpCollapsedEdges; // Tmp container used in collapse().
	IndexBufferInfoList mIndexBufferInfoList;

	Ogre::MeshPtr mMesh;

	/**
	 * @brief The name of the mesh being processed.
	 *
	 * This is separate from mMesh in order to allow for access from background threads.
	 */
	std::string mMeshName;
	Ogre::Real mMeshBoundingSphereRadius;
	Ogre::Real mCollapseCostLimit;

	size_t calcLodVertexCount(const LodLevel& lodConfig);
	void tuneContainerSize();
	void addVertexData(Ogre::VertexData* vertexData, bool useSharedVertexLookup);
	template<typename IndexType>
	void addIndexDataImpl(IndexType* iPos, const IndexType* iEnd, VertexLookupList& lookup, unsigned short submeshID);
	void addIndexData(Ogre::IndexData* indexData, bool useSharedVertexLookup, unsigned short submeshID);

	void computeCosts();
	bool isBorderVertex(const PMVertex* vertex) const;
	PMEdge* getPointer(VEdges::iterator it);
	void computeVertexCollapseCost(PMVertex* vertex);
	Ogre::Real computeEdgeCollapseCost(PMVertex* src, PMEdge* dstEdge);
	virtual void bakeLods();
	void collapse(PMVertex* vertex);
	void initialize();
	void computeLods(LodConfig& lodConfigs);
	void updateVertexCollapseCost(PMVertex* src);

	bool hasSrcID(unsigned int srcID, unsigned short submeshID);
	size_t findDstID(unsigned int srcID, unsigned short submeshID);
	void replaceVertexID(PMTriangle* triangle, unsigned int oldID, unsigned int newID, PMVertex* dst);

#ifndef NDEBUG
	void assertValidVertex(PMVertex* v);
	void assertValidMesh();
	void assertOutdatedCollapseCost(PMVertex* vertex);
#endif // ifndef NDEBUG

	void addTriangleToEdges(PMTriangle* triangle);
	void removeTriangleFromEdges(PMTriangle* triangle, PMVertex* skip = NULL);
	void addEdge(PMVertex* v, const PMEdge& edge);
	void removeEdge(PMVertex* v, const PMEdge& edge);
	void printTriangle(PMTriangle* triangle, std::stringstream& str);
	PMTriangle* findSideTriangle(const PMVertex* v1, const PMVertex* v2);
	bool isDuplicateTriangle(PMTriangle* triangle, PMTriangle* triangle2);
	PMTriangle* isDuplicateTriangle(PMTriangle* triangle);
	int getTriangleID(PMTriangle* triangle);
	void cleanupMemory();
};

}
}
}
#endif // ifndef PROGRESSIVEMESHGENERATOR_H
