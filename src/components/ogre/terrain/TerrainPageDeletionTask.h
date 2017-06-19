/*
 Copyright (C) 2013 Erik Ogenvik

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

#ifndef TERRAINPAGEDELETIONTASK_H_
#define TERRAINPAGEDELETIONTASK_H_

#include "framework/tasks/TemplateNamedTask.h"

namespace Ember
{
namespace OgreView
{
namespace Terrain
{

class TerrainPage;

/**
 * @brief Deletes a page on the main thread.
 *
 * This task only deletes a page. The reason for having it as a task is that we want to make sure no other tasks
 * are using the page when it's deleted.
 */
class TerrainPageDeletionTask: public Tasks::TemplateNamedTask<TerrainPageDeletionTask>
{
public:
	TerrainPageDeletionTask(TerrainPage* page);
	virtual ~TerrainPageDeletionTask();

	virtual void executeTaskInBackgroundThread(Tasks::TaskExecutionContext& context);

	virtual bool executeTaskInMainThread();

private:
	TerrainPage* mPage;
};

}
}
}
#endif /* TERRAINPAGEDELETIONTASK_H_ */
