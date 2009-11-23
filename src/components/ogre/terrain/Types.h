/*
 Copyright (C) 2009 Erik Hjortsberg <erik.hjortsberg@gmail.com>

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

#ifndef EMBEROGRETERRAINTYPES_H_
#define EMBEROGRETERRAINTYPES_H_

#include "components/ogre/Types.h"

#include <vector>
#include <map>
#include <string>
#include <wfmath/point.h>

namespace Mercator {
	class Area;
	class Terrain;
	class Shader;
	class TerrainMod;
}

namespace WFMath
{
	template<int> class AxisBox;
}

namespace EmberOgre {

	namespace Terrain {

		class TerrainPage;
		class TerrainShader;

		/**
		 @brief Defines the height of a special "base point" in the terrain.
		 These base points are then user by Mercator::Terrain for generating the actual terrain.
		 */
		class TerrainDefPoint
		{
		public:
			/**
			 *       Ctor.
			 * @param x The position of the point, on the x axis, in world units.
			 * @param y The position of the point, on the y axis, in world units.
			 * @param terrainHeight The height of the point, in world units.
			 */
			TerrainDefPoint(float x, float y, float terrainHeight) : mPosition(x,y), mHeight(terrainHeight) {}

			/**
			 * @brief Gets the position of the definition point, in world units.
			 * @return The position of the point.
			 */
			const TerrainPosition& getPosition() const;

			/**
			 * @brief Gets the height of the definition point, in world units.
			 * @return The height of the point.
			 */
			float getHeight() const;

		private:
			/**
			 The position of the point, in world units.
			 */
			TerrainPosition mPosition;

			/**
			 The height of the point, in world units.
			 */
			float mHeight;
		};

		/**
		 * @brief A type used for storing changes to areas. We use instances instead of pointers or references since this type will be used in delayed updating, where the originating instance might not any longer be around.
		 */
		typedef std::vector<WFMath::AxisBox<2> > AreaStore;

		/**
		 * @brief A type used for storing the terrain definition points.
		 */
		typedef std::vector<TerrainDefPoint> TerrainDefPointStore;

		/**
		 * @brief STL map to store sparse array of TerrainPage pointers.
		 */
		typedef std::map<int, TerrainPage *> TerrainPagecolumn;

		/**
		 * @brief STL map to store sparse array of TerrainPage pointer columns.
		 */
		typedef std::map<int, TerrainPagecolumn > TerrainPagestore;

		/**
		 * @brief Encapsules a shader update request.
		 */
		struct ShaderUpdateRequest
		{
			/**
			 * @brief A list of areas that have been changed.
			 * Unless UpdateAll is true, this should be used for determining what geometry needs updating.
			 */
			AreaStore Areas;

			/**
			 * @brief If this is set to true, all geometry should be updated, no matter what areas are specified in Areas.
			 */
			bool UpdateAll;
		};

		typedef std::map<int, const TerrainShader*> AreaShaderstore;

		typedef std::map<const TerrainShader*, ShaderUpdateRequest> ShaderUpdateSet;

		typedef std::map<std::string, TerrainPage*> PageStore;
		/**
		 A type for use when keeping track of changes done to areas. We use instances of Mercator::Area instead of pointers or references since we want to batch the updates, and the original area instances might not be around at that time.
		 */
		typedef std::map<const TerrainShader*, std::vector<Mercator::Area> > TerrainAreaMap;

		typedef std::multimap<const std::string, Mercator::TerrainMod*> TerrainModMap;

		typedef std::map<const Mercator::Shader*, const TerrainShader*> ShaderStore;
	}
}

#endif /* EMBEROGRETERRAINTYPES_H_ */
