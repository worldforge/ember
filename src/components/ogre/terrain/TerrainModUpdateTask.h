/*
 Copyright (C) 2009 Erik Ogenvik <erik@ogenvik.org>

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef TERRAINMODADDTASK_H_
#define TERRAINMODADDTASK_H_
#include "TerrainModTranslator.h"
#include "framework/tasks/TemplateNamedTask.h"
#include <Mercator/TerrainMod.h>
#include <Atlas/Message/Element.h>

namespace Mercator
{
class Terrain;
class TerrainMod;
}

namespace Ember
{
namespace OgreView
{

namespace Terrain
{
class TerrainHandler;
class TerrainMod;

/**
 * @author Erik Ogenvik <erik@ogenvik.org>
 * @brief Task for adding a new terrain mod.
 */
class TerrainModUpdateTask: public Tasks::TemplateNamedTask<TerrainModUpdateTask>
{
public:
	TerrainModUpdateTask(Mercator::Terrain& terrain, const TerrainMod& terrainMod, TerrainHandler& handler);
	~TerrainModUpdateTask() override = default;

	void executeTaskInBackgroundThread(Tasks::TaskExecutionContext& context) override;

	bool executeTaskInMainThread() override;

private:

	/**
	 * @brief The terrain.
	 */
	Mercator::Terrain& mTerrain;

	/**
	 * @brief The terrain manager.
	 */
	TerrainHandler& mHandler;

	/**
	 * @brief A list of updates areas. Any geometry in these areas will need to be recalculated.
	 */
	std::vector<WFMath::AxisBox<2>> mUpdatedAreas;

	long mId;

	const WFMath::Point<3> mPosition;
	const WFMath::Quaternion& mOrientation;
	Ember::Terrain::TerrainModTranslator mTranslator;

};

}

}

}

#endif /* TERRAINMODADDTASK_H_ */
