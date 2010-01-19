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

#include "framework/tasks/TemplateNamedTask.h"
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
class TerrainPageSurfaceCompilationInstance;

/**
 * @brief Updates a terrain shader, i.e. the mercator surfaces.
 * This will also recompile the terrain page material once the surface has been updated.
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 */
class TerrainShaderUpdateTask : public Ember::Tasks::TemplateNamedTask<TerrainShaderUpdateTask>
{
public:
	/**
	 * @brief Ctor.
	 * @param pages The pages which needs their surfaces updated.
	 * @param shader The shader which for each page will be be applied.
	 * @param areas Any areas which define the area to update. This will only be applied if updateAll is set to false.
	 * @param updateAll Whether all pages should be updated. Setting this to true will make the system skip checking the areas sent.
	 * @param signal A signal which will be emitted in the main thread once all surfaces have been updated.
	 */
	TerrainShaderUpdateTask(PageVector& pages, const TerrainShader* shader, const AreaStore& areas, bool updateAll, sigc::signal<void, const TerrainShader*, const AreaStore*>& signal);
	virtual ~TerrainShaderUpdateTask();

	virtual void executeTaskInBackgroundThread(Ember::Tasks::TaskExecutionContext& context);

	virtual void executeTaskInMainThread();

private:

	/**
	 * @brief The pages which will be updated.
	 */
	const PageVector mPages;

	/**
	 * @brief The shader which will be applied.
	 */
	const TerrainShader* mShader;

	/**
	 * @brief An optional collection of areas. If mUpdateAll is set to false, only the pages affected by the areas will be updated.
	 */
	const AreaStore mAreas;

	/**
	 * @brief If set to true, all pages will be updated, never checking whether any pages are affected by areas.
	 */
	const bool mUpdateAll;

	/**
	 * @brief A signal to emit once the update is done.
	 */
	sigc::signal<void, const TerrainShader*, const AreaStore* >& mSignal;

};

}

}

#endif /* TERRAINSHADERUPDATETASK_H_ */
