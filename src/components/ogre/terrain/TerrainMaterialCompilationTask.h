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

#ifndef EMBEROGRETERRAINTERRAINMATERIALCOMPILATIONTASK_H_
#define EMBEROGRETERRAINTERRAINMATERIALCOMPILATIONTASK_H_

#include "framework/tasks/TemplateNamedTask.h"
#include "Types.h"
#include <vector>

namespace EmberOgre
{

namespace Terrain
{

class TerrainPageSurfaceCompilationInstance;
class TerrainPage;

/**
 * @brief Recompiles the material for a terrain page.
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 */
class TerrainMaterialCompilationTask : public Ember::Tasks::TemplateNamedTask<TerrainMaterialCompilationTask>
{
public:

	/**
	 * @brief Ctor.
	 * @param pages The pages which needs to have their material recompiled.
	 */
	TerrainMaterialCompilationTask(const PageVector& pages);

	/**
	 * @brief Ctor.
	 * @param page The page which needs to have its material recompiled.
	 */
	TerrainMaterialCompilationTask(TerrainPage* page);

	/**
	 * @brief Dtor.
	 */
	virtual ~TerrainMaterialCompilationTask();

	virtual void executeTaskInBackgroundThread(Ember::Tasks::TaskExecutionContext& context);

	virtual void executeTaskInMainThread();
private:

	/**
	 * @brief A store of terrain page surface compilation instances, and the page to which they belong.
	 */
	typedef std::vector<std::pair<TerrainPageSurfaceCompilationInstance*, TerrainPage*> > CompilationInstanceStore;

	/**
	 * @brief The pages which needs recompilation.
	 */
	PageVector mPages;

	/**
	 * @brief The compilation instances and their corresponding pages.
	 */
	CompilationInstanceStore mMaterialRecompilations;

	/**
	 * @brief This needs to be called after materials have changed to make sure that Ogre flushes it's material caches.
	 * Failure to do so will result in assert errors during Ogre's rendering.
	 */
	void updateSceneManagersAfterMaterialsChange();


};

}

}

#endif /* EMBEROGRETERRAINTERRAINMATERIALCOMPILATIONTASK_H_ */
