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

#ifndef PROGRESSIVEMESHGENERATOR_H
#define PROGRESSIVEMESHGENERATOR_H

#include <OgrePrerequisites.h>
#include <OgreVector3.h>
#include <OgreHardwareIndexBuffer.h>
#include "OgreSmallVector.h"

#include <boost/unordered_set.hpp>

#include <vector>
#include <set>

namespace Ember
{
namespace OgreView
{
namespace Lod
{


/**
 * @brief Improved version of Ogre::ProgressiveMesh.
 */
class ProgressiveMeshGenerator
{
public:
	/**
	 * @brief Type of the reduction.
	 *
	 * Note: The vertex count is determined by unique vertices per submesh.
	 * A mesh may have duplicate vertices with same position.
	 */
	enum VertexReductionMethod {
		/**
		 * @brief Percentage of vertexes to be removed from each submesh.
		 *
		 * Valid range is a number between 0.0 and 1.0
		 */
		VRM_PROPORTIONAL,

		/**
		 * @brief Exact vertex count to be removed from each submesh.
		 *
		 * Pass only integers or it will be rounded.
		 */
		VRM_CONSTANT,

		/**
		 * @brief Reduces the vertices, until the cost is bigger then the given value.
		 *
		 * Collapse cost is equal to the amount of artifact the reduction causes.
		 * This generates the best Lod output, but the collapse cost depends on implementation.
		 */
		VRM_COLLAPSE_COST
	};

	/**
	 * @brief Structure for automatic Lod configuration.
	 */
	struct LodConfig {
		/**
		 * @brief Distance to swap the Lod.
		 *
		 * This depends on LodStrategy.
		 */
		Ogre::Real distance;

		/**
		 * @brief Reduction method to use.
		 *
		 * @see ProgressiveMeshGenerator::VertexReductionMethod
		 */
		VertexReductionMethod reductionMethod;

		/**
		 * @brief The value, which depends on reductionMethod.
		 */
		Ogre::Real reductionValue;

		/**
		 * @brief This is set by ProgressiveMeshGenerator::build() function.
		 *
		 * Use Mesh::getNumLodLevels() for generated Lod count.
		 */
		size_t outUniqueVertexCount;

		/**
		 * @brief Whether the Lod level generation was skipped, because it has same vertex count as the previous Lod level.
		 */
		bool outSkipped;
	};


	/**
	 * @brief Vector of Lod configurations.
	 */
	typedef std::vector<LodConfig> LodConfigList;

	/**
	 * @brief Ctor.
	 */
	ProgressiveMeshGenerator(Ogre::Mesh& mesh);
	~ProgressiveMeshGenerator();

	/**
	 * @brief Builds the Lod levels for a submesh based on a LodConfigList.
	 *
	 * @param lodConfigs Specification of the requested Lod levels.
	 */
	void build(LodConfigList& lodConfigs);
private:

	// VectorSet is basically a helper to use a vector as a small set container.
	// Also these functions keep the code clean and fast.
	// You can insert in O(1) time, if you know that it doesn't exists.
	// You can remove in O(1) time, if you know the position of the item.
	template<typename T, unsigned S>
	struct VectorSet :
		public Ogre::SmallVector<T, S> {
		typedef typename Ogre::SmallVector<T, S> baseClass;
		typedef typename baseClass::iterator iterator;
		 
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

	Ogre::Mesh& mMesh;
	unsigned short mCurLod;

	size_t calcLodVertexCount(const LodConfig& lodConfig);
	void tuneContainerSize();
	void addVertexData(Ogre::VertexData* vertexData, bool useSharedVertexLookup);
	template<typename indexType>
	void addIndexDataImpl(const Ogre::HardwareIndexBufferSharedPtr& ibuf, VertexLookupList& lookup, unsigned short submeshID);
	void addIndexData(Ogre::IndexData* indexData, bool useSharedVertexLookup, unsigned short submeshID);

	void computeCosts();
	bool isBorderVertex(const PMVertex* vertex) const;
	PMEdge* getPointer(VEdges::iterator it);
	void computeVertexCollapseCost(PMVertex* vertex);
	Ogre::Real computeEdgeCollapseCost(PMVertex* src, PMEdge* dstEdge);
	void bakeLods(const LodConfigList& lodConfigs);
	void collapse(PMVertex* vertex);
	void initialize();
	void computeLods(LodConfigList& lodConfigs);
	void updateVertexCollapseCost(PMVertex* src);

	bool hasSrcID(unsigned int srcID, unsigned short submeshID);
	size_t findDstID(unsigned int srcID, unsigned short submeshID);
	void replaceVertexID(PMTriangle* triangle, unsigned int oldID, unsigned int newID, PMVertex* dst);

	void assertValidVertex(PMVertex* v);
	void assertValidMesh();

	void addTriangleToEdges(PMTriangle* triangle);
	void removeTriangleFromEdges(PMTriangle* triangle, PMVertex* skip = NULL);
	void addEdge(PMVertex* v, const PMEdge& edge);
	void removeEdge(PMVertex* v, const PMEdge& edge);
	void printTriangle(PMTriangle* triangle, std::stringstream& str);
	PMTriangle* findSideTriangle(const PMVertex* v1, const PMVertex* v2);
	void assertOutdatedCollapseCost(PMVertex* vertex);
	bool isDuplicateTriangle(PMTriangle* triangle, PMTriangle* triangle2);
	PMTriangle* isDuplicateTriangle(PMTriangle* triangle);
	int getTriangleID(PMTriangle* triangle);

};

}
}
}
#endif // ifndef PROGRESSIVEMESHGENERATOR_H
