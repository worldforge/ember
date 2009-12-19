/*
 Copyright (C) 2009 erik

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

#ifndef EMBEROGRETERRAINTERRAINAREAREMOVETASK_H_
#define EMBEROGRETERRAINTERRAINAREAREMOVETASK_H_

#include "TerrainAreaTaskBase.h"

namespace EmberOgre
{

namespace Terrain
{

class TerrainShader;


/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 * @brief Removes terrain areas.
 */
class TerrainAreaRemoveTask : public TerrainAreaTaskBase
{
public:
	TerrainAreaRemoveTask(Mercator::Terrain& terrain, Mercator::Area& terrainArea, ShaderUpdateSlotType markForUpdateSlot, const TerrainShader* shader);
	virtual ~TerrainAreaRemoveTask();

	virtual void executeTaskInBackgroundThread(Ember::Tasks::TaskExecutionContext& context);

	virtual void executeTaskInMainThread();

private:

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

#endif /* EMBEROGRETERRAINTERRAINAREAREMOVETASK_H_ */
