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

#ifndef EMBEROGRETERRAINTERRAINAREAUPDATETASK_H_
#define EMBEROGRETERRAINTERRAINAREAUPDATETASK_H_

#include "framework/tasks/TemplateNamedTask.h"
#include "Types.h"
#include "TerrainHandler.h"
#include "TerrainLayerDefinitionManager.h"
#include <Mercator/Area.h>

#include <wfmath/point.h>
#include <wfmath/axisbox.h>

#include <sigc++/slot.h>
#include <Mercator/Terrain.h>


namespace Ember {
namespace OgreView {

namespace Terrain {

/**
 * @author Erik Ogenvik <erik@ogenvik.org>
 * @brief Updates terrain areas.
 */
class TerrainAreaUpdateTask : public Tasks::TemplateNamedTask<TerrainAreaUpdateTask> {
public:
	typedef sigc::slot<void, int, const WFMath::AxisBox<2>&> ShaderUpdateSlotType;

	/**
	 * @brief Ctor.
	 * @param terrain The terrain.
	 * @param area The terrain area which is updated.
	 * @param shader The affected shader.
	 * @param markForUpdateSlot A slot which will be called in the main thread when the update is complete.
	 * @param oldShape The old shape, before the update.
	 */
	TerrainAreaUpdateTask(Mercator::Terrain& terrain,
						  long id,
						  std::unique_ptr<Mercator::Area> area,
						  ShaderUpdateSlotType markForUpdateSlot);

	~TerrainAreaUpdateTask() override;

	void executeTaskInBackgroundThread(Tasks::TaskExecutionContext& context) override;

	bool executeTaskInMainThread() override;

private:

	/**
	 * @brief The terrain.
	 */
	Mercator::Terrain& mTerrain;

	long mId;

	/**
	 * @brief The terrain area.
	 */
	std::unique_ptr<Mercator::Area> mArea;

	ShaderUpdateSlotType mShaderUpdateSlot;

	long mLayerId;

	WFMath::AxisBox<2> mOldShape, mNewShape;


};

}
}
}
#endif /* EMBEROGRETERRAINTERRAINAREAUPDATETASK_H_ */
