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

#ifndef TERRAINPAGECREATIONTASK_H_
#define TERRAINPAGECREATIONTASK_H_

#include "domain/Types.h"
#include "framework/tasks/TemplateNamedTask.h"
#include <wfmath/point.h>
#include <wfmath/vector.h>

namespace Ember {
namespace OgreView {

namespace Terrain {

class TerrainHandler;

class TerrainPage;

class ITerrainPageBridge;

class HeightMapBufferProvider;

class HeightMap;

class TerrainPageCreationTask : public Tasks::TemplateNamedTask<TerrainPageCreationTask> {
public:
	TerrainPageCreationTask(TerrainHandler& handler,
							std::shared_ptr<Terrain::TerrainPage> page,
							HeightMapBufferProvider& heightMapBufferProvider,
							HeightMap& heightMap);

	~TerrainPageCreationTask() override = default;

	void executeTaskInBackgroundThread(Tasks::TaskExecutionContext& context) override;

	bool executeTaskInMainThread() override;

private:
	TerrainHandler& mTerrainHandler;

	std::shared_ptr<Terrain::TerrainPage> mPage;

	HeightMapBufferProvider& mHeightMapBufferProvider;
	HeightMap& mHeightMap;
};

}

}

}

#endif /* TERRAINPAGECREATIONTASK_H_ */
