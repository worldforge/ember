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

#ifndef GEOMETRYUPDATETASK_H_
#define GEOMETRYUPDATETASK_H_

#include "framework/tasks/ITask.h"
#include "framework/tasks/TemplateNamedTask.h"
#include "Types.h"
#include <wfmath/vector.h>
#include <set>

namespace Ember {
namespace OgreView {

namespace Terrain {
class TerrainPage;

class TerrainHandler;

class HeightMapBufferProvider;

class HeightMap;

struct TerrainShader;

class GeometryUpdateTask : public Tasks::TemplateNamedTask<GeometryUpdateTask> {
public:
	GeometryUpdateTask(BridgeBoundGeometryPtrVector geometry,
					   std::vector<WFMath::AxisBox<2>> areas,
					   TerrainHandler& handler,
					   std::vector<Terrain::TerrainShader> shaders,
					   HeightMapBufferProvider& heightMapBufferProvider,
					   HeightMap& heightMap);

	~GeometryUpdateTask() override = default;

	void executeTaskInBackgroundThread(Tasks::TaskExecutionContext& context) override;

	bool executeTaskInMainThread() override;

private:

	BridgeBoundGeometryPtrVector mGeometry;
	const std::vector<WFMath::AxisBox<2>> mAreas;
	TerrainHandler& mHandler;
	std::vector<Terrain::TerrainShader> mShaders;
	HeightMapBufferProvider& mHeightMapBufferProvider;
	HeightMap& mHeightMap;
	std::set<TerrainPage*> mPages;
	std::set<ITerrainPageBridgePtr> mBridgesToNotify;


};

}

}

}

#endif /* GEOMETRYUPDATETASK_H_ */
