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

#ifndef TERRAINSHADERUPDATETASK_H_
#define TERRAINSHADERUPDATETASK_H_

#include "framework/tasks/ITask.h"
#include "Types.h"
#include <vector>
#include <map>
#include <string>
#include <sigc++/signal.h>

namespace WFMath
{
template<int> class AxisBox;
}


namespace EmberOgre
{

namespace Terrain
{

class TerrainShader;
class TerrainPage;

class TerrainShaderUpdateTask : public Ember::Tasks::ITask
{
public:
	TerrainShaderUpdateTask(PageStore& pages, const TerrainShader* shader, const AreaStore& areas, bool updateAll, sigc::signal<void, const TerrainShader*, const AreaStore*>& signal);
	virtual ~TerrainShaderUpdateTask();

	virtual void executeTaskInBackgroundThread(Ember::Tasks::TaskExecutionContext& context);

	virtual void executeTaskInMainThread();

private:

	const PageStore mPages;
	const TerrainShader* mShader;
	const AreaStore mAreas;
	const bool mUpdateAll;
	sigc::signal<void, const TerrainShader*, const AreaStore* >& mSignal;

};

}

}

#endif /* TERRAINSHADERUPDATETASK_H_ */
