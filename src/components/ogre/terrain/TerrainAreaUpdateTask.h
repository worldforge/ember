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

#ifndef EMBEROGRETERRAINTERRAINAREAUPDATETASK_H_
#define EMBEROGRETERRAINTERRAINAREAUPDATETASK_H_
#include "framework/tasks/ITask.h"
#include <sigc++/slot.h>

namespace Mercator {

	class Terrain;
	class Area;

} // namespace Mercator

namespace EmberOgre
{

	namespace Terrain
	{
		class TerrainArea;
		class TerrainShader;

		/**
		 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
		 * @brief Updates terrain areas.
		 */
		class TerrainAreaUpdateTask: public Ember::Tasks::ITask
		{
		public:

			/**
			 * @brief Ctor.
			 * @param terrain The terrain.
			 * @param terrainArea The terrain area which is updated.
			 * @param shader The affected shader.
			 * @param markForUpdateSlot A slot which will be called in the main thread when the update is complete.
			 */
			TerrainAreaUpdateTask(Mercator::Terrain& terrain, TerrainArea& terrainArea, const TerrainShader* shader, sigc::slot<void, const TerrainShader*, Mercator::Area*> markForUpdateSlot);
			virtual ~TerrainAreaUpdateTask();

			virtual void executeTaskInBackgroundThread(Ember::Tasks::TaskExecutionContext& context);

			virtual void executeTaskInMainThread();

		private:
			/**
			 * @brief The terrain.
			 */
			Mercator::Terrain& mTerrain;

			/**
			 * @brief The terrain area.
			 */
			TerrainArea& mTerrainArea;

			/**
			 * @brief The terrain shader affected.
			 */
			const TerrainShader* mShader;

			/**
			 * @brief A slot which will be called in the main thread when the area update is complete.
			 */
			sigc::slot<void, const TerrainShader*, Mercator::Area*> mMarkForUpdateSlot;
		};

	}
}
#endif /* EMBEROGRETERRAINTERRAINAREAUPDATETASK_H_ */
