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

#include <OgreProgressiveMesh.h>

namespace Ember
{
namespace OgreView
{
namespace Lod
{


/**
 * @brief Improved version of Ogre::ProgressiveMesh.
 */
class ProgressiveMeshGenerator :
	public Ogre::ProgressiveMesh
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
	ProgressiveMeshGenerator::ProgressiveMeshGenerator(const Ogre::VertexData* vertexData, const Ogre::IndexData* indexData);

	/**
	 * @brief Builds the Lods for a submesh based on a LodConfigList.
	 * 
	 * @param lodConfigs Specification of the requested lods.
	 * @param outList The output of the algorithm.
	 */
	void build(const LodConfigList& lodConfigs, LODFaceList& outList);
private:
	/**
	 * @brief Calculates the user requested vertex count for a LodConfig
	 */
	size_t calcLodVertexCount(const LodConfig& lodConfig);
};

}
}
}
#endif // ifndef PROGRESSIVEMESHGENERATOR_H
