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

#include<OgrePrerequisites.h>
#include<OgreVector3.h>
#include <OgreHardwareIndexBuffer.h>

#include <boost/unordered_set.hpp>
#include <boost/numeric/ublas/matrix.hpp>

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
	};


	/**
	 * @brief Vector of Lod configurations.
	 */
	typedef std::vector<LodConfig> LodConfigList;

	/**
	 * @brief Ctor.
	 */
	ProgressiveMeshGenerator::ProgressiveMeshGenerator(Ogre::Mesh& mesh);
	~ProgressiveMeshGenerator();

	/**
	 * @brief Builds the Lods for a submesh based on a LodConfigList.
	 *
	 * @param lodConfigs Specification of the requested lods.
	 * @param outList The output of the algorithm.
	 */
	void build(const LodConfigList& lodConfigs);
private:
	struct PMEdge;
	struct PMVertex;
	struct PMTriangle;
	struct PMVertexHash;
	struct PMCollapseCostLess;

	typedef boost::unordered_set<PMVertex, PMVertexHash> UniqueVertexSet;
	typedef std::multiset<PMVertex*, PMCollapseCostLess> CollapseCostSet;
	typedef std::vector<PMVertex*> SubmeshVertexLookupList;
	typedef std::vector<SubmeshVertexLookupList> VertexLookupList;
	typedef std::vector<PMTriangle> TriangleList;
	typedef std::set<PMEdge> VertexEdges;
	typedef std::set<size_t> VertexTriangles;

	// Hash function for UniqueVertexSet.
	struct PMVertexHash
		: std::unary_function<PMVertex, size_t> {
		size_t operator() (const PMVertex& v) const
		{
			size_t seed = 0;
			boost::hash_combine(seed, v.position.x);
			boost::hash_combine(seed, v.position.y);
			boost::hash_combine(seed, v.position.z);
			return seed;
		}
	};

	// Comparator for CollapseCostSet.
	struct PMCollapseCostLess {
		bool operator() (const PMVertex* lhs, const PMVertex* rhs) const
		{
			return lhs->collapseCost < rhs->collapseCost;
		}
	};

	// Directed edge
	struct PMEdge {
		PMVertex* dst;
		float collapseCost;
		int refCount;

		explicit PMEdge(PMVertex* destination);
		bool operator< (const PMEdge& other) const;
	};

	struct PMVertex {
		const Ogre::Vector3 position;
		float collapseCost;

		VertexEdges edges;
		VertexTriangles triangles; // Triangle ID set, which are using this vertex.

		PMVertex* collapseTo;
		bool seam;

		CollapseCostSet::iterator costSetPosition; // Iterator pointing to the position in the mCollapseCostSet, which allows fast remove.

		// Equality comparator for UniqueVertexSet (unordered_set).
		// Note: std::set is checking equality like (a<b && b<a), so CollapseCostSet is not using this.
		bool operator== (const PMVertex& other) const;

		explicit PMVertex(Ogre::Real x, Ogre::Real y, Ogre::Real z) :
			position(x, y, z) { }

		void addEdge(const PMEdge& edge);
		void removeEdge(const PMEdge& edge);
	};

	struct PMTriangle {
		PMVertex* vertex[3];
		Ogre::Vector3 normal;

		unsigned short minLod; // Triangle is only used by Lod levels, which are bigger, then minLod.
		unsigned short maxLod; // Triangle is only used by Lod levels, which are smaller, then maxLod.
		unsigned short submeshID; // ID of the submesh. Usable with mMesh.getSubMesh() function.
		unsigned int vertexID[3]; // Vertex ID in the buffer associated with the submeshID.

		void computeNormal();
		bool hasVertex(const PMVertex* v) const;
		void replaceVertexID(unsigned int oldID, unsigned int newID, PMVertex* dst);
		unsigned int getVertexID(const PMVertex* v) const;
		void removeTriangle(VertexTriangles::iterator it);
		void addTriangle(VertexTriangles::iterator it);
		void invalidateEdgeCosts();
	};

	struct IndexBufferInfo {
		size_t indexSize;
		size_t indexCount;
	};

	struct CollapsedEdge {
		unsigned int srcID;
		unsigned int dstID;
		unsigned short submeshID;
		CollapsedEdge(unsigned int _srcID, unsigned int _dstID, unsigned short _submeshID) :
			srcID(_srcID), dstID(_dstID), submeshID(_submeshID)
		{

		}
	};

	Ogre::Mesh& mMesh;
	UniqueVertexSet mUniqueVertexSet;
	CollapseCostSet mCollapseCostSet;
	TriangleList mTriangleList;
	VertexLookupList mVertexLookup; // This is only used at the beginning, when generating triangles from index buffer.
	unsigned short mCurLod;
	IndexBufferInfo* mIndexBufferInfoList; // This is needed to prevent reopening the index buffer in bakeLod().
	typedef boost::numeric::ublas::matrix<int> LodIndexCountMatrix;
	LodIndexCountMatrix mLodIndexCountMatrix;

	typedef std::vector<CollapsedEdge> CollapsedEdges; // Tmp typedef.
	CollapsedEdges tmpCollapsedEdges; // Tmp container used in collapse().

	size_t calcLodVertexCount(const LodConfig& lodConfig);
	void tuneContainerSize();
	template<typename indexType>
	void addIndexDataImpl(const Ogre::HardwareIndexBufferSharedPtr& ibuf,
	                      const std::vector<PMVertex*>& vertices,
	                      Ogre::VertexData* vertexData,
	                      Ogre::IndexData* indexData,
	                      unsigned short submeshID);
	void addIndexData(Ogre::VertexData* vertexData, Ogre::IndexData* indexData, bool verticesAdded, unsigned short submeshID);
	void computeCosts();

	bool isBorderVertex(const PMVertex* vertex) const;
	PMVertex* getPointer(UniqueVertexSet::iterator it);
	PMEdge* getPointer(VertexEdges::iterator it);
	void computeVertexCollapseCost(PMVertex* vertex);
	float computeEdgeCollapseCost(PMVertex* src, PMEdge* dstEdge);

	void bakeLods(const LodConfigList& lodConfigs);
	void collapse(PMVertex* vertex);
	void initialize();
	void computeLods(const LodConfigList& lodConfigs);

	void updateVertexCollapseCost(PMVertex* src);

	bool hasSrcID(unsigned int srcID, unsigned short submeshID);
	size_t findDstID(unsigned int srcID, unsigned short submeshID);

	void assertValidVertex(PMVertex* v);
	void assertValidMesh();

	void addTriangleToEdges(size_t triangleID);
	void removeTriangleFromEdges(size_t triangleID);
	void removeTriangleFromEdges(size_t triangleID, PMVertex* skip = NULL);

	PMEdge* addEdge(PMVertex* v, const PMEdge& edge);
	PMEdge* removeEdge(PMVertex* v, const PMEdge& edge);
	void printTriangle(PMTriangle* triangle, std::stringstream& str);
	PMTriangle* findSideTriangle(const PMVertex* v1, const PMVertex* v2);
	void assertOutdatedCollapseCost(PMVertex* vertex);
};

}
}
}
#endif // ifndef PROGRESSIVEMESHGENERATOR_H
