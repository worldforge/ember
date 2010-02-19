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

#ifndef GEOMETRYUPDATETASK_H_
#define GEOMETRYUPDATETASK_H_

#include "framework/tasks/ITask.h"
#include "framework/tasks/TemplateNamedTask.h"
#include "Types.h"
#include <set>

namespace EmberOgre
{

namespace Terrain
{
class TerrainPage;
class TerrainManager;
class HeightMapBufferProvider;
class HeightMap;

class GeometryUpdateTask : public Ember::Tasks::TemplateNamedTask<GeometryUpdateTask>
{
public:
	GeometryUpdateTask(const GeometryPtrVector& geometry, const std::vector<TerrainPosition>& positions, TerrainManager& manager, const ShaderStore& shaders, HeightMapBufferProvider& heightMapBufferProvider, HeightMap& heightMap);
	virtual ~GeometryUpdateTask();

	virtual void executeTaskInBackgroundThread(Ember::Tasks::TaskExecutionContext& context);

	virtual void executeTaskInMainThread();

private:

	GeometryPtrVector mGeometry;
	const std::vector<TerrainPosition> mPositions;
	TerrainManager& mManager;
	ShaderStore mShaders;
	HeightMapBufferProvider& mHeightMapBufferProvider;
	HeightMap& mHeightMap;
	std::set<TerrainPage*> mPages;

};

}

}

#endif /* GEOMETRYUPDATETASK_H_ */
